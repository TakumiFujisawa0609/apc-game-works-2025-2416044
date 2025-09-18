#pragma once
#include <DxLib.h>
#include "Geometry.h"

class GeometryDxLib {
	static FLOAT2 Vector2ToFLOAT2(const Vector2& v);
	static Vector2 FLOAT2ToVector2(const FLOAT2& f);

	static VECTOR Vector3ToVECTOR(const Vector3& v);
	static Vector3 VECTORToVector3(const VECTOR& v);

	static MATRIX Matrix4x4ToMATRIX(const Matrix4x4& m);
	static Matrix4x4 MATRIXToMatrix4x4(const MATRIX& m);

};

