#include <DxLib.h>
#include "../../Common/Geometry.h"
#include "Cube.h"
#include "Stage.h"

bool Stage::SystemInit() {
	return true;
}

bool Stage::GameInit() {
	return true;
}

void Stage::Update() {
	if (spinTimer_ <= 0) {
		if (--delayTimer_ <= 0) {
			spinTimer_ = SPIN_FRAME;
			delayTimer_ = DELAY_FRAME;
		}
	}
	else {
		--spinTimer_;
		for (auto& i : cubes_) for (auto& j : i) {
			auto mat = IdentityMatrix();

			auto bb = j.GetBoundingBox();
			Vector3 tMat = { 0.F, bb.min.y, bb.min.z };
			mat = TranslationMatrix(tMat) * (RotationMatrixX(SPIN_DEGREE) * TranslationMatrix(-tMat));

			j.Update(mat);
		}
	}
}

void Stage::Draw() {
	for (auto& i : cubes_) for (auto& j : i) j.Draw();
}

bool Stage::Release() {
	return true;
}
