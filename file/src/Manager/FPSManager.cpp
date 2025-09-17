#include <cmath>
#include <Dxlib.h>
#include <string>
#include "FPSManager.h"

FPSManager* FPSManager::instance_ = nullptr;

void FPSManager::CreateInstance(unsigned int fps) {
    // instance_ 変数が空ポインタならインスタンスを生成
    if (instance_ == nullptr) instance_ = new FPSManager(fps);
}

FPSManager& FPSManager::GetInstance() {
	return *instance_;
}

FPSManager::FPSManager(unsigned int fps) :
    TARGET_FPS(fps), TIME_PER_FRAME(static_cast<int>(MICRO_TIMER / TARGET_FPS)) {
    // 表示FPS
    showFPS_ = 0.F;
}

FPSManager::~FPSManager() {
    // リストの後片付け
    timeList_.clear();
}

void FPSManager::Draw() {
    if (timeList_.size() >= 2) { // リストの長さが2以上必要

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
    }

    // FPSを表示
    DrawFormatString(10, 10, 0xFFFFFF, "FPS: %.2f", showFPS_);
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
            WaitTimer(static_cast<int>((tookTime - TIME_PER_FRAME) * 1000.F));
        }
    }

    // 今回の時間を記録する
    RegisterTime(GetNowHiPerformanceCount());
}  

bool FPSManager::Release() {
	delete instance_;
	return true;
}

void FPSManager::RegisterTime(const long long now_time) {
    // 現在のタイマー内時間をリスト上に記録
    timeList_.push_back(now_time);

    // リスト長が最大値を超えたら、その分だけ古い記録を弾く
    while (timeList_.size() > static_cast<int>(TARGET_FPS)) timeList_.pop_front();
}
