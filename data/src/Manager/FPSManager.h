#pragma once
#include <list>

class FPSManager final {
public:
	/* 関数 */
	static void CreateInstance(unsigned int fps = BASE_TARGET_FPS);
	static FPSManager& GetInstance();

	// 描画スキップ処理
	bool IsSkipDraw();

	// 描画処理
	void Draw();

	// 待機処理
	void Wait();

	// 解放処理
	bool Release();

	// 目標FPS ゲッター
	unsigned int GetTargetFPS();

private:
	/* 定数 */
	// 基本目標FPS
	static constexpr unsigned int BASE_TARGET_FPS = 60U;

	// タイマー時間単位
	static constexpr double MILI_TIMER = 1000.0;

	// 目標FPS
	const unsigned int targetFPS_;

	// フレーム毎のミリ秒数
	const int timePerFrame_;

	// リストの長さの最大値
	const unsigned int listMax_;

	/* 変数 */
	// インスタンス
	static FPSManager* instance_;

	// タイマー記録用リスト
	std::list<int> timeList_;

	// 描画スキップフラグ
	bool frameSkip_;

	// 表示FPS
	float showFPS_;

	/* 関数 */
	// コンストラクタ
	FPSManager(unsigned int fps);

	// デストラクタ
	~FPSManager();

	// 待機確認処理
	bool CheckWait(int& time);

	// 時間記録処理
	void RegisterTime(const int now_time);

};
