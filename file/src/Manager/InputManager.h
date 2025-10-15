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
		BUTTON_16,
		BUTTON_17,
		BUTTON_18,
		BUTTON_19,
		BUTTON_20,
		BUTTON_21,
		BUTTON_22,
		BUTTON_23,
		BUTTON_24,
		BUTTON_25,
		BUTTON_26,
		BUTTON_27,
		BUTTON_28,
		BUTTON_29,
		BUTTON_30,
		BUTTON_31,

		END
	};

	static constexpr size_t MAP_INDEX_MAX = 2ULL;

	struct INPUT_MAP {
		std::array<BUTTONS, MAP_INDEX_MAX> padMap;
		std::array<int, MAP_INDEX_MAX> keyMap;
	};

	static void CreateInstance();
	static InputManager& GetInstance();

	bool Init();
	void Update();
	bool Release();

	void AddInputMap(const char* name, BUTTONS pad1, BUTTONS pad2, int key1, int key2);
	void AddInputMap(const char* name, std::array<BUTTONS, MAP_INDEX_MAX> pad_map, std::array<int, MAP_INDEX_MAX> key_map);
	void AddInputMap(const char* name, INPUT_MAP map);
	void ReplaceInputMapButton(const char* name, std::array<BUTTONS, MAP_INDEX_MAX> replace);
	void ReplaceInputMapButton(const char* name, BUTTONS replace, size_t index);
	void ReplaceInputMapKey(const char* name, std::array<int, MAP_INDEX_MAX> replace);
	void ReplaceInputMapKey(const char* name, int replace, size_t index);

	bool NowMap(const char* name) const;
	bool PrevMap(const char* name) const;
	bool DownMap(const char* name) const;

	int NowButton(BUTTONS) const;
	int PrevButton(BUTTONS) const;
	bool DownButton(BUTTONS) const;
	bool UpButton(BUTTONS) const;

	int NowKey(int DxLib_KEYcode) const;
	int PrevKey(int DxLib_KEYcode) const;
	bool DownKey(int DxLib_KEYcode) const;
	bool UpKey(int DxLib_KEYcode) const;

private:
	static constexpr double XINPUT_STICK_MULT = 1000.0 / (short)MAXSHORT;

	static InputManager* instance_;

	std::map<const char*, INPUT_MAP> inputMap_;
	std::array<int, (size_t)BUTTONS::END> nowButton_;
	std::array<int, (size_t)BUTTONS::END> prevButton_;

	std::array<char, 256ULL> nowKey_;
	std::array<char, 256ULL> prevKey_;

	InputManager() {}
	~InputManager() {}

	void GetKeyInput();
	void GetPadInput(int pad_num);

};
