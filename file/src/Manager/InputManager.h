#pragma once
#include <array>
#include <list>
#include <map>
#include "DxLib.h"

class InputManager {
public:
	enum class BUTTONS {
		NONE,

		LSTICK_L,
		LSTICK_R,
		LSTICK_U,
		LSTICK_D,

		RSTICK_L,
		RSTICK_R,
		RSTICK_U,
		RSTICK_D,

		DPAD_L,
		DPAD_R,
		DPAD_U,
		DPAD_D,

		BUTTON_0,
		BUTTON_1,
		BUTTON_2,
		BUTTON_3,
		BUTTON_4,
		BUTTON_5,
		BUTTON_6,
		BUTTON_7,
		BUTTON_8,
		BUTTON_9,
		BUTTON_10,
		BUTTON_11,
		BUTTON_12,
		BUTTON_13,
		BUTTON_14,
		BUTTON_15,

		END
	};

	static constexpr size_t KEY_INDEX_MAX = 2Ui64;

	struct BUTTON_MAP {
		BUTTONS padMap;
		std::array<int, KEY_INDEX_MAX> keyMap;
	};

	static void CreateInstance();
	static InputManager& GetInstance();

	bool Init();
	void Update();
	bool Release();

	void AddButtonMap(const char* name, BUTTONS padMap = BUTTONS::NONE, std::array<int, KEY_INDEX_MAX> keyMap = { 0x00, 0x00 });
	void ReplacePadMap(const char* name, BUTTONS replace);
	void ReplaceKeyMap(const char* name, std::array<int, KEY_INDEX_MAX> replace);
	void ReplaceKeyMap(const char* name, int replace, size_t index);

	int GetNowButtonState(BUTTONS) const;
	int GetPrevButtonState(BUTTONS) const;

private:
	static InputManager* instance_;

	std::map<const char*, BUTTON_MAP> buttonMap_;
	std::array<int, static_cast<size_t>(BUTTONS::END)> nowButton_;
	std::array<int, static_cast<size_t>(BUTTONS::END)> prevButton_;

	InputManager() {}
	~InputManager() {}

	void GetKeyInput();
	void GetPadInput(int pad_num);

};
