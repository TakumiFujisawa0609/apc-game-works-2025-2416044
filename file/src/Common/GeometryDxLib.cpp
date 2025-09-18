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
	return MATRIX(
		m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
		m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
		m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
		m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]
	);
}

Matrix4x4 GeometryDxLib::MATRIXToMatrix4x4(const MATRIX& m) {
	return Matrix4x4(
		m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
		m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
		m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
		m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3]
	);
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
