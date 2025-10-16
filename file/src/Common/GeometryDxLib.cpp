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

VECTOR GeometryDxLib::VZero() {
	return { 0.0f, 0.0f, 0.0f };
}

VECTOR GeometryDxLib::VGetIdentX() {
	return { 1.0f, 0.0f, 0.0f };
}

VECTOR GeometryDxLib::VGetIdentY() {
	return { 0.0f, 1.0f, 0.0f };
}

VECTOR GeometryDxLib::VGetIdentZ() {
	return { 0.0f, 0.0f, 1.0f };
}

VECTOR GeometryDxLib::VLerp(const VECTOR& In1, const VECTOR& In2, float lerp) {
	VECTOR v = {};

	if (lerp <= 1.0f) {
		v.x = In1.x + (In2.x - In1.x) * lerp;
		v.y = In1.y + (In2.y - In1.y) * lerp;
		v.z = In1.z + (In2.z - In1.z) * lerp;
	}

	return v;
}

VECTOR GeometryDxLib::VLerpRad(const VECTOR& In1, const VECTOR& In2, float lerp) {
	VECTOR v = {};

	if (lerp <= 1.0f) {
		v.x = LerpRad(In1.x, In2.x, lerp);
		v.y = LerpRad(In1.y, In2.y, lerp);
		v.z = LerpRad(In1.z, In2.z, lerp);
	}

	return v;
}

VECTOR GeometryDxLib::VMinus(const VECTOR& In) {
	return { -In.x, -In.y, -In.z };
}

VECTOR GeometryDxLib::VInverse(const VECTOR& In) {
	return VMinus(In);
}

bool GeometryDxLib::VEquals(const VECTOR& In1, const VECTOR& In2) {
	return In1.x == In2.x && In1.y == In2.y && In1.z == In2.z;
}

bool GeometryDxLib::DrawShadow3D(VECTOR pos, float r, int div_num) {
	if (div_num < 4) return false;

	VECTOR rPos = { 0.0f, 0.0f, r };

	MATRIX rot = MGetIdent();
	CreateRotationYMatrix(&rot, DX_TWO_PI_F / div_num);

	VECTOR rPos2 = VTransform(rPos, rot);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0x80);
	for (int d = 0; d < div_num; d++) {
		DrawTriangle3D(pos, VAdd(pos, rPos), VAdd(pos, rPos2), 0x202020u, true);

		rPos = VTransform(rPos, rot);
		rPos2 = VTransform(rPos2, rot);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0x00);

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
