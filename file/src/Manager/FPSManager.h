#pragma once
#include <list>

class FPSManager final {
public:
	/* 関数 */
	static void CreateInstance(unsigned int fps = NORMAL_TARGET_FPS) { if (instance_ == nullptr) instance_ = new FPSManager(fps); }
	static FPSManager& GetInstance() { return *instance_; }
	static void DeleteInstance() { if (instance_ != nullptr) delete instance_; instance_ = nullptr; }

	// 描画処理
	void Draw(bool show_key, int handle = -1);

	// 待機処理
	void CheckWait();

	// 解放処理
	bool Release();

private:
	/* 定数 */
	// 基本目標FPS
	static constexpr unsigned int NORMAL_TARGET_FPS = 60U;

	// マイクロ秒タイマーの時間単位
	static constexpr double MICRO_TIMER = 1000.0 * 1000.0;

	// 表示FPS用秒間サンプリング回数
	const unsigned int SHOW_FPS_SAMPLING_NUM_PER_SECOND = 30U;

	// 目標FPS
	const unsigned int TARGET_FPS;

	// フレーム毎の時間単位
	const long long TIME_PER_FRAME;

	/* 変数 */
	// インスタンス
	static FPSManager* instance_;

	// タイマー記録用リスト
	std::list<long long> timeList_;

	// 表示FPS
	float showFPS_;

	// 記録回数カウンタ
	unsigned int registerCount_;

	// FPS表示フラグ
	bool showFlag_;

	// 表示キーの前フレームの入力状態
	bool showKey_;

	/* 関数 */
	// コンストラクタ
	FPSManager(unsigned int fps);

	// デストラクタ
	~FPSManager();

	// 時間記録処理
	void RegisterTime(const LONGLONG now_time);

};
