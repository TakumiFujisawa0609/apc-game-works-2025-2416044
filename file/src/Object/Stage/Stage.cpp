#include <algorithm>
#include <iostream>
#include "../../Common/GeometryDxLib.h"
#include "../../Manager/AudioManager.h"
#include "../../Manager/FontManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Object/Player/Player.h"
#include "../../Scene/GameScene/GameScene.h"
#include "../../Utility/Utility.h"
#include "../Camera/Camera.h"
#include "Block.h"
#include "Stage.h"

int Stage::spinFrameIndex_ = 0;

Stage::Stage(GameScene* ptr) :
	SPIN_DEGREE(90.0f / SPIN_FRAME[spinFrameIndex_]),
	SPIN_DELAY_FRAME(int(SPIN_FRAME[0] + SPIN_FRAME[spinFrameIndex_] * 1.5f)),
	gameScene_(ptr) {}

bool Stage::SystemInit() {
	blockModels_[(size_t)Block::TYPE::NORMAL] = MV1LoadModel("Data/Model/Blocks/Block_Stone.mv1");
	if (blockModels_[(size_t)Block::TYPE::NORMAL] == -1) return false;

	blockModels_[(size_t)Block::TYPE::ADVANTAGE] = MV1LoadModel("Data/Model/Blocks/Block_Blank.mv1");
	if (blockModels_[(size_t)Block::TYPE::ADVANTAGE] == -1) return false;

	blockModels_[(size_t)Block::TYPE::FORBIDDEN] = MV1LoadModel("Data/Model/Blocks/Block_Crate.mv1");
	if (blockModels_[(size_t)Block::TYPE::FORBIDDEN] == -1) return false;

	return true;
}

bool Stage::GameInit() {
	blockWidth_ = 4;

	cubeDepth_ = 2;
	wave_ = 3;
	phase_ = 0;
	stage_ = 1;

	platformDepth_ = RoundDown(cubeDepth_ * wave_ * PLATFORM_DEPTH_MULT);

	for (int pd = 0; pd < platformDepth_; ++pd) {
		auto& ptr = platformList_.emplace_back();
		ptr = new Block(blockWidth_);
		ptr->SetType(Block::TYPE::PLATFORM);
		ptr->SetModelHandle(blockModels_[0]);
		ptr->SetStageIndex(0, pd);
		ptr->SetPosition(
			{ Block::HALF_BLOCK_SIZE, -Block::HALF_BLOCK_SIZE, pd * (-Block::BLOCK_SIZE) - Block::HALF_BLOCK_SIZE }
		);
	}

	// キューブ
	SetUpCube();

	spinTimer_ = 0;
	waveEndDelay_ = -1;

	gameStart_ = false;
	startwave_ = false;
	nextwave_ = false;
	isSpinning_ = false;
	fastForward_ = false;
	isClear_ = 0;

	fallCount_ = 0;
	waveFallCount_ = 0;

	trapVanish_ = false;
	advVanishCount_ = 0;

	perfectCamTimer_ = 0;

	return true;
}

