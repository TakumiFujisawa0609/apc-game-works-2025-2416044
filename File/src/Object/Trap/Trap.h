#pragma once
#include <DxLib.h>
#include <list>
#include "../../Common/Geometry.h"

class Stage;

class Trap {
public:
	static constexpr unsigned int COLOR_TRAP = 0x4040FFU;
	static constexpr unsigned int COLOR_SUPER_TRAP = 0x40FF4FU;
	static constexpr unsigned int COLOR_EXECUTE = 0xFF4040U;

	static constexpr int WAIT_DEPLOY = 6;
	static constexpr int WAIT_EXECUTE = 12;

	enum class TRAP_TYPE {
		NORMAL,
		ADVANCE,
	};

	struct TRAP {
		TRAP_TYPE type;
		Vector2 stagePos;
		unsigned int color;
		int depWait;
		int exeWait;
	};

	Trap(Stage*);

	bool Init();
	void Update();
	void Draw();
	bool Release();

	void SetTrap(const VECTOR& pos, TRAP_TYPE type = TRAP_TYPE::NORMAL);
	void ExecuteAdvTrap();
	void Reset();

	bool CheckTrapReady() const;
	Vector2 GetStagePos() const;

private:
	Stage* stage_;

	bool setTrap_;
	bool readyTrap_;
	int waitTrapExecute_;
	Vector2 stagePos_;

	std::list<TRAP> traps_;

	bool prevKey_;

	void SetNormalTrap(const VECTOR& pos);
	void SetAdvanceTrap(const VECTOR& pos);

};

