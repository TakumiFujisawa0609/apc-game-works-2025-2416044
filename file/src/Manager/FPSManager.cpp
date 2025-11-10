#include <cmath>
#include <Dxlib.h>
#include <string>
#include "FPSManager.h"

FPSManager* FPSManager::instance_ = nullptr;

FPSManager::FPSManager(unsigned int fps) :
    TARGET_FPS(fps),
    IDEAL_FRAME_SECOND(1.0 / TARGET_FPS),
    timeList_(),
    prevTime_(),
    showFPS_(0.0F),
    showFlag_(false) {
    // 垂直同期を無効化
    SetWaitVSyncFlag(false);
}

FPSManager::~FPSManager() {}

void FPSManager::Update(bool show_key) {
    if (show_key) showFlag_ = !(showFlag_);
}

void FPSManager::Draw(int handle) {
    if (showFlag_)
        DrawFormatStringToHandle(10, 10, 0xFFFFFFU, handle, "FPS: %.2f", showFPS_);
}

void FPSManager::CheckWait() {
    // 現在時間
    auto nowTime = std::chrono::high_resolution_clock::now();

    // 前回からの経過時間
    std::chrono::duration<double> delta = nowTime - prevTime_;

    // 経過時間(秒)
    double deltaTime = delta.count();

    // 経過時間が理想時間よりも短ければ待機
    if (deltaTime < IDEAL_FRAME_SECOND)
    {

        // 待つべき時間(ミリ秒)
        double waitMiliSecond = (IDEAL_FRAME_SECOND - deltaTime) * 1000.0;

        // Sleepで待ち時間分を待機
        if (waitMiliSecond >= 1.0)
        {
            // 指定ミリ秒数待つ(DxLib関数)
            WaitTimer(static_cast<int>(waitMiliSecond));

        }

        // 指定時間になるまでbusyになるが待つ
        while (deltaTime < IDEAL_FRAME_SECOND)
        {
            // 再計測
            nowTime = std::chrono::high_resolution_clock::now();
            delta = nowTime - prevTime_;
            deltaTime = delta.count();
        }

    }

    // 前回時間を更新
    prevTime_ = nowTime;

    // FPS計測(指定された最新フレーム数分の平均）
    RegisterTime(deltaTime);

    // 合計時間
    double total = 0.0;
    for (double time : timeList_) total += time;

    // 平均FPS
    showFPS_ = static_cast<float>(timeList_.size() / total);
}  

bool FPSManager::Release() {
    timeList_.clear();

    return true;
}

double FPSManager::GetDeltaTime() { return IDEAL_FRAME_SECOND; }

void FPSManager::RegisterTime(const double delta_time) {
    timeList_.emplace_back(delta_time);

    while (timeList_.size() > TARGET_FPS)
        timeList_.erase(timeList_.begin());
}
