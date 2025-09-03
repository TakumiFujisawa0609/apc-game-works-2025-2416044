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

unsigned int InputManager::GetNowButtonState(BUTTONS) const {
	return 0;
}

unsigned int InputManager::GetPrevButtonState(BUTTONS) const {
	return 0;
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
		keyInput_[static_cast<int>(map.first)].prev = keyInput_[static_cast<int>(map.first)].now;
		keyInput_[static_cast<int>(map.first)].now = k[map.second];
	}
}

void InputManager::GetPadInput(int pad_num) {
	DINPUT_JOYSTATE d = {};
	XINPUT_STATE x = {};

	GetJoypadDirectInputState(pad_num, &d);
	GetJoypadXInputState(pad_num, &x);

	for (auto idx = static_cast<int>(BUTTONS::DPAD_UP); idx <= static_cast<int>(BUTTONS::DPAD_RIGHT); idx++) {
		padInput_[idx].prev = padInput_[idx].now;
	}

	padInput_[static_cast<int>(BUTTONS::DPAD_UP)].now = 0U;
	padInput_[static_cast<int>(BUTTONS::DPAD_DOWN)].now = 0U;
	padInput_[static_cast<int>(BUTTONS::DPAD_LEFT)].now = 0U;
	padInput_[static_cast<int>(BUTTONS::DPAD_RIGHT)].now = 0U;

	switch (d.POV[0]) {
	case 4500U * 0U:
		padInput_[static_cast<int>(BUTTONS::DPAD_UP)].now = 1U;
		break;
	case 4500U * 1U:
		padInput_[static_cast<int>(BUTTONS::DPAD_UP)].now = 1U;
		padInput_[static_cast<int>(BUTTONS::DPAD_RIGHT)].now = 1U;
		break;
	case 4500U * 2U:
		padInput_[static_cast<int>(BUTTONS::DPAD_RIGHT)].now = 1U;
		break;
	case 4500U * 3U:
		padInput_[static_cast<int>(BUTTONS::DPAD_DOWN)].now = 1U;
		padInput_[static_cast<int>(BUTTONS::DPAD_RIGHT)].now = 1U;
		break;
	case 4500U * 4U:
		padInput_[static_cast<int>(BUTTONS::DPAD_DOWN)].now = 1U;
		break;
	case 4500U * 5U:
		padInput_[static_cast<int>(BUTTONS::DPAD_DOWN)].now = 1U;
		padInput_[static_cast<int>(BUTTONS::DPAD_LEFT)].now = 1U;
		break;
	case 4500U * 6U:
		padInput_[static_cast<int>(BUTTONS::DPAD_LEFT)].now = 1U;
		break;
	case 4500U * 7U:
		padInput_[static_cast<int>(BUTTONS::DPAD_UP)].now = 1U;
		padInput_[static_cast<int>(BUTTONS::DPAD_LEFT)].now = 1U;
		break;
	default:
	}

	for (auto& map : padMap_) {
		padInput_[static_cast<int>(map.first)].prev = padInput_[static_cast<int>(map.first)].now;
		padInput_[static_cast<int>(map.first)].now = d.Buttons[map.second];
	}
}
