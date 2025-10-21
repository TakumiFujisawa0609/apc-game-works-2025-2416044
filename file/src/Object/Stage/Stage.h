#pragma once
#include <array>
#include <list>
#include <string>
#include <vector>
#include <DxLib.h>
#include "../../Common/Geometry.h"

class Block;
class GameScene;

class Stage {
public:
	static constexpr int SPIN_FRAME = 45;
	static constexpr float SPIN_DEGREE = 90.f / SPIN_FRAME;

	static constexpr float FAST_SPIN_DEGREE = 9.0f;

	static constexpr int SPIN_DELAY_FRAME = SPIN_FRAME * 2;
	static constexpr int EXTRA_DELAY_FRAME = 90;

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
	bool Release();

	void VanishBlock(const Vector2& trap_stage_pos);
	void StartStep();
	void AddStep();

	void ConvertStagePos(const VECTOR& pos, int& x, int& z);
	VECTOR ConvertWorldPos(int x, int z);

	void GetPlatformSize(int& x, int& z) const;
	int GetPrevPlatformSizeZ() const;
	std::list<std::list<std::list<Block*>>> GetCubeList() const;

	void SetFastForward(bool);

	bool IsSpinning() const;

private:
	static constexpr unsigned long long SCORE_LIST_MAX = 8ull;
	static constexpr int SCORE_LIST[SCORE_LIST_MAX] = {
		100, 300, 500, 700, 1000, 1500, 2000, 3000
	};

	static constexpr unsigned int FONT_COLOR_NORMAL = 0xffffffu;
	static constexpr unsigned int FONT_COLOR_LESS_STEP = 0x8080ffu;
	static constexpr unsigned int FONT_COLOR_MORE_STEP = 0xff8080u;

	static constexpr float PLATFORM_DEPTH_MULT = 2.7f;

	static constexpr int EXTRA_TIMER_FIRST_PHASE = 360;
	static constexpr int EXTRA_TIMER_NEW_PHASE = 180;

	// GameSceneクラスのポインタ
	GameScene* gameScene_;

	// 足場リスト
	std::list<Block*> platformList_;
	// キューブリスト
	std::list<std::list<std::list<Block*>>> cubeList_;
	// キューブパターン
	std::vector<std::vector<std::string>> cubePattern_;
	// 歩数ノルマ
	std::list<int> stepQuota_;

	// ブロック全体の幅
	int blockWidth_;
	// 足場の奥行き
	int platformDepth_;
	// 足場の奥行き（前フレーム）
	int prevPlatformDepth_;
	// キューブの奥行き
	int cubeDepth_;
	// フェーズ
	int phase_;
	// ウェーブ
	int wave_;
	// 落下カウント
	int fallCount_;
	// 歩数カウント
	int stepCount_;

	// スピンタイマー
	int spinTimer_;
	// 追加タイマー
	int extraTimer_;
	// ゲーム開始フラグ
	bool gameStart_;
	// ウェーブ開始フラグ
	bool startwave_;
	// 次のウェーブへの移行フラグ
	bool nextwave_;
	// 回転中フラグ
	bool isSpinning_;
	// 高速進行フラグ
	bool fastForward_;

	// ブロック全体のモデルハンドル配列
	std::array<int, 3ull> blockModels_;

	// キューブの準備
	void SetUpCube();
	// キューブパターン読み込み
	void LoadPattern();

	// ウェーブ開始
	void StartWave();
	// 停止と落下
	void StopAndFall();
	// 停止状態の維持
	bool KeepStop();
	// 次のウェーブに移行
	void NextWave();

	// 停止中の更新処理
	void UpdateStop();
	// 回転中の更新処理
	void UpdateSpin();

	// 現在のウェーブのみを解放
	bool ReleaseWave();

};
