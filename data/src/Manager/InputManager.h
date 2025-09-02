#pragma once
#include <map>
#include "DxLib.h"

class InputManager {
public:
	enum class BUTTONS {
		DPAD_UP,
		DPAD_DOWN,
		DPAD_LEFT,
		DPAD_RIGHT,
		LSTICK_UP,
		LSTICK_DOWN,
		LSTICK_LEFT,
		LSTICK_RIGHT,
		RSTICK_UP,
		RSTICK_DOWN,
		RSTICK_LEFT,
		RSTICK_RIGHT,
		A,
		B,
		X,
		Y,
		START,
		SELECT,
		L1,
		R1,
		L2,
		R2,
		L3,
		R3,

		END
	};

	struct BUTTON_LOG {
		unsigned int now;
		unsigned int prev;
	};

	static void CreateInstance();
	static InputManager& GetInstance();

	bool Init();
	void Update();
	bool Release();

	void SetKeyMap(BUTTONS out, int in);
	void SetPadMap(BUTTONS out, int in);

	bool IsPressButton(BUTTONS) const;
	bool IsDownButton(BUTTONS) const;
	bool IsUpButton(BUTTONS) const;

private:
	static InputManager* instance_;

	std::map<BUTTONS, int> keyMap_;
	std::map<BUTTONS, int> padMap_;

	BUTTON_LOG keyInput_[static_cast<int>(BUTTONS::END)];
	BUTTON_LOG padInput_[static_cast<int>(BUTTONS::END)];

	InputManager() {}
	~InputManager() {}

	void GetKeyInput();
	void GetPadInput(int pad_num);

};