void Stage::Update() {
	if (isClear_) return;

	if (!isSpinning_) { // 回転終了後の処理
		UpdateStop();
	}
	else { // 回転中の処理
		UpdateSpin();
	}

	for (auto& waveList : cubeList_) for (auto& subList : waveList) for (auto& cube : subList) {
		cube->Update();
	}

	if (fallCount_ >= blockWidth_) {
		fallCount_ -= blockWidth_;

		for (auto rit = platformList_.rbegin(); rit != platformList_.rend(); rit++) {
			if ((*rit)->GetState() == Block::STATE::NONE) {
				(*rit)->ChangeState(Block::STATE::ALERT);
				AudioManager::GetInstance().PlaySE("足場崩壊");
				break;
			}
		}
	}

	int count = 0;
	prevPlatformDepth_ = platformDepth_;
	for (auto& platform : platformList_) {
		platform->Update();

		if (platform->GetState() != Block::STATE::FALL &&
			platform->GetState() != Block::STATE::ADD) count++;

		if (!platform->IsAlive()) {
			delete platform;
			platformList_.remove(platform);
			break;
		}
	}
	platformDepth_ = count;

	if (trapVanish_) {
		gameScene_->AddScore(SCORE_LIST[0]);
		trapVanish_ = false;
	}
	if (advVanishCount_ > 0) {
		if (advVanishCount_ > SCORE_LIST_MAX) {
			advVanishCount_ = SCORE_LIST_MAX;
		}
		gameScene_->AddScore(SCORE_LIST[advVanishCount_ - 1]);
		advVanishCount_ = 0;
	}

	if (perfectCamTimer_ > 0) {
		perfectCamTimer_--;
		gameScene_->GetCameraPtr()->ChangeCameraMode(Camera::MODE::FIXED_PERFECT);
	}
	else {
		gameScene_->GetCameraPtr()->ChangeCameraMode(Camera::MODE::FOLLOW);
	}
}

void Stage::Draw() {
	// 足場
	for (auto& platform : platformList_) {
		platform->Draw();
	}

	// キューブ
	auto np = SceneManager::GetInstance().IsPause(),
		pp = SceneManager::GetInstance().PrevPause();
	bool nowWave = true;
	for (auto rit = cubeList_.rbegin(); rit != cubeList_.rend(); ++rit) {
		for (auto& subList : (*rit)) for (auto& cube : subList) {
			if (nowWave && startwave_) {
				if (np && !pp)
					cube->SetModelHandle(blockModels_[0]);
				else if (!np && pp)
					cube->SetModelHandle(blockModels_[(size_t)cube->GetType()]);
			}

			cube->Draw();
		}
		nowWave = false;
	}
}

void Stage::DrawUI() {
	// フォント取得
	auto f = FontManager::GetInstance().GetFontData("汎用");

	// ウインドウサイズ取得
	int x, y;
	GetWindowSize(&x, &y);

	// 回転
	if (isSpinning_) DrawFormatStringToHandle(32, y - 64, FONT_COLOR_NORMAL, f.handle, "_");

	// 歩数
	int count = stepQuota_.size() > 0 && gameStart_ ?
		stepQuota_.back() : 0;

	unsigned int color = FONT_COLOR_NORMAL;
	if (stepCount_ < count) color = FONT_COLOR_LESS_STEP;
	else if (stepCount_ > count) color = FONT_COLOR_MORE_STEP;

	DrawFormatStringToHandle(x - 48 - int(f.size * 2.5), 48, color, f.handle, "%2d/%2d", stepCount_, count);

	// 落下数
	std::string s = "";
	for (int i = (blockWidth_ - 1) - fallCount_; i > 0; i--) {
		s += "□";
	}
	for (int i = fallCount_; i > 0; i--) {
		s += "■";
	}
	DrawFormatStringToHandle(x - 48 - int(f.size * (int)s.length() * 0.5), y - 48 - f.size, FONT_COLOR_NORMAL, f.handle, s.c_str());
}

bool Stage::Release() {
	for (auto& platform : platformList_) {
		if (platform != nullptr) {
			platform->Release();
			delete platform;
		}
	}
	platformList_.clear();

	for (auto& waveList : cubeList_) {
		for (auto& subList : waveList) {
			for (auto& cube : subList) {
				if (cube != nullptr) {
					cube->Release();
					delete cube;
				}
			}
			subList.clear();
		}
		waveList.clear();
	}
	cubeList_.clear();

	for (auto& sub : cubePattern_)
		sub.clear();
	cubePattern_.clear();

	for (auto index : blockModels_)
		MV1DeleteModel(index);

	return true;
}

