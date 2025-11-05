#include <cmath>
#include <Dxlib.h>
#include <string>
#include "FPSManager.h"

FPSManager* FPSManager::instance_ = nullptr;

FPSManager::FPSManager(unsigned int fps) :
    TARGET_FPS(fps), TIME_PER_FRAME(static_cast<int>(MICRO_TIMER / TARGET_FPS)) {
    // 表示FPS
    showFPS_ = 0.0F;
    showFlag_ = showKey_ = false;
}

FPSManager::~FPSManager() {}

void FPSManager::Draw(bool show_key, int handle) {
    if (registerCount_ >= TARGET_FPS / SHOW_FPS_SAMPLING_NUM_PER_SECOND) { // 記録カウンターがサンプリング間隔に達する
        // リストの逆イテレータを取得
        auto rit = timeList_.rbegin();

        // リスト末尾（直前）の時間を取得
        long long nowFps = *rit;

        // 逆イテレータを1つ進める
        rit++;

        // リスト末尾から2番目（1フレーム前）の時間を取得
        long long prevFps = *rit;

        // FPSを算出
        showFPS_ = TIME_PER_FRAME * TARGET_FPS / static_cast<float>(nowFps - prevFps);

        // カウンタをリセット
        registerCount_ -= TARGET_FPS / SHOW_FPS_SAMPLING_NUM_PER_SECOND;
    }

    // FPS表示フラグの管理
    if (!showKey_ && show_key) {
        showFlag_ = !(showFlag_);
    }
    showKey_ = show_key;

    // FPSを表示
    if (showFlag_) {
        if (handle == -1) {
            DrawFormatString(10, 10, 0xFFFFFFU, "FPS: %.2f", showFPS_);
        }
        else {
            DrawFormatStringToHandle(10, 10, 0xFFFFFFU, handle, "FPS: %.2f", showFPS_);
        }
    }
}

void FPSManager::CheckWait() {
	// リストが空の場合は大きく飛ばす
    if (!timeList_.empty()) {
        // リストの末尾（直前）の時間
        long long prevTime = timeList_.back();

        // 今回の時間
        long long nowTime = GetNowHiPerformanceCount();

        // 実際にかかった時間
        long long tookTime = nowTime - prevTime;

        // 実際にかかった時間が、本来の経過時間より少ない場合
        if (tookTime < TIME_PER_FRAME) {
            // ウェイト
            WaitTimer(static_cast<int>((TIME_PER_FRAME - tookTime) / 1000.0));
        }
    }

    // 今回の時間を記録する
    RegisterTime(GetNowHiPerformanceCount());
}  

bool FPSManager::Release() {
    // リストの後片付け
    timeList_.clear();

	return true;
}

void FPSManager::RegisterTime(const LONGLONG now_time) {
    // 現在のタイマー内時間をリスト上に記録
    timeList_.push_back(now_time);

    registerCount_++;

    // リスト長が最大値を超えたら、その分だけ古い記録を弾く
    while (timeList_.size() > static_cast<int>(TARGET_FPS)) timeList_.pop_front();
}
