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

	int num = GetJoypadNum();
	for (int i = DX_INPUT_PAD1; i <= num || i <= DX_INPUT_PAD4; i++) {
		GetPadInput(i);
	}
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

int InputManager::NowKey(int DxLib_KEYcode) {
	return nowKey_[DxLib_KEYcode];
}

int InputManager::PrevKey(int DxLib_KEYcode) {
	return prevKey_[DxLib_KEYcode];
}

void InputManager::GetKeyInput() {
	prevKey_ = nowKey_;
	GetHitKeyStateAll(nowKey_.data());
}

void InputManager::GetPadInput(int pad_num) {
	int n = GetJoypadButtonNum(pad_num);

	int t = GetJoypadType(pad_num);

	DINPUT_JOYSTATE dIn = {};
	GetJoypadDirectInputState(pad_num, &dIn);

	bool xFlg = CheckJoypadXInput(pad_num) ? true : false;
	XINPUT_STATE xIn = {};
	if (xFlg) GetJoypadXInputState(pad_num, &xIn);

	prevButton_ = nowButton_;
	nowButton_ = {};

	// X/Y（左スティック）
	if (dIn.X < 0.0F)
		nowButton_[(int)BUTTONS::LSTICK_L] = abs(dIn.X);
	else if (dIn.X > 0.0F)
		nowButton_[(int)BUTTONS::LSTICK_R] = abs(dIn.X);

	if (dIn.Y > 0.0F)
		nowButton_[(int)BUTTONS::LSTICK_U] = abs(dIn.Y);
	else if (dIn.Y < 0.0F)
		nowButton_[(int)BUTTONS::LSTICK_D] = abs(dIn.Y);

	// POV（ハットスイッチ／十字キー）
	// 本来はx = cos、y = sinだが、0が上から始まるので
	// cosとsinを入れ替えた方が良い
	float rad = DegToRad(dIn.POV[0] / 100.0F);

	if (sinf(rad) < 0.0F)
		nowButton_[(int)BUTTONS::DPAD_L] = 1;
	else if (sinf(rad) > 0.0F)
		nowButton_[(int)BUTTONS::DPAD_R] = 1;

	if (cosf(rad) > 0.0F)
		nowButton_[(int)BUTTONS::DPAD_U] = 1;
	else if (cosf(rad) < 0.0F)
		nowButton_[(int)BUTTONS::DPAD_D] = 1;

	// Buttons（ボタン）
	for (int index = 0; index < (int)BUTTONS::END - (int)BUTTONS::BUTTON_0; index++)
		nowButton_[(int)BUTTONS::BUTTON_0 + index] = dIn.Buttons[index];
}
