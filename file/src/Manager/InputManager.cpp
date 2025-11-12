#include "../Common/Geometry.h"
#include "InputManager.h"

InputManager* InputManager::instance_ = nullptr;

bool InputManager::Init() {
	return true;
}

void InputManager::Update() {
	GetKeyInput();

	GetPadInput(DX_INPUT_PAD1);
}

bool InputManager::Release() {
	inputMap_.clear();

	return true;
}

void InputManager::AddInputMap(const char* name, BUTTONS pad1, BUTTONS pad2, int key1, int key2) {
	INPUT_MAP in = { { pad1, pad2 }, { key1, key2 } };
	AddInputMap(name, in);
}

void InputManager::AddInputMap(const char* name, std::array<BUTTONS, MAP_INDEX_MAX> pad_map, std::array<int, MAP_INDEX_MAX> key_map) {
	INPUT_MAP in = { pad_map, key_map };
	AddInputMap(name, in);
}

void InputManager::AddInputMap(const char* name, INPUT_MAP map) {
	if (inputMap_.find(name) != inputMap_.end()) return;

	inputMap_.emplace(name, map);
}

void InputManager::ReplaceInputMapButton(const char* name, std::array<BUTTONS, MAP_INDEX_MAX> replace) {
	auto it = inputMap_.find(name);

	if (it != inputMap_.end())
		(*it).second.padMap = replace;
}

void InputManager::ReplaceInputMapButton(const char* name, BUTTONS replace, size_t index) {
	auto it = inputMap_.find(name);

	if (it != inputMap_.end())
		(*it).second.padMap[index] = replace;
}

void InputManager::ReplaceInputMapKey(const char* name, std::array<int, MAP_INDEX_MAX> replace) {
	auto it = inputMap_.find(name);

	if (it != inputMap_.end())
		(*it).second.keyMap = replace;
}

void InputManager::ReplaceInputMapKey(const char* name, int replace, size_t index) {
	auto it = inputMap_.find(name);

	if (it != inputMap_.end())
		(*it).second.keyMap[index] = replace;
}

bool InputManager::NowMap(const char* name) const {
	auto it = inputMap_.find(name);

	if (it != inputMap_.end()) {
		auto p1 = NowButton((*it).second.padMap[0]);
		auto p2 = NowButton((*it).second.padMap[1]);
		auto k1 = NowKey((*it).second.keyMap[0]);
		auto k2 = NowKey((*it).second.keyMap[1]);
		return p1 || p2 || k1 || k2;
	}

	return false;
}

bool InputManager::PrevMap(const char* name) const {
	auto it = inputMap_.find(name);

	if (it != inputMap_.end()) {
		auto p1 = PrevButton((*it).second.padMap[0]);
		auto p2 = PrevButton((*it).second.padMap[1]);
		auto k1 = PrevKey((*it).second.keyMap[0]);
		auto k2 = PrevKey((*it).second.keyMap[1]);
		return p1 || p2 || k1 || k2;
	}

	return false;
}

