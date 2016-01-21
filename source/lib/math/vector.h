//==================================================================================
/*!
	@file	vector.h
    @brief  Vector 操作
    @author M.Morimoto
    @date   2004/06/11
 */
//==================================================================================
#ifndef __VECTOR_H_
#define __VECTOR_H_

#include "../lib_common.h"
#include "math.h"

#pragma warning(disable:4201) 							// anonymous unions warning

/*!
	@class	Vector2
	@brief	2d vector
	@author	M.Morimoto
	@date	2004/06/11
*/
class Vector2
{
public:
	//------------------------------------------------------------------------------
	//	CONSTRUCTOR
	//------------------------------------------------------------------------------
	Vector2(){}
	/*!
		@brief		コピーコンストラクタ
		@param[in]	v :
	*/
	Vector2(const Vector2& v)
	{
		x = v.x;
		y = v.y;
	}
	/*!
		@brief		コピーコンストラクタ
		@param[in]	x :
		@param[in]	y :
	*/
	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	/*!
		@brief		コピーコンストラクタ
		@param[in]	v :
	*/
	Vector2(const float* v)
	{
		ASSERT_MSG(v != NULL, "Vector2::Vector2()");
		x = v[0];
		y = v[1];
	}
	//------------------------------------------------------------------------------
	//	CASTING
	//------------------------------------------------------------------------------
	/*!
		@brief		float 型ポインタにキャスト
		@retval		v[0] への float 型ポインタ
	*/
	operator float* (){ return (float*)&x; }
	/*!
		@brief		float 型ポインタにキャスト
		@retval		v[0] への float 型ポインタ
	*/
	operator const float* () const { return (const float*)&x; }
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	/*!
		@brief		代入オペレータ
		@param[in]	v :
		@retval		参照
	*/
	Vector2& operator = (const float* v)
	{
		ASSERT_MSG(v != NULL, "Vector2::operator = ()");
		x = v[0];
		y = v[1];
		return *this;
	}
	/*!
		@brief		代入オペレータ
		@param[in]	v :
		@retval		参照
	*/
	Vector2& operator = (const Vector2& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}
	//------------------------------------------------------------------------------
	// ASSIGNMENT OPERATORS
	//------------------------------------------------------------------------------
	/*!
		@brief		加算代入オペレータ
		@param[in]	v :
		@retval		参照
	*/
	Vector2& operator += (const Vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	/*!
		@brief		減算代入オペレータ
		@param[in]	v :
		@retval		参照
	*/
	Vector2& operator -= (const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	/*!
		@brief		スケール代入オペレータ
		@param[in]	s :
		@retval		参照
	*/
	Vector2& operator *= (float s)
	{
		x *= s;
		y *= s;
		return *this;
	}
	/*!
		@brief		除算代入オペレータ
		@param[in]	s :
		@retval		参照
	*/
	Vector2& operator /= (float s)
	{
		ASSERT_MSG(s != 0.0f, "Vector2::operator /= ()");
		x /= s;
		y /= s;
		return *this;
	}
	//------------------------------------------------------------------------------
	// UNARY OPERATORS
	//------------------------------------------------------------------------------
	/*!
		@brief		正符号オペレータ
		@param[in]	s :
		@retval		+(*this)
	*/
	const Vector2 operator + () const { return *this; }
	/*!
		@brief		負符号オペレータ
		@param[in]	s :
		@retval		-(*this)
	*/
	const Vector2 operator - () const { return Vector2(-x, -y); }
	//------------------------------------------------------------------------------
	// BINARY OPERATORS
	//------------------------------------------------------------------------------
	/*!
		@brief		加算オペレータ
		@param[in]	v :
		@retval		(*this) + v
	*/
	const Vector2 operator + (const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
	/*!
		@brief		減算オペレータ
		@param[in]	v :
		@retval		(*this) - v
	*/
	const Vector2 operator - (const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
	/*!
		@brief		スケールオペレータ
		@param[in]	s :
		@retval		(*this) * s
	*/
	const Vector2 operator * (float s) const { return Vector2(x * s, y * s); }
	/*!
		@brief		除算オペレータ
		@param[in]	s :
		@retval		(*this) / s
	*/
	const Vector2 operator / (float s) const
	{
		ASSERT_MSG(s != 0.0f, "Vector2::operator / () const");
		return Vector2(x / s, y / s);
	}
	/*!
		@brief		スケールオペレータ
		@param[in]	s :
		@retval		s * v
	*/
	friend const Vector2 operator * (float s, const Vector2& v);
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : ＝
		@retval		false : ≠
	*/
	bool operator == (const Vector2& v) const { return (x == v.x && y == v.y); }
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : ≠
		@retval		false : ＝
	*/
	bool operator != (const Vector2& v) const { return (x != v.x || y != v.y); }
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : (*this) <  v
		@retval		false : (*this) >= v
	*/
	bool operator < (const Vector2& v) const { return ((x*x+y*y) < (v.x*v.x+v.y*v.y))? true : false; }
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : (*this) <= v
		@retval		false : (*this) >  v
	*/
	bool operator <= (const Vector2& v) const { return ((x*x+y*y) <= (v.x*v.x+v.y*v.y))? true : false; }
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : (*this) >  v
		@retval		false : (*this) <= v
	*/
	bool operator > (const Vector2& v) const { return ((x*x+y*y) > (v.x*v.x+v.y*v.y))? true : false; }
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : (*this) >= v
		@retval		false : (*this) <  v
	*/
	bool operator >= (const Vector2& v) const { return ((x*x+y*y) >= (v.x*v.x+v.y*v.y))? true : false; }
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	/*!
		@brief		設定
		@param[in]	x :
		@param[in]	y :
		@retval		参照
	*/
	Vector2& set(float x, float y)
	{
		this->x = x;
		this->y = y;
		return *this;
	}
	//------------------------------------------------------------------------------
	//	DEBUG
	//------------------------------------------------------------------------------
	void debug_print() const;
public:
	union
	{
		struct
		{
			float x, y;
		};
		float v[2];
	};
};

//----------------------------------------------------------------------------------
// 	OPERATIONS
//----------------------------------------------------------------------------------
Vector2* Vec2Add(Vector2* out, const Vector2* v1, const Vector2* v2);
Vector2* Vec2Subtract(Vector2* out, const Vector2* v1, const Vector2* v2);
Vector2* Vec2Scale(Vector2* out, const Vector2* v, float s);
Vector2* Vec2Maximize(Vector2* out, const Vector2* v1, const Vector2* v2);
Vector2* Vec2Minimize(Vector2* out, const Vector2* v1, const Vector2* v2);
float Vec2Length(const Vector2* v);
float Vec2LengthSq(const Vector2* v);
Vector2* Vec2Normalize(Vector2* out, const Vector2* v);
float Vec2OuterProduct(const Vector2* v1, const Vector2* v2);
float Vec2InnerProduct(const Vector2* v1, const Vector2* v2);
Vector2* Vec2Lerp(Vector2* out, const Vector2* v1, const Vector2* v2, float s);
Vector2* Vec2BaryCentric(Vector2* out, const Vector2* v1, const Vector2* v2, const Vector2* v3, float f, float g);

/*!
	@class	Vector3
	@brief	3d vector
	@author	M.Morimoto
	@date	2004/06/11
*/
class Vector3
{
public:
	//------------------------------------------------------------------------------
	//	CONSTRUCTOR
	//------------------------------------------------------------------------------
	Vector3(){}
	/*!
		@brief		コピーコンストラクタ
		@param[in]	v :
	*/
	Vector3(const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	/*!
		@brief		コピーコンストラクタ
		@param[in]	x :
		@param[in]	y :
		@param[in]	z :
	*/
	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	/*!
		@brief		コピーコンストラクタ
		@param[in]	v :
	*/
	Vector3(const float* v)
	{
		ASSERT_MSG(v != NULL, "Vector3::Vector3()");
		x = v[0];
		y = v[1];
		z = v[2];
	}
	//------------------------------------------------------------------------------
	//	CASTING
	//------------------------------------------------------------------------------
	/*!
		@brief		float 型ポインタにキャスト
		@retval		v[0] への float 型ポインタ
	*/
	operator float* (){ return (float*)&x; }
	/*!
		@brief		float 型ポインタにキャスト
		@retval		v[0] への float 型ポインタ
	*/
	operator const float* () const { return (const float*)&x; }
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	/*!
		@brief		代入オペレータ
		@param[in]	v :
		@retval		参照
	*/
	Vector3& operator = (const float* v)
	{
		ASSERT_MSG(v != NULL, "Vector3::operator = ()");
		x = v[0];
		y = v[1];
		z = v[2];
		return *this;
	}
	/*!
		@brief		代入オペレータ
		@param[in]	v :
		@retval		参照
	*/
	Vector3& operator = (const Vector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
	//------------------------------------------------------------------------------
	// ASSIGNMENT OPERATORS
	//------------------------------------------------------------------------------
	/*!
		@brief		加算代入オペレータ
		@param[in]	v :
		@retval		参照
	*/
	Vector3& operator += (const Vector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	/*!
		@brief		減算代入オペレータ
		@param[in]	v :
		@retval		参照
	*/
	Vector3& operator -= (const Vector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	/*!
		@brief		スケール代入オペレータ
		@param[in]	s :
		@retval		参照
	*/
	Vector3& operator *= (float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	/*!
		@brief		除算代入オペレータ
		@param[in]	s :
		@retval		参照
	*/
	Vector3& operator /= (float s)
	{
		ASSERT_MSG(s != 0.0f, "Vector3::operator /= ()");
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}
	//------------------------------------------------------------------------------
	// UNARY OPERATORS
	//------------------------------------------------------------------------------
	/*!
		@brief		正符号オペレータ
		@param[in]	s :
		@retval		+(*this)
	*/
	const Vector3 operator + () const { return *this; }
	/*!
		@brief		負符号オペレータ
		@param[in]	s :
		@retval		-(*this)
	*/
	const Vector3 operator - () const { return Vector3(-x, -y, -z); }
	//------------------------------------------------------------------------------
	// BINARY OPERATORS
	//------------------------------------------------------------------------------
	/*!
		@brief		加算オペレータ
		@param[in]	v :
		@retval		(*this) + v
	*/
	const Vector3 operator + (const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
	/*!
		@brief		減算オペレータ
		@param[in]	v :
		@retval		(*this) - v
	*/
	const Vector3 operator - (const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
	/*!
		@brief		スケールオペレータ
		@param[in]	s :
		@retval		(*this) * s
	*/
	const Vector3 operator * (float s) const { return Vector3( x * s, y * s, z * s ); }
	/*!
		@brief		除算オペレータ
		@param[in]	s :
		@retval		(*this) / s
	*/
	const Vector3 operator / (float s) const
	{
		ASSERT_MSG(s != 0.0f, "Vector3::operator / () const");
		return Vector3(x / s, y / s, z / s);
	}
	/*!
		@brief		スケールオペレータ
		@param[in]	s :
		@retval		s * v
	*/
	friend const Vector3 operator * (float s, const Vector3& v);
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : ＝
		@retval		false : ≠
	*/
	bool operator == (const Vector3& v) const { return (x == v.x && y == v.y && z == v.z); }
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : ≠
		@retval		false : ＝
	*/
	bool operator != (const Vector3& v) const { return (x != v.x || y != v.y || z != v.z); }
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : (*this) <  v
		@retval		false : (*this) >= v
	*/
	bool operator < (const Vector3& v) const { return ((x*x+y*y+z*z) < (v.x*v.x+v.y*v.y+v.z*v.z))? true : false; }
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : (*this) <= v
		@retval		false : (*this) >  v
	*/
	bool operator <= (const Vector3& v) const { return ((x*x+y*y+z*z) <= (v.x*v.x+v.y*v.y+v.z*v.z))? true : false; }
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : (*this) >  v
		@retval		false : (*this) <= v
	*/
	bool operator > (const Vector3& v) const { return ((x*x+y*y+z*z) > (v.x*v.x+v.y*v.y+v.z*v.z))? true : false; }
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : (*this) >= v
		@retval		false : (*this) <  v
	*/
	bool operator >= (const Vector3& v) const { return ((x*x+y*y+z*z) >= (v.x*v.x+v.y*v.y+v.z*v.z))? true : false; }
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	/*!
		@brief		設定
		@param[in]	x :
		@param[in]	y :
		@param[in]	z :
		@retval		参照
	*/
	Vector3& set(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		return *this;
	}
	//------------------------------------------------------------------------------
	// DEBUG
	//------------------------------------------------------------------------------
	void debug_print() const;
public:
	union
	{
		struct
		{
			float x, y, z;
		};
		float v[3];
	};
};

//----------------------------------------------------------------------------------
//	OPERATIONS
//----------------------------------------------------------------------------------
Vector3* Vec3Add(Vector3* out, const Vector3* v1, const Vector3* v2);
Vector3* Vec3Subtract(Vector3* out, const Vector3* v1, const Vector3* v2);
Vector3* Vec3Scale(Vector3* out, const Vector3* v, float s);
Vector3* Vec3Maximize(Vector3* out, const Vector3* v1, const Vector3* v2);
Vector3* Vec3Minimize(Vector3* out, const Vector3* v1, const Vector3* v2);
float Vec3Length(const Vector3* v);
float Vec3LengthSq(const Vector3* v);
Vector3* Vec3Normalize(Vector3* out, const Vector3* v);
Vector3* Vec3OuterProduct(Vector3* out, const Vector3* v1, const Vector3* v2);
float Vec3InnerProduct(const Vector3* v1, const Vector3* v2);
Vector3* Vec3Lerp(Vector3* out, const Vector3* v1, const Vector3* v2, float s);
Vector3* Vec3BaryCentric(Vector3* out, const Vector3* v1, const Vector3* v2, const Vector3* v3, float f, float g);
Vector3* Vec3OrthogonalTransform(Vector3* out, const Vector3* v, const Vector3* x, const Vector3* y, const Vector3* z);

/*!
	@class	Vector4
	@brief	4d vector
	@author	M.Morimoto
	@date	2004/06/11
*/
class Vector4
{
public:
	//------------------------------------------------------------------------------
	//	CONSTRUCTOR
	//------------------------------------------------------------------------------
	Vector4(){}
	/*!
		@brief		コピーコンストラクタ
		@param[in]	v :
	*/
	Vector4(const Vector4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}
	/*!
		@brief		コピーコンストラクタ
		@param[in]	x :
		@param[in]	y :
		@param[in]	z :
		@param[in]	w :
	*/
	Vector4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	/*!
		@brief		コピーコンストラクタ
		@param[in]	v :
	*/
	Vector4(const float* v)
	{
		ASSERT_MSG(v != NULL, "Vector4::Vector4()");
		x = v[0];
		y = v[1];
		z = v[2];
		w = v[3];
	}
	//------------------------------------------------------------------------------
	//	CASTING
	//------------------------------------------------------------------------------
	/*!
		@brief		float 型ポインタにキャスト
		@retval		v[0] への float 型ポインタ
	*/
	operator float* () { return (float*)&x; }
	/*!
		@brief		float 型ポインタにキャスト
		@retval		v[0] への float 型ポインタ
	*/
	operator const float* () const { return (const float*)&x; }
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	/*!
		@brief		代入オペレータ
		@param[in]	v :
		@retval		参照
	*/
	Vector4& operator = (const float* v)
	{
		ASSERT_MSG(v != NULL, "Vector4::operator = ()");
		x = v[0];
		y = v[1];
		z = v[2];
		w = v[3];
		return *this;
	}
	/*!
		@brief		代入オペレータ
		@param[in]	v :
		@retval		参照
	*/
	Vector4& operator = (const Vector4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}
	//------------------------------------------------------------------------------
	// ASSIGNMENT OPERATORS
	//------------------------------------------------------------------------------
	/*!
		@brief		加算代入オペレータ
		@param[in]	v :
		@retval		参照
	*/
	Vector4& operator += (const Vector4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}
	/*!
		@brief		減算代入オペレータ
		@param[in]	v :
		@retval		参照
	*/
	Vector4& operator -= (const Vector4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}
	/*!
		@brief		スケール代入オペレータ
		@param[in]	s :
		@retval		参照
	*/
	Vector4& operator *= (float s)
	{
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}
	/*!
		@brief		除算代入オペレータ
		@param[in]	s :
		@retval		参照
	*/
	Vector4& operator /= (float s)
	{
		ASSERT_MSG(s != 0.0f, "Vector4::operator /= ()");
		x /= s;
		y /= s;
		z /= s;
		w /= s;
		return *this;
	}
	//------------------------------------------------------------------------------
	// UNARY OPERATORS
	//------------------------------------------------------------------------------
	/*!
		@brief		正符号オペレータ
		@param[in]	s :
		@retval		+(*this)
	*/
	const Vector4 operator + () const { return *this; }
	/*!
		@brief		負符号オペレータ
		@param[in]	s :
		@retval		-(*this)
	*/
	const Vector4 operator - () const { return Vector4(-x, -y, -z, -w); }
	//------------------------------------------------------------------------------
	// BINARY OPERATORS
	//------------------------------------------------------------------------------
	/*!
		@brief		加算オペレータ
		@param[in]	v :
		@retval		(*this) + v
	*/
	const Vector4 operator + (const Vector4& v) const { return Vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
	/*!
		@brief		減算オペレータ
		@param[in]	v :
		@retval		(*this) - v
	*/
	const Vector4 operator - (const Vector4& v) const { return Vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
	/*!
		@brief		スケールオペレータ
		@param[in]	s :
		@retval		(*this) * s
	*/
	const Vector4 operator * (float s) const { return Vector4(x * s, y * s, z * s, w * s); }
	/*!
		@brief		除算オペレータ
		@param[in]	s :
		@retval		(*this) / s
	*/
	const Vector4 operator / (float s) const
	{
		ASSERT_MSG(s != 0.0f, "Vector4::operator / () const");
		return Vector4(x / s, y / s, z / s, w / s);
	}
	//
	/*!
		@brief		スケールオペレータ
		@param[in]	s :
		@retval		s * v
	*/
	friend const Vector4 operator * (float s, const Vector4& v);
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : ＝
		@retval		false : ≠
	*/
	bool operator == (const Vector4& v) const { return (x == v.x && y == v.y && z == v.z && w == v.w); }
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : (*this) <  v
		@retval		false : (*this) >= v
	*/
	bool operator != (const Vector4& v) const { return (x != v.x || y != v.y || z != v.z || w != v.w); }
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : (*this) <  v
		@retval		false : (*this) >= v
	*/
	bool operator < (const Vector4& v) const { return ((x*x+y*y+z*z+w*w) < (v.x*v.x+v.y*v.y+v.z*v.z+v.w*v.w))? true : false; }
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : (*this) <= v
		@retval		false : (*this) >  v
	*/
	bool operator <= (const Vector4& v) const { return ((x*x+y*y+z*z+w*w) <= (v.x*v.x+v.y*v.y+v.z*v.z+v.w*v.w))? true : false; }
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : (*this) >  v
		@retval		false : (*this) <= v
	*/
	bool
	operator > (const Vector4& v) const { return ((x*x+y*y+z*z+w*w) > (v.x*v.x+v.y*v.y+v.z*v.z+v.w*v.w))? true : false; }
	/*!
		@brief		比較オペレータ
		@param[in]	v :
		@retval		true  : (*this) >= v
		@retval		false : (*this) <  v
	*/
	bool operator >= (const Vector4& v) const { return ((x*x+y*y+z*z+w*w) >= (v.x*v.x+v.y*v.y+v.z*v.z+v.w*v.w))? true : false; }
	//------------------------------------------------------------------------------
	//------------------------------------------------------------------------------
	/*!
		@brief		設定
		@param[in]	x :
		@param[in]	y :
		@param[in]	z :
		@param[in]	w :
		@retval		参照
	*/
	Vector4& set(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
		return *this;
	}
	//------------------------------------------------------------------------------
	//	DEBUG
	//------------------------------------------------------------------------------
	void debug_print() const;
public:
	union
	{
		struct
		{
			float x, y, z, w;
		};
		float v[4];
	};
};

//----------------------------------------------------------------------------------
//	OPERATIONS
//----------------------------------------------------------------------------------
Vector4* Vec4Add(Vector4* out, const Vector4* v1, const Vector4* v2);
Vector4* Vec4Subtract(Vector4* out, const Vector4* v1, const Vector4* v2);
Vector4* Vec4Scale(Vector4* out, const Vector4* v, float s);
Vector4* Vec4Maximize(Vector4* out, const Vector4* v1, const Vector4* v2);
Vector4* Vec4Minimize(Vector4* out, const Vector4* v1, const Vector4* v2);
float Vec4Length(const Vector4* v);
float Vec4LengthSq(const Vector4* v);
Vector4* Vec4Normalize(Vector4* out, const Vector4* v);
float Vec4InnerProduct(const Vector4* v1, const Vector4* v2);
Vector4* Vec4Lerp(Vector4* out, const Vector4* v1, const Vector4* v2, float s);
Vector4* Vec4BaryCentric(Vector4* out, const Vector4* v1, const Vector4* v2, const Vector4* v3, float f, float g);

#pragma warning(default:4201)

#endif // !__VECTOR_H_
