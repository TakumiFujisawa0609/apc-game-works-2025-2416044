#include <DxLib.h>
#include "../../Common/Geometry.h"
#include "Cube.h"
#include "Stage.h"

bool Stage::SystemInit() {
	normalImg_ = LoadGraph("data/image/dice.png");
	return true;
}

bool Stage::GameInit() {
	// 足場
	for (int pd = 0; pd < PLATFORM_DEPTH; ++pd) {
		// 足場リスト
		std::list<Cube*> platformSubList;
		for (int pw = 0; pw < CUBE_WIDTH; ++pw) {
			// 足場ブロック
			auto pb = new Cube(HALF_CUBE_SIZE);
			// 足場外見
			pb->SetTexture(normalImg_);
			// 場所を指定
			pb->Update(TranslationMatrix({ pw * CUBE_SIZE + HALF_CUBE_SIZE, -HALF_CUBE_SIZE, -pd * CUBE_SIZE - HALF_CUBE_SIZE }));
			// 挿入
			platformSubList.emplace_back(pb);
		}
		// 挿入
		platformList_.emplace_back(platformSubList);
	}

	// キューブ
	for (int cd = 0; cd < CUBE_DEPTH; ++cd) {
		// キューブリスト
		std::list<Cube*> cubeSubList;
		for (int cw = 0; cw < CUBE_WIDTH; ++cw) {
			// キューブブロック
			auto cb = new Cube(HALF_CUBE_SIZE);
			// キューブ外見
			cb->SetTexture(normalImg_);
			// 場所を指定
			cb->Update(TranslationMatrix({ cw * CUBE_SIZE + HALF_CUBE_SIZE, HALF_CUBE_SIZE, -cd * CUBE_SIZE - HALF_CUBE_SIZE }));
			// 挿入
			cubeSubList.emplace_back(cb);
		}
		// 挿入
		cubeList_.emplace_back(cubeSubList);
	}

	spinTimer_ = 0;
	delayTimer_ = 0;

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
		for (auto& i : cubeList_) for (auto j : i) {
			auto mat = IdentityMatrix();

			auto bb = j->GetBoundingBox();
			//mat = TranslationMatrix({ 0.F, bb.min.y, bb.min.z }) * RotationMatrixX(DX_PI_F / 180.F * -SPIN_DEGREE) * TranslationMatrix({ 0.F, -bb.min.y, -bb.min.z });
			mat = TranslationMatrix({ 0.F, bb.min.y, bb.min.z }) * RotationMatrixX(1.F) * TranslationMatrix({ 0.F, -bb.min.y, -bb.min.z });

			j->Update(mat);
		}
	}
}

void Stage::Draw() {
	for (auto& i : platformList_) for (auto j : i) j->Draw();
	for (auto& i : cubeList_) for (auto j : i) j->Draw();
}

bool Stage::Release() {
	for (auto& i : platformList_) for (auto j : i) delete j;
	platformList_.clear();

	for (auto& i : cubeList_) for (auto j : i) delete j;
	cubeList_.clear();

	DeleteGraph(normalImg_);

	return true;
}
