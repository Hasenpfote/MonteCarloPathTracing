//==================================================================================
/*!
	@file	vector.cpp
    @brief  Vector 操作
    @author M.Morimoto
    @date   2004/06/11
*/
//==================================================================================
#include <iostream>
#include <iomanip>
#include "vector.h"

using namespace std;
//----------------------------------------------------------------------------------
/*!
	@brief		スケールオペレータ
	@param[i]	s: スケール
	@param[i]	v: 座標
	@retval		スケールの結果
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
const Vector2 operator * (float s, const Vector2& v)
{
	return Vector2(v.x * s, v.y * s);
}

//----------------------------------------------------------------------------------
/*!
	@brief		加算
	@param[o]	out: v1 + v2 の結果
	@param[i]	v1:
	@param[i]	v2:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector2* Vec2Add(Vector2* out, const Vector2* v1, const Vector2* v2)
{
	ASSERT_MSG(out != NULL, "Vec2Add()");
	ASSERT_MSG(v1  != NULL, "Vec2Add()");
	ASSERT_MSG(v2  != NULL, "Vec2Add()");

	out->x = v1->x + v2->x;
	out->y = v1->y + v2->y;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		減算
	@param[o]	out: v1 - v2 の結果
	@param[i]	v1:
	@param[i]	v2:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector2* Vec2Subtract(Vector2* out, const Vector2* v1, const Vector2* v2)
{
	ASSERT_MSG(out != NULL, "Vec2Subtract()");
	ASSERT_MSG(v1  != NULL, "Vec2Subtract()");
	ASSERT_MSG(v2  != NULL, "Vec2Subtract()");

	out->x = v1->x - v2->x;
	out->y = v1->y - v2->y;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		スケール
	@param[o]	out: v * s の結果
	@param[i]	v:
	@param[i]	s:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector2* Vec2Scale(Vector2* out, const Vector2* v, float s)
{
	ASSERT_MSG(out != NULL, "Vec2Scale()");
	ASSERT_MSG(v   != NULL, "Vec2Scale()");

	out->x = v->x * s;
	out->y = v->y * s;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		成分毎の最大
	@param[o]	out:
	@param[i]	v1:
	@param[i]	v2:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector2* Vec2Maximize(Vector2* out, const Vector2* v1, const Vector2* v2)
{
	ASSERT_MSG(out != NULL, "Vec2Maximize()");
	ASSERT_MSG(v1  != NULL, "Vec2Maximize()");
	ASSERT_MSG(v2  != NULL, "Vec2Maximize()");

	out->x = (v1->x > v2->x)? v1->x : v2->x;
	out->y = (v1->y > v2->y)? v1->y : v2->y;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		成分毎の最小
	@param[o]	out:
	@param[i]	v1:
	@param[i]	v2:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector2* Vec2Minimize(Vector2* out, const Vector2* v1, const Vector2* v2)
{
	ASSERT_MSG(out != NULL, "Vec2Minimize()");
	ASSERT_MSG(v1  != NULL, "Vec2Minimize()");
	ASSERT_MSG(v2  != NULL, "Vec2Minimize()");

	out->x = (v1->x < v2->x)? v1->x : v2->x;
	out->y = (v1->y < v2->y)? v1->y : v2->y;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		ベクトル長
	@param[i]	v:
	@retval		ベクトル長
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
float Vec2Length(const Vector2* v)
{
	ASSERT_MSG(v != NULL, "Vec2Length()");

	return sqrtf(v->x * v->x + v->y * v->y);
}

//----------------------------------------------------------------------------------
/*!
	@brief		ベクトル平方長
	@param[i]	v:
	@retval		ベクトル平方長
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
float Vec2LengthSq(const Vector2* v)	//	Square
{
	ASSERT_MSG(v != NULL, "Vec2LengthSq()");

	return v->x * v->x + v->y * v->y;
}

//----------------------------------------------------------------------------------
/*!
	@brief		正規化
	@param[o]	out: 正規化ベクトル
	@param[i]	v: 元ベクトル
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector2* Vec2Normalize(Vector2* out, const Vector2* v)
{
	ASSERT_MSG(out != NULL, "Vec2Normalize()");
	ASSERT_MSG(v   != NULL, "Vec2Normalize()");

	float s = Vec2Length(v);
	//	0除算チェック
	ASSERT_MSG(s != 0.0f, "Vec2Normalize()");
	Vec2Scale(out, v, 1.0f / s);
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		外積
	@param[i]	v1:
	@param[i]	v2:
	@retval		外積結果
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
float Vec2OuterProduct(const Vector2* v1, const Vector2* v2)
{
	ASSERT_MSG(v1  != NULL, "Vec2OuterProduct()");
	ASSERT_MSG(v2  != NULL, "Vec2OuterProduct()");

	return v1->x * v2->y - v1->y * v2->x;
}

//----------------------------------------------------------------------------------
/*!
	@brief		内積
	@param[i]	v1:
	@param[i]	v2:
	@retval		内積結果
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
float Vec2InnerProduct(const Vector2* v1, const Vector2* v2)
{
	ASSERT_MSG(v1  != NULL, "Vec2InnerProduct()");
	ASSERT_MSG(v2  != NULL, "Vec2InnerProduct()");

	return v1->x * v2->x + v1->y * v2->y;
}

//----------------------------------------------------------------------------------
/*!
	@brief		線形補間
	@param[o]	out: v1 + (v2 - v1) * s の結果
	@param[i]	v1:
	@param[i]	v2:
	@param[i]	s: 重み係数
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector2* Vec2Lerp(Vector2* out, const Vector2* v1, const Vector2* v2, float s)
{
	ASSERT_MSG(out != NULL, "Vec2Lerp()");
	ASSERT_MSG(v1  != NULL, "Vec2Lerp()");
	ASSERT_MSG(v2  != NULL, "Vec2Lerp()");

	out->x = v1->x + (v2->x - v1->x) * s;
	out->y = v1->y + (v2->y - v1->y) * s;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		重心座標計算
	@param[o]	out: 結果
	@param[i]	v1: 頂点 1
	@param[i]	v2: 頂点 2
	@param[i]	v3: 頂点 3
	@param[i]	f: 重み係数
	@param[i]	g: 重み係数
	@retval		out と同じ
	@note		D3DXVec3BaryCentric 関数は、三角形の実際の位置とは関係なく、
				三角形の内部や周辺の点を理解する方法を提供する。
				この関数は、次の式を使って結果である点を返す。V1 + f(V2-V1) + g(V3-V1)
				平面 V1V2V3 の点はいずれも、重心座標 (f,g) で表すことができる。
				パラメータ f は、V2 の結果に対する加重を制御し、
				パラメータ g は、V3 の結果に対する加重を制御する。
				最後に、1-f-g は V1 の結果に対する加重を制御する。
				次の関係に注意すること。
				(f>=0 && g>=0 && 1-f-g>=0) の場合、点は三角形 V1V2V3 内にある。
				(f==0 && g>=0 && 1-f-g>=0) の場合、点は線 V1V3 上にある。
				(f>=0 && g==0 && 1-f-g>=0) の場合、点は線 V1V2 上にある。
				(f>=0 && g>=0 && 1-f-g==0) の場合、点は線 V2V3 上にある。
				重心座標は一般座標の 1 つの形式である。
				このコンテキストで、重心座標を使うことは、座標系の変更を表している。
				直交座標で真のものは、重心座標でも真である。
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector2* Vec2BaryCentric(Vector2* out, const Vector2* v1, const Vector2* v2, const Vector2* v3, float f, float g)
{
	ASSERT_MSG(out != NULL, "Vec2BaryCentric()");
	ASSERT_MSG(v1  != NULL, "Vec2BaryCentric()");
	ASSERT_MSG(v2  != NULL, "Vec2BaryCentric()");
	ASSERT_MSG(v3  != NULL, "Vec2BaryCentric()");

	out->x = v1->x + f * (v2->x - v1->x) + g * (v3->x - v1->x);
	out->y = v1->y + f * (v2->y - v1->y) + g * (v3->y - v1->y);
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		デバッグ出力
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
void Vector2::debug_print() const
{
	cout << "< Vector2 >" << endl
		 << setprecision( 6 )
		 << setiosflags( ios::fixed )
		 << "[x]" << setw(10) << x << " "
		 << "[y]" << setw(10) << y << endl;
}

//----------------------------------------------------------------------------------
/*!
	@brief		スケールオペレータ
	@param[i]	s: スケール
	@param[i]	v: 座標
	@retval		スケールの結果
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
const Vector3 operator * (float s, const Vector3& v)
{
	return Vector3(v.x * s, v.y * s, v.z * s);
}

//----------------------------------------------------------------------------------
/*!
	@brief		加算
	@param[o]	out: v1 + v2 の結果
	@param[i]	v1:
	@param[i]	v2:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector3* Vec3Add(Vector3* out, const Vector3* v1, const Vector3* v2)
{
	ASSERT_MSG(out != NULL, "Vec3Add()");
	ASSERT_MSG(v1  != NULL, "Vec3Add()");
	ASSERT_MSG(v2  != NULL, "Vec3Add()");

	out->x = v1->x + v2->x;
	out->y = v1->y + v2->y;
	out->z = v1->z + v2->z;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		減算
	@param[o]	out: v1 - v2 の結果
	@param[i]	v1:
	@param[i]	v2:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector3* Vec3Subtract(Vector3* out, const Vector3* v1, const Vector3* v2)
{
	ASSERT_MSG(out != NULL, "Vec3Subtract()");
	ASSERT_MSG(v1  != NULL, "Vec3Subtract()");
	ASSERT_MSG(v2  != NULL, "Vec3Subtract()");

	out->x = v1->x - v2->x;
	out->y = v1->y - v2->y;
	out->z = v1->z - v2->z;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		スケール
	@param[o]	out: v * s の結果
	@param[i]	v:
	@param[i]	s:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector3* Vec3Scale(Vector3* out, const Vector3* v, float s)
{
	ASSERT_MSG(out != NULL, "Vec3Scale()");
	ASSERT_MSG(v   != NULL, "Vec3Scale()");

	out->x = v->x * s;
	out->y = v->y * s;
	out->z = v->z * s;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		成分毎の最大
	@param[o]	out:
	@param[i]	v1:
	@param[i]	v2:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector3* Vec3Maximize(Vector3* out, const Vector3* v1, const Vector3* v2)
{
	ASSERT_MSG(out != NULL, "Vec3Maximize()");
	ASSERT_MSG(v1  != NULL, "Vec3Maximize()");
	ASSERT_MSG(v2  != NULL, "Vec3Maximize()");

	out->x = (v1->x > v2->x)? v1->x : v2->x;
	out->y = (v1->y > v2->y)? v1->y : v2->y;
	out->z = (v1->z > v2->z)? v1->z : v2->z;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		成分毎の最小
	@param[o]	out:
	@param[i]	v1:
	@param[i]	v2:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector3* Vec3Minimize(Vector3* out, const Vector3* v1, const Vector3* v2)
{
	ASSERT_MSG(out != NULL, "Vec3Minimize()");
	ASSERT_MSG(v1  != NULL, "Vec3Minimize()");
	ASSERT_MSG(v2  != NULL, "Vec3Minimize()");

	out->x = (v1->x < v2->x)? v1->x : v2->x;
	out->y = (v1->y < v2->y)? v1->y : v2->y;
	out->z = (v1->z < v2->z)? v1->z : v2->z;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		ベクトル長
	@param[i]	v:
	@retval		ベクトル長
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
float Vec3Length(const Vector3* v)
{
	ASSERT_MSG(v != NULL, "Vec3Length()");

	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

//----------------------------------------------------------------------------------
/*!
	@brief		ベクトル平方長
	@param[i]	v:
	@retval		ベクトル平方長
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
float Vec3LengthSq(const Vector3* v)	//	Square
{
	ASSERT_MSG(v != NULL, "Vec3LengthSq()");

	return v->x * v->x + v->y * v->y + v->z * v->z;
}

//----------------------------------------------------------------------------------
/*!
	@brief		正規化
	@param[o]	out: 正規化ベクトル
	@param[i]	v: 元ベクトル
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector3* Vec3Normalize(Vector3* out, const Vector3* v)
{
	ASSERT_MSG(out != NULL, "Vec3Normalize()");
	ASSERT_MSG(v   != NULL, "Vec3Normalize()");

	float s = Vec3Length( v );
	//	0除算チェック
	ASSERT_MSG(s != 0.0f, "Vec3Normalize()");
	Vec3Scale(out, v, 1.0f / s);

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		外積
	@param[o]	out: 外積結果
	@param[i]	v1:
	@param[i]	v2:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector3* Vec3OuterProduct(Vector3* out, const Vector3* v1, const Vector3* v2)
{
	ASSERT_MSG(out != NULL, "Vec3OuterProduct()");
	ASSERT_MSG(v1  != NULL, "Vec3OuterProduct()");
	ASSERT_MSG(v2  != NULL, "Vec3OuterProduct()");

	float x, y, z;

	x = v1->y * v2->z - v1->z * v2->y;
	y = v1->z * v2->x - v1->x * v2->z;
	z = v1->x * v2->y - v1->y * v2->x;

	out->set(x, y, z);

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		内積
	@param[i]	v1:
	@param[i]	v2:
	@retval		内積結果
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
float Vec3InnerProduct(const Vector3* v1, const Vector3* v2)
{
	ASSERT_MSG(v1  != NULL, "Vec3InnerProduct()");
	ASSERT_MSG(v2  != NULL, "Vec3InnerProduct()");

	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

//----------------------------------------------------------------------------------
/*!
	@brief		線形補間
	@param[o]	out: v1 + (v2 - v1) * s の結果
	@param[i]	v1:
	@param[i]	v2:
	@param[i]	s: 重み係数
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector3* Vec3Lerp(Vector3* out, const Vector3* v1, const Vector3* v2, float s)
{
	ASSERT_MSG(out != NULL, "Vec3Lerp()");
	ASSERT_MSG(v1  != NULL, "Vec3Lerp()");
	ASSERT_MSG(v2  != NULL, "Vec3Lerp()");

	out->x = v1->x + (v2->x - v1->x) * s;
	out->y = v1->y + (v2->y - v1->y) * s;
	out->z = v1->z + (v2->z - v1->z) * s;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		重心座標計算
	@param[o]	out: 結果
	@param[i]	v1: 頂点 1
	@param[i]	v2: 頂点 2
	@param[i]	v3: 頂点 3
	@param[i]	f: 重み係数
	@param[i]	g: 重み係数
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector3* Vec3BaryCentric(Vector3* out, const Vector3* v1, const Vector3* v2, const Vector3* v3, float f, float g)
{
	ASSERT_MSG(out != NULL, "Vec3BaryCentric()");
	ASSERT_MSG(v1  != NULL, "Vec3BaryCentric()");
	ASSERT_MSG(v2  != NULL, "Vec3BaryCentric()");
	ASSERT_MSG(v3  != NULL, "Vec3BaryCentric()");

	out->x = v1->x + f * (v2->x - v1->x) + g * (v3->x - v1->x);
	out->y = v1->y + f * (v2->y - v1->y) + g * (v3->y - v1->y);
	out->z = v1->z + f * (v2->z - v1->z) + g * (v3->z - v1->z);
	return out;
}

/*!
	@brief		正規直行基底による直行変換
	@param[o]	out: 変換後の v
	@param[i]	v: 変換前
	@param[i]	x: 基底 x 軸
	@param[i]	y: 基底 y 軸
	@param[i]	z: 基底 z 軸
 */
