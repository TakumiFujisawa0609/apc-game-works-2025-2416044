#pragma once
#include <map>
#include "DxLib.h"

class InputManager {
public:
	enum class PAD_INPUT {
		DINPUT,
		XINPUT,
	};

	enum class BUTTONS {
		LSTICK_UP = 0,
		LSTICK_DOWN,
		LSTICK_LEFT,
		LSTICK_RIGHT,
		RSTICK_UP,
		RSTICK_DOWN,
		RSTICK_LEFT,
		RSTICK_RIGHT,
		DPAD_UP,
		DPAD_DOWN,
		DPAD_LEFT,
		DPAD_RIGHT,
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

		// LSTICK_UP + DPAD_UP
		LEFT_UP,
		// LSTICK_DOWN + DPAD_DOWN
		LEFT_DOWN,
		// LSTICK_LEFT + DPAD_LEFT
		LEFT_LEFT,
		// LSTICK_RIGHT + DPAD_RIGHT
		LEFT_RIGHT,

		// L1 + L2
		L,
		// R1 + R2
		R,

		END
	};

	struct BUTTON_LOG {
		BUTTONS botton;
		bool isDown;
		bool isUp;
	};

	static void CreateInstance();
	static InputManager& GetInstance();

	bool Init();
	void Update();
	bool Release();

	void SetInputGroup();

private:
	static InputManager* instance_;

	std::map<BUTTONS, int> keyMap_;
	std::map<BUTTONS, int> padMap_;

	InputManager() {}
	~InputManager() {}

};
