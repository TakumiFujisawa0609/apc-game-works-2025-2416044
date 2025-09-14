#pragma once
#include <array>
#include <DxLib.h>
#include "../../Common/Geometry.h"

struct BoundingBox { //ダイスの最小最大の座標
	Vector3 min;
	Vector3 max;
};

class Cube {
private:
	int imgHandle_;
	Vector3 pos;

	std::array<VERTEX3D, 24> vertices_;
	std::array<uint16_t, 36> indices_;

public:
	Cube();

	void Update(const Matrix4x4& mat);
	void Draw() const;

	BoundingBox GetBoundingBox() const;

};