bool Stage::ReleaseWave() {
	if (cubeList_.size() > 0) {
		auto& waveList = cubeList_.back();
		for (auto& subList : waveList) {
			for (auto& cube : subList) {
				cube->Release();
				delete cube;
			}
			subList.clear();
		}
		waveList.clear();
		cubeList_.pop_back();
	}

	if (stepQuota_.size() > 0) {
		stepQuota_.pop_back();
	}

	stepCount_ = 0;

	return true;
}

void Stage::VanishBlock(const Vector2& trap_stage_pos, Trap::TYPE type) {
	for (auto& waveList : cubeList_) for (auto& subList : waveList) for (auto& cube : subList) {
		if (!cube->IsAlive() || cube->GetState() == Block::STATE::NONE ||
			cube->GetState() == Block::STATE::WAIT || cube->GetState() == Block::STATE::VANISH) continue;

		if (cube->GetStageIndex() == trap_stage_pos) {
			cube->ChangeState(Block::STATE::VANISH);

			if (advVanishCount_ == 0) AudioManager::GetInstance().PlaySE("キューブ消滅");

			switch (type) {
			case Trap::TYPE::NORMAL:
				trapVanish_ = true;
				break;
			case Trap::TYPE::ADVANCE:
				advVanishCount_++;
				break;
			}

			switch (cube->GetType()) {
			case Block::TYPE::NORMAL:
				break;
			case Block::TYPE::ADVANTAGE:
				gameScene_->GetTrapPtr()->SetTrap(GeometryDxLib::Vector3ToVECTOR(cube->GetPosition()), Trap::TYPE::ADVANCE);
				break;
			case Block::TYPE::FORBIDDEN:
				fallCount_ += blockWidth_;
				waveFallCount_ += blockWidth_;
				break;
			}

			StartStep();

			spinTimer_ = SPIN_DELAY_FRAME;
			return;
		}
	}
}

void Stage::StartStep() {
	if (stepCount_ == 0) stepCount_++;
}

void Stage::AddStep() {
	if (stepCount_ > 0) stepCount_++;
}

void Stage::ConvertStagePos(const VECTOR& pos, int& x, int& z) {
	x = (int)(pos.x / Block::BLOCK_SIZE);
	z = (int)(pos.z / -Block::BLOCK_SIZE);
}

VECTOR Stage::ConvertWorldPos(int x, int z) {
	VECTOR ret = {};
	ret.x = x * Block::BLOCK_SIZE;
	ret.z = z * -Block::BLOCK_SIZE;
	return ret;
}

void Stage::GetPlatformSize(int& x, int& z) const {
	x = blockWidth_;
	z = platformDepth_;
}

int Stage::GetPrevPlatformSizeZ() const { return prevPlatformDepth_; }

std::list<std::list<std::list<Block*>>> Stage::GetCubeList() const { return cubeList_; }

int Stage::GetSpinFrameIndex() {
	return spinFrameIndex_;
}

void Stage::SetSpinFrameIndex(int i) {
	spinFrameIndex_ = std::clamp(i, 0, SPIN_FRAME_MAX - 1);
}

void Stage::SetFastForward(bool b) { fastForward_ = b; }

bool Stage::IsSpinning() const { return isSpinning_; }

bool Stage::IsVanishing() const {
	bool ret = false;

	if (cubeList_.size() > 0) {
		auto& waveList = cubeList_.back();

		for (auto& subList : waveList) {
			for (auto& cube : subList) {
				if (cube->IsAlive() && cube->GetState() == Block::STATE::VANISH) {
					ret = true;
					break;
				}
			}
			if (ret) break;
		}
	}

	return ret;
}

int Stage::IsClear() const {
	return isClear_;
}