bool InputManager::DownMap(const char* name) const {
	auto it = inputMap_.find(name);

	if (it != inputMap_.end()) {
		auto p1 = DownButton((*it).second.padMap[0]);
		auto p2 = DownButton((*it).second.padMap[1]);
		auto k1 = DownKey((*it).second.keyMap[0]);
		auto k2 = DownKey((*it).second.keyMap[1]);
		return p1 || p2 || k1 || k2;
	}

	return false;
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

int InputManager::NowKey(int d) const {
	return nowKey_[d];
}

int InputManager::PrevKey(int d) const {
	return prevKey_[d];
}

bool InputManager::DownKey(int d) const {
	return NowKey(d) && !PrevKey(d);
}

bool InputManager::UpKey(int d) const {
	return !NowKey(d) && PrevKey(d);
}

void InputManager::GetKeyInput() {
	prevKey_ = nowKey_;
	GetHitKeyStateAll(nowKey_.data());
}

void InputManager::GetPadInput(int pad_num) {
	// DirectInput
	DINPUT_JOYSTATE dIn = {};
	GetJoypadDirectInputState(pad_num, &dIn);

	// XInput（XBOX対応コントローラー専用）
	XINPUT_STATE xIn = {};
	bool xFlg = CheckJoypadXInput(pad_num) ? true : false;
	if (xFlg) GetJoypadXInputState(pad_num, &xIn);

	// 更新
	prevButton_ = nowButton_;
	nowButton_ = {};

	// 左スティック
	if (!xFlg) { // DirectInput
		// X軸
		if (dIn.X < 0)
			nowButton_[(int)BUTTONS::LSTICK_L] = abs(dIn.X);
		else if (dIn.X > 0)
			nowButton_[(int)BUTTONS::LSTICK_R] = abs(dIn.X);
		// Y軸
		if (dIn.Y > 0)
			nowButton_[(int)BUTTONS::LSTICK_U] = abs(dIn.Y);
		else if (dIn.Y < 0)
			nowButton_[(int)BUTTONS::LSTICK_D] = abs(dIn.Y);
	}
	else { // XInput
		// X軸
		if (xIn.ThumbLX < 0)
			nowButton_[(int)BUTTONS::LSTICK_L] = (int)abs(xIn.ThumbLX * XINPUT_STICK_MULT);
		else if (xIn.ThumbLX > 0)
			nowButton_[(int)BUTTONS::LSTICK_R] = (int)abs(xIn.ThumbLX * XINPUT_STICK_MULT);
		// Y軸
		if (xIn.ThumbLY > 0)
			nowButton_[(int)BUTTONS::LSTICK_U] = (int)abs(xIn.ThumbLY * XINPUT_STICK_MULT);
		else if (xIn.ThumbLY < 0)
			nowButton_[(int)BUTTONS::LSTICK_D] = (int)abs(xIn.ThumbLY * XINPUT_STICK_MULT);
	}

	// 右スティック
	if (!xFlg) { // DirectInput
		// DirectInputは、右スティック専用のパラメーターを持たない（RxやRyなどは本来、スティックの回転量である）ため
		// ここに記述してあるプログラムでは反応しない場合がある
		
		// X軸
		if (dIn.Rx < 0)
			nowButton_[(int)BUTTONS::RSTICK_L] = abs(dIn.Rx);
		else if (dIn.X > 0)
			nowButton_[(int)BUTTONS::RSTICK_R] = abs(dIn.Rx);
		// Y軸
		if (dIn.Ry > 0)
			nowButton_[(int)BUTTONS::RSTICK_U] = abs(dIn.Ry);
		else if (dIn.Ry < 0)
			nowButton_[(int)BUTTONS::RSTICK_D] = abs(dIn.Ry);
	}
	else { // XInput
		// X軸
		if (xIn.ThumbRX < 0)
			nowButton_[(int)BUTTONS::RSTICK_L] = (int)abs(xIn.ThumbRX * XINPUT_STICK_MULT);
		else if (xIn.ThumbRX > 0)
			nowButton_[(int)BUTTONS::RSTICK_R] = (int)abs(xIn.ThumbRX * XINPUT_STICK_MULT);
		// Y軸
		if (xIn.ThumbRY < 0)
			nowButton_[(int)BUTTONS::RSTICK_U] = (int)abs(xIn.ThumbRY * XINPUT_STICK_MULT);
		else if (xIn.ThumbRY > 0)
			nowButton_[(int)BUTTONS::RSTICK_D] = (int)abs(xIn.ThumbRY * XINPUT_STICK_MULT);
	}

	// ハットスイッチ／十字キー
	if (dIn.POV[0] != 0xFFFFFFFFU) {
		/*
		// POVは、出力に弧度×100のunsigned int型1つを用いているため
		// 取り出した値を100で割った後、弧度からラジアンに変換すると良い
		double rad = DegToRad(static_cast<double>(dIn.POV[0]) / 100.0);
		// 計算時の誤差に対するマージン
		double margin = 0.01;

		// 本来はx = cos、y = sinだが、0が上から始まるので
		// cosとsinを入れ替えた方が良い
		double sinRet = sin(rad);
		double cosRet = cos(rad);

		// X軸
		if (sinRet < margin)
			nowButton_[(int)BUTTONS::DPAD_L] = 1;
		else if (sinRet > margin)
			nowButton_[(int)BUTTONS::DPAD_R] = 1;
		// Y軸
		if (cosRet > margin)
			nowButton_[(int)BUTTONS::DPAD_U] = 1;
		else if (cosRet < margin)
			nowButton_[(int)BUTTONS::DPAD_D] = 1;
		*/

		// 上の処理が上手く行かないため、代わりの処理
		switch (dIn.POV[0] / 100u) {
		case 0u:
			nowButton_[(int)BUTTONS::DPAD_U] = 1;
			break;
		case 45u:
			nowButton_[(int)BUTTONS::DPAD_R] = 1;
			nowButton_[(int)BUTTONS::DPAD_U] = 1;
			break;
		case 90u:
			nowButton_[(int)BUTTONS::DPAD_R] = 1;
			break;
		case 135u:
			nowButton_[(int)BUTTONS::DPAD_R] = 1;
			nowButton_[(int)BUTTONS::DPAD_D] = 1;
			break;
		case 180u:
			nowButton_[(int)BUTTONS::DPAD_D] = 1;
			break;
		case 225u:
			nowButton_[(int)BUTTONS::DPAD_L] = 1;
			nowButton_[(int)BUTTONS::DPAD_D] = 1;
			break;
		case 270u:
			nowButton_[(int)BUTTONS::DPAD_L] = 1;
			break;
		case 315u:
			nowButton_[(int)BUTTONS::DPAD_L] = 1;
			nowButton_[(int)BUTTONS::DPAD_U] = 1;
			break;
		}
	}

	// Buttons（ボタン）
	if (!xFlg) { // DirectInput
		// 最大32ボタン
		for (int index = 0; index <= (int)BUTTONS::BUTTON_31 - (int)BUTTONS::BUTTON_0; index++)
			nowButton_[(int)BUTTONS::BUTTON_0 + index] = dIn.Buttons[index];
	}
	else { // XInput
		// 最大12ボタン＋ハットスイッチ
		for (int index = 0; index <= (int)BUTTONS::BUTTON_11 - (int)BUTTONS::BUTTON_0; index++)
			nowButton_[(int)BUTTONS::BUTTON_0 + index] = dIn.Buttons[index];

		// トリガーを余った領域に割り当て
		nowButton_[(int)BUTTONS::BUTTON_12] = xIn.LeftTrigger;
		nowButton_[(int)BUTTONS::BUTTON_13] = xIn.RightTrigger;
	}

	/*
	// Buttons（ボタン）
	if (!xFlg) { // DirectInput
		// 最大32ボタン
		for (int index = 0; index < _countof(DINPUT_JOYSTATE::Buttons); index++) {
			int buttonIndex = index + (int)BUTTONS::BUTTON_0;
			nowButton_[buttonIndex] = dIn.Buttons[index];
		}
	}
	else { // XInput
		int index, buttonIndex;

		// 4方向ハットスイッチ＋12ボタン、ボタンのみ取り出す
		for (index = 4; index < _countof(XINPUT_STATE::Buttons); index++) {
			buttonIndex = (index - 4) + (int)BUTTONS::BUTTON_0;
			nowButton_[buttonIndex] = dIn.Buttons[index];
		}

		// トリガーを余った領域に割り当て
		// ボタン12
		buttonIndex = (int)BUTTONS::BUTTON_12;
		nowButton_[buttonIndex] = int(xIn.LeftTrigger);
		// ボタン13
		buttonIndex = (int)BUTTONS::BUTTON_13;
		nowButton_[buttonIndex] = int(xIn.RightTrigger);
	}
	*/
}
