#pragma once
#include <chrono>
#include <list>

class FPSManager final {
public:
	// シングルトン
	static void CreateInstance(unsigned int fps = STANDARD_FPS) { if (instance_ == nullptr) instance_ = new FPSManager(fps); }
	static FPSManager& GetInstance() { return *instance_; }
	static void DeleteInstance() { if (instance_ != nullptr) delete instance_; instance_ = nullptr; }

	// 更新
	void Update(bool show_key);

	// 描画
	void Draw(int handle = -1);

	// 待機
	void CheckWait();

	// 解放
	bool Release();

	// デルタタイム取得（外部クラス向け）
	double GetDeltaTime();

private:
	// インスタンス
	static FPSManager* instance_;

	// シングルトン
	FPSManager(const FPSManager&) = delete;
	FPSManager& operator=(const FPSManager&) = delete;
	FPSManager(FPSManager&&) = delete;
	FPSManager& operator=(FPSManager&&) = delete;

	// 標準目標FPS
	static constexpr unsigned int STANDARD_FPS = 60U;
	// 最大目標FPS
	static constexpr unsigned int MAX_FPS = 1200U;

	// 目標FPS
	const unsigned int TARGET_FPS;
	// フレーム毎の理想時間
	const double IDEAL_FRAME_SECOND;

	// タイマー記録用リスト
	std::list<double> timeList_;
	// 直前フレームの時間
	std::chrono::high_resolution_clock::time_point prevTime_;

	// 表示FPS
	float showFPS_;
	// 表示FPS可視化フラグ
	bool showFlag_;

	// コンストラクタ
	FPSManager(unsigned int fps);
	// デストラクタ
	~FPSManager();

	// 時間記録処理
	void RegisterTime(const double delta_time);

};
