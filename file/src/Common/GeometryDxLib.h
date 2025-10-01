#pragma once
#include <DxLib.h>
#include "Geometry.h"

class GeometryDxLib {
public:
	static FLOAT2 Vector2ToFLOAT2(const Vector2& v);
	static Vector2 FLOAT2ToVector2(const FLOAT2& f);

	static VECTOR Vector3ToVECTOR(const Vector3& v);
	static Vector3 VECTORToVector3(const VECTOR& v);

	static MATRIX Matrix4x4ToMATRIX(const Matrix4x4& m);
	static Matrix4x4 MATRIXToMatrix4x4(const MATRIX& m);

	// VECTOR 型の単項マイナス演算
	static VECTOR VInverse(const VECTOR& In);

	// VECTOR 型の等価演算
	static bool VEquals(const VECTOR& In1, const VECTOR& In2);
	
	// 疑似的な影を描画する（div_num が4未満の場合は描画しない）
	static bool DrawShadow3D(VECTOR pos, float r, int div_num = 4);

	// オイラー角からXYZ順の回転行列を取得
	static MATRIX GetMatrixRotateXYZ(const VECTOR& euler);

	// 親子の回転行列を合成する
	static MATRIX Multiplication(const MATRIX& child, const MATRIX& parent);

	// 親子のオイラー角を合成する
	static MATRIX Multiplication(const VECTOR& childEuler, const VECTOR& parentEuler);

};
