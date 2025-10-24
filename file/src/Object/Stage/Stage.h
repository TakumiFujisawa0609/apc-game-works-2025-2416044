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
	static constexpr int SPIN_FRAME = 36;
	static constexpr float SPIN_DEGREE = 90.0f / SPIN_FRAME;

	static constexpr float FAST_SPIN_DEGREE = 9.0f;

	static constexpr int SPIN_DELAY_FRAME = 30 * 2 + SPIN_FRAME;

	static constexpr int CUBE_WIDTH = 4;
	static constexpr int CUBE_DEPTH = 3;
	static constexpr float CUBE_FALL_RANGE_MAX = -1200;

	static constexpr int PLATFORM_WIDTH = CUBE_WIDTH;
	static constexpr int PLATFORM_HEIGHT = 3;
	static constexpr int PLATFORM_DEPTH = CUBE_DEPTH * 4;

	static constexpr float STOMP_DEGREE_THRESHOLD = -70.0f;

	static constexpr int CUBE_PATTERN_MAX = 5;

	Stage(GameScene*);
	~Stage() {}

	bool SystemInit();
	bool GameInit();
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
	int GetPrevPlatformSizeZ() const;
	std::list<std::list<std::list<Block*>>> GetCubeList() const;

	void SetFastForward(bool);

	bool IsSpinning() const;
	bool IsVanishing() const;

private:
	/// 定数
	static constexpr int SCORE_LIST[] = { // 消去時のスコア
		2, 3, 5, 7, 10, 15, 20, 30, 50, 75, 100
	};
	static constexpr unsigned int SCORE_LIST_MAX = _countof(SCORE_LIST);

	static constexpr unsigned int FONT_COLOR_NORMAL = 0xffffffu;	// 色：白
	static constexpr unsigned int FONT_COLOR_LESS_STEP = 0x8080ffu;	// 色：薄青
	static constexpr unsigned int FONT_COLOR_MORE_STEP = 0xff8080u;	// 色：薄赤

	static constexpr float PLATFORM_DEPTH_MULT = 2.7f;	// 初期配置のキューブの奥行きに対する、足場の奥行きの倍率

	static constexpr int WAVE_END_DELAY = 45;			// ウェーブ終了時遅延
	static constexpr int EXTRA_TIMER_FIRST_PHASE = 360;	// フェーズ1の開始前の追加タイマー
	static constexpr int EXTRA_TIMER_NEW_PHASE = 240;	// フェーズ2以降の開始前の追加タイマー
	static constexpr int EXTRA_TIMER_PERFECT = 120;		// パーフェクト時の追加タイマー
	static constexpr int PERFECT_CAM_TIMER = 75;		// カメラ用パーフェクト演出時間

	/// 変数
	// 外部クラスポインタ
	GameScene* gameScene_; // GameSceneクラスのポインタ

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

	/// 関数
	void SetUpCube();	// キューブの準備
	void LoadPattern();	// キューブパターン読み込み

	void StartWave();	// ウェーブ開始
	void StopAndFall();	// 停止と落下
	bool KeepStop();	// 停止状態の維持
	void NextWave();	// 次のウェーブに移行
	void PerfectProc();	// パーフェクト時処理

	void UpdateStop();	// 停止中の更新処理
	void UpdateSpin();	// 回転中の更新処理

	bool ReleaseWave();	// 現在のウェーブのみを解放

};
