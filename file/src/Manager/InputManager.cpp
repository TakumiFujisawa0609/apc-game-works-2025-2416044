#include "../Common/Geometry.h"
#include "InputManager.h"

InputManager* InputManager::instance_ = nullptr;

void InputManager::CreateInstance() {
	// instance_ 変数が空ポインタならインスタンスを生成
	if (instance_ == nullptr) instance_ = new InputManager;
}

InputManager& InputManager::GetInstance() {
	return *instance_;
}

bool InputManager::Init() {
	return true;
}

void InputManager::Update() {
	GetKeyInput();

	GetPadInput(DX_INPUT_PAD1);
}

bool InputManager::Release() {
	buttonMap_.clear();

	return true;
}

void InputManager::AddButtonMap(const char* name, BUTTONS padMap, std::array<int, 2> keyMap) {
	buttonMap_.emplace(name, BUTTON_MAP(padMap, keyMap));
}

void InputManager::ReplacePadMap(const char* name, BUTTONS replace) {
	for (auto& pad : buttonMap_) if (name == pad.first) {
		pad.second.padMap = replace;
		return;
	}
}

void InputManager::ReplaceKeyMap(const char* name, std::array<int, 2> replace) {
	for (auto& pad : buttonMap_) if (name == pad.first) {
		pad.second.keyMap = replace;
		return;
	}
}

void InputManager::ReplaceKeyMap(const char* name, int replace, size_t index) {
	for (auto& pad : buttonMap_) if (name == pad.first) {
		pad.second.keyMap[index] = replace;
		return;
	}
}

int InputManager::NowButton(BUTTONS b) const {
	return nowButton_[(int)b];
}

int InputManager::PrevButton(BUTTONS b) const {
	return prevButton_[(int)b];
}

bool InputManager::DownButton(BUTTONS b) const {
	return NowButton(b) && !PrevButton(b);
}

bool InputManager::UpButton(BUTTONS b) const {
	return !NowButton(b) && PrevButton(b);
}

int InputManager::NowKey(int d) {
	return nowKey_[d];
}

int InputManager::PrevKey(int d) {
	return prevKey_[d];
}

bool InputManager::DownKey(int d) {
	return NowKey(d) && !PrevKey(d);
}

bool InputManager::UpKey(int d) {
	return !NowKey(d) && PrevKey(d);
}

void InputManager::GetKeyInput() {
	prevKey_ = nowKey_;
	GetHitKeyStateAll(nowKey_.data());
}

void InputManager::GetPadInput(int pad_num) {
	int n = GetJoypadButtonNum(pad_num);

	DINPUT_JOYSTATE dIn = {};
	GetJoypadDirectInputState(pad_num, &dIn);

	bool xFlg = CheckJoypadXInput(pad_num) ? true : false;
	XINPUT_STATE xIn = {};
	if (xFlg) GetJoypadXInputState(pad_num, &xIn);

	prevButton_ = nowButton_;
	nowButton_ = {};

	// X/Y（左スティック）
	if (dIn.X < 0)
		nowButton_[(int)BUTTONS::LSTICK_L] = abs(dIn.X);
	else if (dIn.X > 0)
		nowButton_[(int)BUTTONS::LSTICK_R] = abs(dIn.X);

	if (dIn.Y > 0)
		nowButton_[(int)BUTTONS::LSTICK_U] = abs(dIn.Y);
	else if (dIn.Y < 0)
		nowButton_[(int)BUTTONS::LSTICK_D] = abs(dIn.Y);

	// POV（ハットスイッチ／十字キー）
	// 本来はx = cos、y = sinだが、0が上から始まるので
	// cosとsinを入れ替えた方が良い
	if (dIn.POV[0] != 0xFFFFFFFFU) {
		double rad = DegToRad(dIn.POV[0] / 100.0);
		double margin = 0.01;
		
		if (sin(rad) < margin)
			nowButton_[(int)BUTTONS::DPAD_L] = (int)(sin(rad) * 100);
		else if (sin(rad) > margin)
			nowButton_[(int)BUTTONS::DPAD_R] = (int)(sin(rad) * 100);

		if (cos(rad) > margin)
			nowButton_[(int)BUTTONS::DPAD_U] = (int)(cos(rad) * 100);
		else if (cos(rad) < margin)
			nowButton_[(int)BUTTONS::DPAD_D] = (int)(cos(rad) * 100);
	}

	// Buttons（ボタン）
	for (int index = 0; index < (int)BUTTONS::END - (int)BUTTONS::BUTTON_0; index++)
		nowButton_[(int)BUTTONS::BUTTON_0 + index] = dIn.Buttons[index];

	int debug = 0;
}
