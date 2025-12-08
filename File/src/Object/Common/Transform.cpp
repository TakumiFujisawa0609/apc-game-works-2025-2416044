#include "Transform.h"

Transform::Transform() :
	modelId(-1),
	scale(1.0f, 1.0f, 1.0f),
	pos(),
	localPos(),
	rot(),
	localRot()
{
}

Transform::~Transform()
{
	MV1DeleteModel(modelId);
}

void Transform::Update()
{
	mScale = MGetScale(scale);

	rot = GeometryDxLib::Vector3ToVECTOR(qRot.ToEuler());
	mRot = GeometryDxLib::Matrix4x4ToMATRIX(qRot.ToMatrix());

	mPos = MGetTranslate(pos);

	MATRIX mat = MGetIdent();
	mat = MMult(mat, mScale);
	Quaternion q = qRot * qLocalRot;
	mat = MMult(mat, GeometryDxLib::Matrix4x4ToMATRIX(q.ToMatrix()));
	mat = MMult(mat, mPos);

	if (modelId != -1)
	{
		MV1SetMatrix(modelId, mat);
	}
}
