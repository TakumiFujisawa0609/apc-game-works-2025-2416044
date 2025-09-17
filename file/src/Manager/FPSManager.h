#pragma once
#include <list>

class FPSManager final {
public:
	/* 関数 */
	static void CreateInstance(unsigned int fps = NORMAL_TARGET_FPS);
	static FPSManager& GetInstance();

	// 描画処理
	void Draw();

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

	/* 関数 */
	// コンストラクタ
	FPSManager(unsigned int fps);

	// デストラクタ
	~FPSManager();

	// 時間記録処理
	void RegisterTime(const long long now_time);

};
