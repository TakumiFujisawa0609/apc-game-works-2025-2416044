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
    targetFPS_(fps), timePerFrame_(static_cast<int>(MILI_TIMER / targetFPS_)), listMax_(targetFPS_ * 2) {
    // 描画スキップフラグ
    frameSkip_ = false;

    // 表示FPS
    showFPS_ = 0.F;
}

FPSManager::~FPSManager() {
    // リストの後片付け
    timeList_.clear();
}

bool FPSManager::IsSkipDraw() {
    if (frameSkip_) {
        frameSkip_ = false;
        return true;
    }
    return false;
}

void FPSManager::Draw() {
    if (timeList_.size() >= 2) { // リストの長さが2以上必要

        // リストの逆イテレータを取得
        auto rit = timeList_.rbegin();

        // リスト末尾（直前）の時間を取得
        int nowFps = *rit;

        // 逆イテレータを1つ進める
        rit++;

        // リスト末尾から2番目（1フレーム前）の時間を取得
        int prevFps = *rit;

        // FPSを算出
        showFPS_ = timePerFrame_ * targetFPS_ / static_cast<float>(nowFps - prevFps);
    }

    // FPSを表示
    DrawFormatString(10, 10, 0xFFFFFF, "FPS: %.2f", showFPS_);
}

void FPSManager::Wait() {
    // 待ち時間
    int waitTime = 0;

    // CheckWait 関数の結果で分岐
    if (CheckWait(waitTime)) {

        // DxLib のウェイト機能を使用
        WaitTimer(waitTime);

        // 現在の時間を記録
        RegisterTime(GetNowCount());
    }
    else {
        // このフレームは最適な処理をできたものとして記録する
        RegisterTime(timeList_.back() + timePerFrame_);

        // 現在のフレームの描画処理をスキップする
        frameSkip_ = true;
    }
}  

bool FPSManager::Release() {
	delete instance_;
	return true;
}

unsigned int FPSManager::GetTargetFPS() {
	return targetFPS_;
}

bool FPSManager::CheckWait(int& time) {
    // リストの中身が無い場合は true を返す
    if (timeList_.empty()) return true;

    // 実際にかかった時間
    int actuallyTookTime = GetNowCount() - timeList_.back();

    // 待ち時間
    int waitTime = timePerFrame_ - actuallyTookTime;

    // 待ち時間で分岐
    if (waitTime >= 0) { // フレーム余り

        // 値参照を用いて、待ち時間を渡す
        time = waitTime;

        return true;
    }
    else { // フレーム欠け
        // 待ち時間の絶対値で分岐
        if (static_cast<int>(abs(waitTime)) < timePerFrame_) { // 1フレーム以内のズレ
            return false;
        }
    }
    return true;
}

void FPSManager::RegisterTime(const int now_time) {
    // 現在のタイマー内時間をリスト上に記録
    timeList_.push_back(now_time);

    // リスト長が最大値を超えたら、その分だけ古い記録を弾く
    while (timeList_.size() > listMax_) timeList_.pop_front();
}