Vector3* Vec3OrthogonalTransform(Vector3* out, const Vector3* v, const Vector3* x, const Vector3* y, const Vector3* z)
{
	ASSERT_MSG(out != NULL, "Vec3OrthogonalTransform()");
	ASSERT_MSG(v   != NULL, "Vec3OrthogonalTransform()");
	ASSERT_MSG(x   != NULL, "Vec3OrthogonalTransform()");
	ASSERT_MSG(y   != NULL, "Vec3OrthogonalTransform()");
	ASSERT_MSG(z   != NULL, "Vec3OrthogonalTransform()");

	out->x = v->x * x->x + v->y * y->x + v->z * z->x;
	out->y = v->x * x->y + v->y * y->y + v->z * z->y;
	out->z = v->x * x->z + v->y * y->z + v->z * z->z;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		デバッグ出力
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
void Vector3::debug_print() const
{
	cout << "< Vector3 >" << endl
		 << setprecision( 6 )
		 << setiosflags( ios::fixed )
		 << "[x]" << setw(10) << x << " "
		 << "[y]" << setw(10) << y << " "
		 << "[z]" << setw(10) << z << endl;
}

//----------------------------------------------------------------------------------
/*!
	@brief		スケールオペレータ
	@param[i]	s: スケール
	@param[i]	v: 座標
	@retval		スケールの結果
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
const Vector4 operator * (float s, const Vector4& v)
{
	return Vector4(v.x * s, v.y * s, v.z * s, v.w * s);
}

//----------------------------------------------------------------------------------
/*!
	@brief		加算
	@param[o]	out: v1 + v2 の結果
	@param[i]	v1:
	@param[i]	v2:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector4* Vec4Add(Vector4* out, const Vector4* v1, const Vector4* v2)
{
	ASSERT_MSG(out != NULL, "Vec4Add()");
	ASSERT_MSG(v1  != NULL, "Vec4Add()");
	ASSERT_MSG(v2  != NULL, "Vec4Add()");

	out->x = v1->x + v2->x;
	out->y = v1->y + v2->y;
	out->z = v1->z + v2->z;
	out->w = v1->w + v2->w;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		減算
	@param[o]	out: v1 - v2 の結果
	@param[i]	v1:
	@param[i]	v2:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector4* Vec4Subtract(Vector4* out, const Vector4* v1, const Vector4* v2)
{
	ASSERT_MSG(out != NULL, "Vec4Subtract()");
	ASSERT_MSG(v1  != NULL, "Vec4Subtract()");
	ASSERT_MSG(v2  != NULL, "Vec4Subtract()");

	out->x = v1->x - v2->x;
	out->y = v1->y - v2->y;
	out->z = v1->z - v2->z;
	out->w = v1->w - v2->w;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		スケール
	@param[o]	out: v * s の結果
	@param[i]	v:
	@param[i]	s:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector4* Vec4Scale(Vector4* out, const Vector4* v, float s)
{
	ASSERT_MSG(out != NULL, "Vec3Scale()");
	ASSERT_MSG(v   != NULL, "Vec3Scale()");

	out->x = v->x * s;
	out->y = v->y * s;
	out->z = v->z * s;
	out->w = v->w * s;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		成分毎の最大
	@param[o]	out:
	@param[i]	v1:
	@param[i]	v2:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector4* Vec4Maximize(Vector4* out, const Vector4* v1, const Vector4* v2)
{
	ASSERT_MSG(out != NULL, "Vec4Maximize()");
	ASSERT_MSG(v1  != NULL, "Vec4Maximize()");
	ASSERT_MSG(v2  != NULL, "Vec4Maximize()");

	out->x = ( v1->x > v2->x )? v1->x : v2->x;
	out->y = ( v1->y > v2->y )? v1->y : v2->y;
	out->z = ( v1->z > v2->z )? v1->z : v2->z;
	out->w = ( v1->w > v2->w )? v1->w : v2->w;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		成分毎の最小
	@param[o]	out:
	@param[i]	v1:
	@param[i]	v2:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector4* Vec4Minimize(Vector4* out, const Vector4* v1, const Vector4* v2)
{
	ASSERT_MSG(out != NULL, "Vec4Minimize()");
	ASSERT_MSG(v1  != NULL, "Vec4Minimize()");
	ASSERT_MSG(v2  != NULL, "Vec4Minimize()");

	out->x = (v1->x < v2->x)? v1->x : v2->x;
	out->y = (v1->y < v2->y)? v1->y : v2->y;
	out->z = (v1->z < v2->z)? v1->z : v2->z;
	out->w = (v1->w < v2->w)? v1->w : v2->w;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		ベクトル長
	@param[i]	v:
	@retval		ベクトル長
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
float Vec4Length(const Vector4* v)
{
	ASSERT_MSG(v != NULL, "Vec4Length()");

	return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}

//----------------------------------------------------------------------------------
/*!
	@brief		ベクトル平方長
	@param[i]	v:
	@retval		ベクトル平方長
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
float Vec4LengthSq(const Vector4* v)	//	Square
{
	ASSERT_MSG(v != NULL, "Vec4LengthSq()");

	return v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w;
}

//----------------------------------------------------------------------------------
/*!
	@brief		正規化
	@param[o]	out: 正規化ベクトル
	@param[i]	v: 元ベクトル
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector4* Vec4Normalize(Vector4* out, const Vector4* v)
{
	ASSERT_MSG(out != NULL, "Vec4Normalize()");
	ASSERT_MSG(v   != NULL, "Vec4Normalize()");

	float s = Vec4Length(v);
	//	0除算チェック
	ASSERT_MSG(s != 0.0f, "Vec4Normalize()");
	Vec4Scale(out, v, 1.0f / s);
	return out;
}
/*
Vector4*
Vec4OuterProduct( Vector4* out, const Vector4* v1, const Vector4* v2, const Vector4* v3 )
{
	return out;
}
*/

//----------------------------------------------------------------------------------
/*!
	@brief		内積
	@param[i]	v1:
	@param[i]	v2:
	@retval		内積結果
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
float Vec4InnerProduct(const Vector4* v1, const Vector4* v2)
{
	ASSERT_MSG(v1 != NULL, "Vec4InnerProduct()");
	ASSERT_MSG(v2 != NULL, "Vec4InnerProduct()");

	return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z + v1->w * v2->w;
}

//----------------------------------------------------------------------------------
/*!
	@brief		線形補間
	@param[o]	out: v1 + (v2 - v1) * s の結果
	@param[i]	v1:
	@param[i]	v2:
	@param[i]	s: 重み係数
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector4* Vec4Lerp(Vector4* out, const Vector4* v1, const Vector4* v2, float s)
{
	ASSERT_MSG(out != NULL, "Vec4Lerp()");
	ASSERT_MSG(v1  != NULL, "Vec4Lerp()");
	ASSERT_MSG(v2  != NULL, "Vec4Lerp()");

	out->x = v1->x + ( v2->x - v1->x ) * s;
	out->y = v1->y + ( v2->y - v1->y ) * s;
	out->z = v1->z + ( v2->z - v1->z ) * s;
	out->w = v1->w + ( v2->w - v1->w ) * s;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		重心座標計算
	@param[o]	out: 結果
	@param[i]	v1: 頂点 1
	@param[i]	v2: 頂点 2
	@param[i]	v3: 頂点 3
	@param[i]	f: 重み係数
	@param[i]	g: 重み係数
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector4* Vec4BaryCentric(Vector4* out, const Vector4* v1, const Vector4* v2, const Vector4* v3, float f, float g)
{
	ASSERT_MSG(out != NULL, "Vec4BaryCentric()");
	ASSERT_MSG(v1  != NULL, "Vec4BaryCentric()");
	ASSERT_MSG(v2  != NULL, "Vec4BaryCentric()");
	ASSERT_MSG(v3  != NULL, "Vec4BaryCentric()");

	out->x = v1->x + f * (v2->x - v1->x) + g * (v3->x - v1->x);
	out->y = v1->y + f * (v2->y - v1->y) + g * (v3->y - v1->y);
	out->z = v1->z + f * (v2->z - v1->z) + g * (v3->z - v1->z);
	out->w = v1->w + f * (v2->w - v1->w) + g * (v3->w - v1->w);
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		デバッグ出力
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
void Vector4::debug_print() const
{
	cout << "< Vector4 >" << endl
		 << setprecision( 6 )
		 << setiosflags( ios::fixed )
		 << "[x]" << setw(10) << x << " "
		 << "[y]" << setw(10) << y << " "
		 << "[z]" << setw(10) << z << " "
		 << "[y]" << setw(10) << w << endl;
}
