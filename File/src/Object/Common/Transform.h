#pragma once
#include <DxLib.h>
#include "../../Common/Geometry.h"
#include "../../Common/GeometryDxLib.h"

class Transform
{
public:
	int modelId;

	VECTOR scale;

	VECTOR pos;
	VECTOR localPos;

	VECTOR rot;
	VECTOR localRot;

	Transform();
	
	~Transform();

	void Update();

private:
	MATRIX mScale;
	MATRIX mRot;
	MATRIX mPos;

	Quaternion qRot;
	Quaternion qLocalRot;


};
