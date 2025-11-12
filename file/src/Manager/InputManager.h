#pragma once
#include <array>
#include <list>
#include <map>
#include "DxLib.h"

class InputManager {
public:
	/// シングルトン
	static void CreateInstance() { if (instance_ == nullptr) instance_ = new InputManager; }
	static InputManager& GetInstance() { return *instance_; }
	static void DeleteInstance() { if (instance_ != nullptr) delete instance_; instance_ = nullptr; }

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

	enum class TAGS {
		NONE,

		CONFIG,
		START,
		PAUSE,

		ATTACK_1,
		ATTACK_2,
		ATTACK_3,
		DASH,
		JUMP,
		MOVE_BACK,
		MOVE_DOWN,
		MOVE_FRONT,
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_UP,
	};

	static constexpr size_t MAP_INDEX_MAX = 2ULL;

	struct INPUT_MAP {
		std::array<BUTTONS, MAP_INDEX_MAX> padMap;
		std::array<int, MAP_INDEX_MAX> keyMap;
	};

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
	/// シングルトン
	static InputManager* instance_;

	InputManager() {}
	~InputManager() {}

	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	InputManager(InputManager&&) = delete;
	InputManager& operator=(InputManager&&) = delete;

	static constexpr double XINPUT_STICK_MULT = 1000.0 / (short)0x7fff;

	std::map<const char*, INPUT_MAP> inputMap_;
	std::array<int, (size_t)BUTTONS::END> nowButton_;
	std::array<int, (size_t)BUTTONS::END> prevButton_;

	std::array<char, 256ULL> nowKey_;
	std::array<char, 256ULL> prevKey_;

	void GetKeyInput();
	void GetPadInput(int pad_num);

};
