#pragma once
#include <DxLib.h>
#include "../../Common/Geometry.h"

class Stage;

class Trap {
public:
	Trap(Stage*);

	bool Init();
	void Update(const VECTOR& player_pos);
	void Draw();

	void Reset();

	bool CheckTrapReady() const;
	Vector2 GetStagePos() const;

private:
	Stage* stage_;

	bool setTrap_;
	bool readyTrap_;
	Vector2 stagePos_;

	bool prevSpaceKey_;

};