void Stage::SetUpCube() {
	gameStart_ = false;

	// nフェーズ目にクリア
	int clrNum = 4;

#ifdef _DEBUG
	clrNum = 1;
#endif

	if (phase_ == clrNum) {
		isClear_++;
		return;
	}

	phase_++;

	if (phase_ == 1) {
		extraTimer_ = EXTRA_TIMER_FIRST_PHASE;
	}
	else {
		extraTimer_ = EXTRA_TIMER_NEW_PHASE;
	}

	if (phase_ == 3) cubeDepth_++;

	unsigned int add = 0;

	for (int cp = 0; cp < wave_; ++cp) {

		auto& waveList = cubeList_.emplace_back();

		LoadPattern();
		stepQuota_.emplace_back(std::stoi(cubePattern_[0][0]));

		for (int cd = 0; cd < cubeDepth_; ++cd) {
			auto& subList = waveList.emplace_back();

			for (int cw = 0; cw < blockWidth_; ++cw) {
				int pat = std::stoi(cubePattern_[size_t(cd + 1)][cw]);

				auto& ptr = subList.emplace_back();

				ptr = new Block();
				// タイプ
				ptr->SetType((Block::TYPE)pat);
				// モデルハンドル
				// 最初は外見で判別ができないよう、統一して隠しておく
				ptr->SetModelHandle(blockModels_[0]);
				// ステージ座標
				ptr->SetStageIndex(cw, cp * cubeDepth_ + cd);
				// 実座標
				ptr->SetPosition(
					{ cw * Block::BLOCK_SIZE + Block::HALF_BLOCK_SIZE, Block::HALF_BLOCK_SIZE, (cp * cubeDepth_ + cd) * (-Block::BLOCK_SIZE) - Block::HALF_BLOCK_SIZE });
				// 状態
				ptr->ChangeState(Block::STATE::RISING, Block::RISING_FRAME + add);
			}
			add += 5u;
		}
	}
}

void Stage::LoadPattern() {
	for (auto& sub : cubePattern_)
		sub.clear();
	cubePattern_.clear();

	// 例: "4x2"
	std::string size = std::to_string(blockWidth_) + "x" + std::to_string(cubeDepth_);

	// ファイルの番号
	int n = 0;
	while (true) { // 永久ループ
		std::string num = "";

		// ファイルの番号をインクリメント
		n++;

		// 例: "01", "10"
		if (n < 10)
			// 桁が1つしかないので、先頭に"0"を足す
			num = "0" + std::to_string(n);
		else
			num = std::to_string(n);

		// 例: "Data/Pattern/4x2/01.csv"
		std::string name = "Data/Pattern/" + size + "/" + num + ".csv";

		// ファイルを確認
		bool b = Utility::CheckFileExists(name.c_str());

		// ファイルがなければループを離脱
		if (!b) break;
	}

	int rand = GetRand(--n - 1) + 1;
	std::string num = "";

	// 例: "01", "10"
	if (rand < 10)
		// 桁が1つしかないので、先頭に"0"を足す
		num = "0" + std::to_string(rand);
	else
		num = std::to_string(rand);

	// 例: "Data/Pattern/4x2/01.csv"
	std::string name = "Data/Pattern/" + size + "/" + num + ".csv";

	Utility::LoadCSV(name.c_str(), cubePattern_);
}

void Stage::StartWave() {
	// 例外スローの防止
	if (cubeList_.size() == 0) return;

	// 現在のウェーブのリスト
	auto& waveList = cubeList_.back();

	for (auto& subList : waveList) for (auto& cube : subList) {
		// モデルハンドルを、キューブに設定されているタイプに合わせたものに再設定
		cube->SetModelHandle(blockModels_[(size_t)cube->GetType()]);
	}

	// ウェーブ開始
	startwave_ = true;

	// ウェーブ終了遅延を初期化
	waveEndDelay_ = -1;
}

