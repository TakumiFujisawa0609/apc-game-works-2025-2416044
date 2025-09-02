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
	keyMap_.clear();
	padMap_.clear();

	return true;
}

void InputManager::SetKeyMap(BUTTONS out, int in) {
	for (auto& map : keyMap_) {
		if (out == map.first) {
			map.second = in;
			return;
		}
	}
	keyMap_.emplace(out, in);
}

void InputManager::SetPadMap(BUTTONS out, int in) {
	for (auto& map : padMap_) {
		if (out == map.first) {
			map.second = in;
			return;
		}
	}
	padMap_.emplace(out, in);
}

bool InputManager::IsPressButton(BUTTONS b) const {
	bool now = keyInput_[static_cast<int>(b)].now || padInput_[static_cast<int>(b)].now;
	return now;
}

bool InputManager::IsDownButton(BUTTONS b) const {
	bool prev = keyInput_[static_cast<int>(b)].prev || padInput_[static_cast<int>(b)].prev;
	bool now = keyInput_[static_cast<int>(b)].now || padInput_[static_cast<int>(b)].now;
	return !prev && now;
}

bool InputManager::IsUpButton(BUTTONS b) const {
	bool prev = keyInput_[static_cast<int>(b)].prev || padInput_[static_cast<int>(b)].prev;
	bool now = keyInput_[static_cast<int>(b)].now || padInput_[static_cast<int>(b)].now;
	return prev && !now;
}

void InputManager::GetKeyInput() {
	char k[256] = {};

	GetHitKeyStateAll(k);

	for (auto& map : keyMap_) {
		auto kIn = 0U;
		if (k[map.second]) kIn = 1U;

		keyInput_[static_cast<int>(map.first)].prev = keyInput_[static_cast<int>(map.first)].now;
		keyInput_[static_cast<int>(map.first)].now = kIn;
	}
}

void InputManager::GetPadInput(int pad_num) {
	DINPUT_JOYSTATE d = {};
	XINPUT_STATE x = {};

	GetJoypadDirectInputState(pad_num, &d);
	GetJoypadXInputState(pad_num, &x);

	for (auto& map : padMap_) {
		auto dIn = 0U;
		if (d.Buttons[map.second]) dIn = 1U;

		padInput_[static_cast<int>(map.first)].prev = padInput_[static_cast<int>(map.first)].now;
		padInput_[static_cast<int>(map.first)].now = dIn;
	}
}
