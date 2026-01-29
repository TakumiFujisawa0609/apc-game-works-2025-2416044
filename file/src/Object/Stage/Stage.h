#pragma once
#include <array>
#include <list>
#include <string>
#include <vector>
#include <DxLib.h>
#include "../../Common/Geometry.h"
#include "../Trap/Trap.h"

class Block;
class GameScene;

class Stage {
public:
	static constexpr int SPIN_FRAME_MAX = 3;
	static constexpr int SPIN_FRAME[SPIN_FRAME_MAX] = { 36, 30, 24 };
	const float SPIN_DEGREE;
	static constexpr float FAST_SPIN_DEGREE = 9.0f;

	const int SPIN_DELAY_FRAME;

	static constexpr int CUBE_WIDTH = 4;
	static constexpr int CUBE_DEPTH = 3;
	static constexpr float CUBE_FALL_RANGE_MAX = -1200;

	static constexpr int PLATFORM_WIDTH = CUBE_WIDTH;
	static constexpr int PLATFORM_HEIGHT = 3;
	static constexpr int PLATFORM_DEPTH = CUBE_DEPTH * 4;

	static constexpr float STOMP_DEGREE_THRESHOLD = -70.0f;

	static constexpr int CLEAR_WAIT_TIMER = 120;			// クリア演出前の待機時間
	static constexpr int CLEAR_PLATFORM_MOVE_TIMER = 20;	// 
	static constexpr int CLEAR_AFTER_WAIT_TIMER = 240;		// クリア演出後の待機時間

	static constexpr unsigned int FONT_COLOR_NORMAL = 0xffffffu;	// 色：白
	static constexpr unsigned int FONT_COLOR_LESS_STEP = 0x8080ffu;	// 色：青
	static constexpr unsigned int FONT_COLOR_MORE_STEP = 0xff8080u;	// 色：赤

	static constexpr int PHASE_MAX = 4;

	Stage(GameScene*);
	~Stage() {}

	bool SystemInit();
	bool GameInit(unsigned int num = 0);
	void Update();
	void Draw();
	void DrawUI();
	bool Release();

	void VanishBlock(const Vector2& trap_stage_pos, Trap::TYPE type);
	void StartStep();
	void AddStep();

	void ConvertStagePos(const VECTOR& pos, int& x, int& z);
	VECTOR ConvertWorldPos(int x, int z);

	void GetPlatformSize(int& x, int& z) const;
	VECTOR GetPlatformSize() const;
	int GetPrevPlatformSizeZ() const;
	std::list<std::list<std::list<Block*>>> GetCubeList() const;
	int GetPhase() const;

	static int GetSpinFrameIndex();
	static void SetSpinFrameIndex(int);
	int GetExtraTimer() const;

	void SetFastForward(bool);

	bool IsGameStart() const;
	bool IsSpinning() const;
	bool IsVanishing() const;
	int IsClear() const;
	int IsEnd() const;
	bool IsMissed() const;
	bool IsExistNextWave() const;

private:
	/// 定数
	static constexpr int SCORE_NORMAL = 1;
	static constexpr int SCORE_ADVANTAGE = 2;
	static constexpr int SCORE_PERFECT = 50;
	static constexpr int SCORE_GREAT = 20;
	static constexpr int SCORE_EXCELLENT = 100;

	static constexpr int SCORE_LIST[] = { // 消去時のスコア
		2, 3, 5, 7, 10, 15, 20, 30, 50, 75, 100
	};
	static constexpr unsigned int SCORE_LIST_MAX = _countof(SCORE_LIST);

	static constexpr int WAVE_END_DELAY = 45;			// ウェーブ終了時遅延
	static constexpr int EXTRA_TIMER_FIRST_PHASE = 360;	// フェーズ1の開始前の追加タイマー
	static constexpr int EXTRA_TIMER_NEW_PHASE = 240;	// フェーズ2以降の開始前の追加タイマー
	static constexpr int EXTRA_TIMER_PERFECT = 120;		// パーフェクト時の追加タイマー
	static constexpr int PERFECT_CAM_TIMER = 75;		// カメラ用パーフェクト演出時間

	static constexpr int CUBE_DEPTH_PRESETS[][PHASE_MAX] = {
		{ 2, 2, 3, 4 },
		{ 5, 5, 6, 6 },
		{ 4, 5, 6, 6 }
	};

	static constexpr int CUBE_WAVE_PRESETS[][PHASE_MAX] = {
		{ 3, 3, 3, 3 },
		{ 3, 3, 3, 3 },
		{ 3, 3, 3, 3 }
	};

	static constexpr int CUBE_WIDTH_PRESETS[] = {
		4, 4, 5
	};

	static constexpr int INIT_PLATFORM_DEPTHS[] = {
		6 + 10, 15 + 12, 12 + 16
	};

	/// 変数
	// 外部クラスポインタ
	GameScene* gameScene_; // GameSceneクラスのポインタ

	static int spinFrameIndex_;

	// ゲーム
	std::array<int, 3ull> blockModels_;	// ブロック全体のモデルハンドル配列
	std::list<Block*> platformList_;	// 足場リスト
	int blockWidth_;					// ブロック全体の幅
	int platformDepth_;					// 足場の奥行き
	int prevPlatformDepth_;				// 足場の奥行き（前フレーム）
	int cubeDepth_;						// キューブの奥行き
	int stage_;							// ステージ数
	int phase_;							// フェーズ数
	int wave_;							// ウェーブ数
	int fallCount_;						// 落下カウント

	// フェーズ
	std::vector<std::vector<std::string>> cubePattern_;	// キューブパターン
	std::list<std::list<std::list<Block*>>> cubeList_;	// キューブリスト
	bool gameStart_;									// ゲーム開始フラグ

	// ウェーブ
	bool startwave_;			// ウェーブ開始フラグ
	bool nextwave_;				// 次のウェーブへの移行フラグ
	std::list<int> stepQuota_;	// 歩数ノルマ
	int stepQuota2_;
	int stepCount_;				// 歩数カウント
	int waveFallCount_;			// ウェーブ内での落下カウント

	// フレーム
	bool trapVanish_;		// 通常ワナでの消去フラグ
	int advVanishCount_;	// スーパーワナでの消去カウント

	// その他
	int spinTimer_;			// スピンタイマー
	int waveEndDelay_;		// ウェーブ終了遅延タイマー
	int extraTimer_;		// 追加タイマー
	int perfectCamTimer_;	// カメラ用パーフェクトタイマー
	bool isSpinning_;		// 回転中フラグ
	bool fastForward_;		// 高速進行フラグ
	int isClear_;			//
	int isClearEnd_;		//

	bool missed_;
	std::list<std::string> cubePresets_;

	/// 関数
	void SetUpCube2();			// キューブの準備
	void LoadPattern2(int num);	// キューブパターン読み込み
	void SetCubeList();			//
	void MissProc();			//

	void UpdateStop();	// 停止中の更新処理
	void UpdateSpin();	// 回転中の更新処理

	void StartWave();	// ウェーブ開始
	void StopAndFall();	// 停止と落下
	bool KeepStop();	// 停止状態の維持
	void NextWave();	// 次のウェーブに移行
	void PerfectProc();	// パーフェクト時処理
	void ClearProc();	// クリア時処理

	bool ReleaseWave();	// 現在のウェーブのみを解放

};