void Stage::StopAndFall() {
	// 現在のウェーブだけで有効なフラグ
	bool nowWave = true;

	// 逆イテレーターでforループ
	for (auto rit = cubeList_.rbegin(); rit != cubeList_.rend(); rit++) {
		for (auto& subList : (*rit)) for (auto& cube : subList) {
			// 既に消えている場合、処理の必要が無いのでスキップ
			if (!cube->IsAlive() || cube->GetState() == Block::STATE::FALL) continue;

			// 足場からはみ出している場合は落下させる
			if (cube->GetPosition().z < platformDepth_ * (-Block::BLOCK_SIZE)) {
				// 落下
				cube->ChangeState(Block::STATE::FALL);

				// フォービドゥンキューブ以外なら、落下カウントを増やす
				// ただ、潰されている間は問答無用で落下カウントを増やす（原作準拠の）仕様にするかも
				if (cube->GetType() != Block::TYPE::FORBIDDEN ||
					gameScene_->GetPlayerPtr()->GetState() == Player::STATE::STOMP) {
					fallCount_++;
					waveFallCount_++;
				}
			}
			else {
				// 上昇中は無効
				if (cube->GetState() != Block::STATE::RISING) {
					// 現在のウェーブに属するか
					if (nowWave) {
						// 回転を止める
						cube->ChangeState(Block::STATE::STOP);
					}
					else {
						// 待機状態を維持
						cube->ChangeState(Block::STATE::WAIT);
					}
				}
			}
		}
		// ここまでで、ウェーブ1つ分の処理が終了したので、フラグを折る
		nowWave = false;
	}
}

bool Stage::KeepStop() {
	// 例外スローの防止
	if (cubeList_.size() == 0) return false;

	// 現在のウェーブのみで判定
	auto& waveList = cubeList_.back();

	for (auto& subList : waveList) for (auto& cube : subList) {
		// 生存中かつ、一時停止が必要な状態でない
		if (cube->IsAlive() &&
			cube->GetState() != Block::STATE::FALL &&
			cube->GetState() != Block::STATE::VANISH) {
			// breakする間もなくそのまま返す
			return false;
		}
	}

	// 関数の終点、戻り値を返す
	return true;
}

void Stage::NextWave() {
	if (waveEndDelay_ == -1) {
		// 例外スローの防止
		if (cubeList_.size() == 0) return;

		// 現在のウェーブのみで判定
		auto& waveList = cubeList_.back();

		for (auto& subList : waveList) for (auto& cube : subList) {
			// 生存していない場合、スキップ
			if (!cube->IsAlive()) continue;

			// 生存している場合、途中で返る
			return;
		}
		// ここに処理が来た場合、ウェーブ内のキューブは全滅している
		waveEndDelay_ = WAVE_END_DELAY;
	}
	else {
		if (waveEndDelay_ > 0) waveEndDelay_--;

		// 次のウェーブに移行するフラグが折れている場合
		if (!nextwave_ && waveEndDelay_ == 0) {
			if (gameStart_) {
				// フラグを立てる
				nextwave_ = true;
			}

			// ウェーブ開始状態をリセット
			startwave_ = false;

			if (phase_ != 0 && gameStart_ && waveFallCount_ == 0) {
				// 追加タイマーを起動
				extraTimer_ += EXTRA_TIMER_PERFECT;
				PerfectProc();
			}
			else {
				// ウェーブ内落下カウントを初期化
				waveFallCount_ = 0;
			}
		}
	}
}

void Stage::PerfectProc() {
	auto& ptr = platformList_.emplace_back();
	ptr = new Block(blockWidth_);
	ptr->SetType(Block::TYPE::PLATFORM);
	ptr->SetModelHandle(blockModels_[0]);
	ptr->SetStageIndex(0, platformDepth_);
	ptr->SetPosition(
		{ Block::HALF_BLOCK_SIZE, -Block::HALF_BLOCK_SIZE, platformDepth_ * (-Block::BLOCK_SIZE) - Block::HALF_BLOCK_SIZE }
	);
	ptr->ChangeState(Block::STATE::ADD);

	if (stepCount_ <= stepQuota_.back()) {
		gameScene_->AddScore(stage_ * phase_ * 100);
		gameScene_->AddIQ(1);
	}
	else {
		gameScene_->AddScore(stage_ * phase_ * 50);
	}

	perfectCamTimer_ = PERFECT_CAM_TIMER;
	AudioManager::GetInstance().PlaySE("パーフェクト");
}

