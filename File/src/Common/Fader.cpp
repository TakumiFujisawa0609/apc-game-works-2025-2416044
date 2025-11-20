#include <DxLib.h>
#include "Fader.h"

Fader::Fader() :
	mode_(FADE_MODE::NONE),
	proc_(PROC::NONE),
	delay_(0U),
	time_(0U),
	wait_(0U),
	delayTimer_(0U),
	waitTimer_(0U),
	isFadeEnd_(true),
	color_(0x0U),
	alpha_(0.0F) {}

Fader::~Fader() {}

void Fader::SetFadeMode(FADE_MODE mode, unsigned int time, unsigned int delay, unsigned int wait) {
	mode_ = mode;
	time_ = time;
	delay_ = delay;
	wait_ = wait;

	delayTimer_ = 0U;
	waitTimer_ = 0U;

	if (mode_ != FADE_MODE::NONE) {
		isFadeEnd_ = false;
	}
}

void Fader::Update() {
	if (isFadeEnd_) return;

	switch (mode_) {
	default:
	case FADE_MODE::NONE:
		return;
	case FADE_MODE::FADE_OUT:
	case FADE_MODE::FADE_IN:
		// 遅延中
		if (delayTimer_ < delay_) {
			++delayTimer_;
			proc_ = PROC::DELAY;
			return;
		}

		// フェード中
		if (mode_ == FADE_MODE::FADE_OUT) {
			if (alpha_ < MAX_ALPHA) {
				alpha_ += MAX_ALPHA / time_;

				if (alpha_ > MAX_ALPHA)
					alpha_ = MAX_ALPHA;


				proc_ = PROC::FADE;
				return;
			}
		}
		else {
			if (alpha_ > 0.0F) {
				alpha_ -= MAX_ALPHA / time_;

				if (alpha_ < 0.0F)
					alpha_ = 0.0F;

				proc_ = PROC::FADE;
				return;
			}
		}

		// 待機中
		if (waitTimer_ < wait_) {
			++waitTimer_;
			proc_ = PROC::WAIT;
			return;
		}

		// 処理完了
		isFadeEnd_ = true;
		break;
	}
}

void Fader::Draw() {
	switch (mode_) {
	default:
	case FADE_MODE::NONE:
		return;
	case FADE_MODE::FADE_OUT:
	case FADE_MODE::FADE_IN:
		// 描画ブレンドモードを「αブレンド」に設定
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);

		// ウインドウサイズを取得
		int sw, sh;
		GetWindowSize(&sw, &sh);

		// ウインドウ全体を単色で描画
		DrawBox(0, 0, sw, sh, color_, true);

		// 描画ブレンドモードを解除
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		break;
	}
}

Fader::FADE_MODE Fader::GetFadeMode() const { return mode_; }

Fader::PROC Fader::GetNowProc() const {
	// 処理が完了している場合は、直前の処理も無しで返す
	if (mode_ == FADE_MODE::NONE) return PROC::NONE;

	return proc_;
}

bool Fader::IsFadeEnd() const { return isFadeEnd_; }
