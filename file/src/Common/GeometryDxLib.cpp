#include "GeometryDxLib.h"

FLOAT2 GeometryDxLib::Vector2ToFLOAT2(const Vector2& v) {
	return FLOAT2(v.x, v.y);
}

Vector2 GeometryDxLib::FLOAT2ToVector2(const FLOAT2& f) {
	return Vector2(f.u, f.v);
}

VECTOR GeometryDxLib::Vector3ToVECTOR(const Vector3& v) {
	return VECTOR(v.x, v.y, v.z);
}

Vector3 GeometryDxLib::VECTORToVector3(const VECTOR& v) {
	return Vector3(v.x, v.y, v.z);
}

MATRIX GeometryDxLib::Matrix4x4ToMATRIX(const Matrix4x4& m) {
	return {
		m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
		m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
		m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
		m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]
	};
}

Matrix4x4 GeometryDxLib::MATRIXToMatrix4x4(const MATRIX& m) {
	return {
		m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
		m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
		m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
		m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]
	};
}

VECTOR GeometryDxLib::VLeap(const VECTOR& In1, const VECTOR& In2, float leap) {
	VECTOR v = {};
	v.x = In1.x + (In2.x - In1.x) * leap;
	v.y = In1.y + (In2.y - In1.y) * leap;
	v.z = In1.z + (In2.z - In1.z) * leap;
	return v;
}

VECTOR GeometryDxLib::VInverse(const VECTOR& In) {
	return { -In.x, -In.y, -In.z };
}

bool GeometryDxLib::VEquals(const VECTOR& In1, const VECTOR& In2) {
	return In1.x == In2.x && In1.y == In2.y && In1.z == In2.z;
}

bool GeometryDxLib::DrawShadow3D(VECTOR pos, float r, int div_num) {
	if (div_num < 4) return false;

	VECTOR rPos = VAdd(pos, { 0.f, 0.f, r });
	VECTOR rPos2 = rPos;

	MATRIX rot = {};
	CreateRotationYMatrix(&rot, DX_TWO_PI_F / div_num);

	VTransform(rPos2, rot);

	DrawTriangle3D(pos, rPos, rPos2, 0x202020, true);

	for (int d = 1; d < div_num; d++) {

	}

	return true;
}

MATRIX GeometryDxLib::GetMatrixRotateXYZ(const VECTOR& euler) {
	MATRIX ret = MGetIdent();
	ret = MMult(ret, MGetRotX(euler.x));
	ret = MMult(ret, MGetRotY(euler.y));
	ret = MMult(ret, MGetRotZ(euler.z));
	return ret;
}

MATRIX GeometryDxLib::Multiplication(const MATRIX& child, const MATRIX& parent)
{
	return MMult(child, parent);
}

MATRIX GeometryDxLib::Multiplication(const VECTOR& childEuler, const VECTOR& parentEuler)
{
	return MMult(GetMatrixRotateXYZ(childEuler), GetMatrixRotateXYZ(parentEuler));
}
