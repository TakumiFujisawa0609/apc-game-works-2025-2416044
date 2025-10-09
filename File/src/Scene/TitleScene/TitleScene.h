#pragma once
#include "../SceneBase.h"

class TitleScene : public SceneBase {
public:
	bool SystemInit() override;
	void Update() override;
	void Draw() override;

};