void Stage::UpdateStop() {
	// 停止・落下処理
	StopAndFall();

	// 足止め処理
	auto stopFlag = KeepStop();

	// ウェーブ移行処理
	NextWave();

	// タイマー処理
	if (fastForward_) spinTimer_ = 0;

	if (spinTimer_ > 0) spinTimer_--;
	else if (extraTimer_ > 0) extraTimer_--;

	// 最終的な判断
	if ((!stopFlag || nextwave_) && waveEndDelay_ <= 0 && extraTimer_ <= 0 && spinTimer_ <= 0 && !IsVanishing()) {
		// 次のウェーブに移行する
		if (nextwave_) {
			// ウェーブ内のデータを解放
			ReleaseWave();

			// 次回の処理に向けて、フラグは折っておく
			nextwave_ = false;
		}

		if (cubeList_.size() > 0) {
			// 次の回転を開始
			isSpinning_ = true;

			// ウェーブ開始処理
			StartWave();
		}
		else {
			// フェーズ進行処理
			SetUpCube();
		}
	}
}

void Stage::UpdateSpin() {
	if (!gameStart_) gameStart_ = true;

	// 例外スローの防止＋α
	if (cubeList_.size() == 0) {
		isSpinning_ = false;
		return;
	}

	// スピンタイマー
	++spinTimer_;

	// 歩数を追加する？
	auto step = false;

	// 現在のウェーブのリスト
	auto& waveList = cubeList_.back();

	// リストを隅々まで処理
	for (auto& subList : waveList) for (auto& cube : subList) {
		// 既に消えているか消失中の場合、処理の必要が無いのでスキップ
		if (!cube->IsAlive() ||
			cube->GetState() == Block::STATE::VANISH || cube->GetState() == Block::STATE::FALL) continue;

		// 衝突判定用（回転中は、キューブ後方の下辺の衝突判定が消える）
		cube->ChangeState(Block::STATE::SPIN);

		// キューブの回転量
		auto cubeRot = cube->GetRotation();

		// 追加される回転量（弧度ベース）
		float deg = -SPIN_DEGREE;
		// 高速状態なら、より大きく回転させる
		if (fastForward_) deg = -FAST_SPIN_DEGREE;
		// 回転量をラジアンに変換
		float nextRot = DX_PI_F / 180.f * deg;

		// 合計回転量が-90度をオーバーしないよう、追加される回転量を調整
		if (cubeRot.x + nextRot < -DX_PI_F / 2.f)
			nextRot = -DX_PI_F / 2.f - cubeRot.x;

		// 行列計算（座標への平行移動×回転×原点への平行移動）
		Matrix4x4 mat = TranslationMatrix(cube->GetMatrixPosition()) * RotationMatrixX(nextRot) * TranslationMatrix(-cube->GetMatrixPosition());

		// キューブの座標を更新
		cube->SetPosition(mat * cube->GetPosition());

		// キューブの回転を更新
		cubeRot.x += nextRot;
		cube->SetRotation(cubeRot);

		// キューブの回転量が-90度になったら回転処理を停止
		if (cubeRot.x <= -DX_PI_F / 2.f) {
			auto idx = cube->GetStageIndex();
			cube->SetStageIndex({ idx.x, idx.y + 1 });
			if (isSpinning_) {
				isSpinning_ = false;
				AudioManager::GetInstance().PlaySE("回転");
			}
			if (cube->GetType() != Block::TYPE::FORBIDDEN)
				step = true;
		}
	}

	if (step) AddStep();
}
