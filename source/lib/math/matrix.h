//==================================================================================
/*!
	@file	matrix.h
    @brief  Matrix 操作
    @author M.Morimoto
    @date   2004/06/11
 */
//==================================================================================
#ifndef __MATRIX_H_
#define __MATRIX_H_

#include "vector.h"

#pragma warning(disable:4201) // anonymous unions warning

/*!
	@class	Matrix44
	@brief	4x4 Matrix
	@author	M.Morimoto
	@date	2004/06/11
*/
class Matrix44
{
public:
	//------------------------------------------------------------------------------
	//	CONSTRUCTOR
	//------------------------------------------------------------------------------
	Matrix44(){}
	//
	Matrix44(const Matrix44& m);
	//
	Matrix44(const float* m);
	//
	Matrix44(float _11, float _12, float _13, float _14,
			 float _21, float _22, float _23, float _24,
			 float _31, float _32, float _33, float _34,
			 float _41, float _42, float _43, float _44);

	//------------------------------------------------------------------------------
	// ACCESS GRANTS
	//------------------------------------------------------------------------------
	/*!
		@brief		行列成分取得
		@param[i]	row: 行
		@param[i]	col: 列
		@retval		参照
		@author		M.Morimoto
		@note		mtx[row][column]
		@date		2004/06/11
	*/
	float& operator () (unsigned int row, unsigned int col)
	{
		ASSERT_MSG(row < 4, "Matrix44::operator () ()");
		ASSERT_MSG(col < 4, "Matrix44::operator () ()");
		return m2[row][col];
	}
	/*!
		@brief		行列成分取得(READ ONLY)
		@param[i]	row: 行
		@param[i]	col: 列
		@retval		参照
		@author		M.Morimoto
		@note		mtx[row][column]
		@date		2004/06/11
	*/
	float operator () (unsigned int row, unsigned int col) const
	{
		ASSERT_MSG(row < 4, "Matrix44::operator () ()");
		ASSERT_MSG(col < 4, "Matrix44::operator () ()");
		return m2[row][col];
	}
	/*!
		@brief		行ベクトル取得
		@param[i]	row: 行
		@retval		参照
		@author		M.Morimoto
		@note		mtx[row]
		@date		2004/06/11
	*/
	Vector3& row_vector3(unsigned int row)
	{
		ASSERT_MSG(row < 4, "row_vector3() : 'row' out of range");
//		return (Vector3&)m2[row];
		return *((Vector3*)m2[row]);
	}
	/*!
		@brief		行ベクトル取得(READ ONLY)
		@param[i]	row: 行
		@retval		参照
		@author		M.Morimoto
		@note		mtx[row]
		@date		2004/06/11
	*/
	const Vector3 row_vector3(unsigned int row) const
	{
		ASSERT_MSG(row < 4, "row_vector3() const: 'row' out of range");
		return *((Vector3*)m2[row]);
	}
	/*!
		@brief		行ベクトル取得
		@param[i]	row: 行
		@retval		参照
		@author		M.Morimoto
		@note		mtx[row]
		@date		2004/06/11
	*/
	Vector4& row_vector4(unsigned int row)
	{
		ASSERT_MSG(row < 4, "row_vector4() : 'row' out of range");
//		return (Vector4&)m2[row];
		return *((Vector4*)m2[row]);
	}
	/*!
		@brief		行ベクトル取得[READ ONLY]
		@param[i]	row: 行
		@retval		参照
		@author		M.Morimoto
		@note		mtx[row]
		@date		2004/06/11
	*/
	const Vector4 row_vector4(unsigned int row) const
	{
		ASSERT_MSG(row < 4, "row_vector4() const: 'row' out of range");
		return *((Vector4*)m2[row]);
	}
	//------------------------------------------------------------------------------
	// CASTING
	//------------------------------------------------------------------------------
	/*!
		@brief		float 型ポインタにキャスト
		@retval		mtx[0][0] への float 型ポインタ
		@author		M.Morimoto
		@date		2004/06/11
	*/
	operator float* (){ return m1; }
	/*!
		@brief		float 型ポインタにキャスト[READ ONLY]
		@retval		mtx[0][0] への float 型ポインタ
		@author		M.Morimoto
		@date		2004/06/11
	*/
	operator const float* () const { return m1; }
	//------------------------------------------------------------------------------
	// ASSIGNMENT OPERATORS
	//------------------------------------------------------------------------------
	Matrix44& operator  = (const Matrix44& m);
	//
	Matrix44& operator += (const Matrix44& m);
	//
	Matrix44& operator -= (const Matrix44& m);
	//
	Matrix44& operator *= (const Matrix44& m);
	//
	Matrix44& operator *= (float s);
	//------------------------------------------------------------------------------
	// UNARY OPERATORS
	//------------------------------------------------------------------------------

	//------------------------------------------------------------------------------
	// BINARY OPERATORS
	//------------------------------------------------------------------------------
	const Matrix44 operator + (const Matrix44& m) const;
	//
	const Matrix44 operator - (const Matrix44& m) const;
	//
	const Matrix44 operator * (const Matrix44& m) const;
	//
	const Matrix44 operator * (float s) const;
	//
	friend const Matrix44 operator * (float s, const Matrix44& m);
	//
	bool operator == (const Matrix44& m) const;
	//
	bool operator != (const Matrix44& m) const;
	//------------------------------------------------------------------------------
	//	DEBUG
	//------------------------------------------------------------------------------
	void debug_print() const;
public:
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float	m1[16];
		float	m2[4][4];
	};
};

//----------------------------------------------------------------------------------
//	OPERATIONS
//----------------------------------------------------------------------------------
Matrix44* Mtx44Zero(Matrix44* m);
Matrix44* Mtx44Identity(Matrix44* m);
bool Mtx44IsIdentity(const Matrix44* m);
Matrix44* Mtx44Add(Matrix44* out, const Matrix44* m1, const Matrix44* m2);
Matrix44* Mtx44Subtract(Matrix44* out, const Matrix44* m1, const Matrix44* m2);
Matrix44* Mtx44Mul(Matrix44* out, const Matrix44* m1, const Matrix44* m2);
Matrix44* Mtx44Scale(Matrix44* out, const Matrix44* m, float s);
Matrix44* Mtx44Transpose(Matrix44* out, const Matrix44* m);
Matrix44* Mtx44Inverse(Matrix44* out, const Matrix44* m);
Matrix44* Mtx44Translation(Matrix44* out, float x, float y, float z);
Matrix44* Mtx44Scaling(Matrix44* out, float x, float y, float z);
Matrix44* Mtx44RotationX(Matrix44* out, float theta);
Matrix44* Mtx44RotationY(Matrix44* out, float theta);
Matrix44* Mtx44RotationZ(Matrix44* out, float theta);
Matrix44* Mtx44RotationYawPitchRoll(Matrix44* out, float yaw, float pitch, float roll);
Matrix44* Mtx44Frustum(Matrix44* out, float t, float b, float l, float r, float n, float f);
Matrix44* Mtx44Perspective(Matrix44* out, float fovy, float aspect, float n, float f);
Matrix44* Mtx44Ortho(Matrix44* out, float t, float b, float l, float r, float n, float f);
Matrix44* Mtx44AdjProjection(Matrix44* out, float fovy, float aspect, float n, float f, float pixdx, float pixdy, float vpw, float vph);
Matrix44* Mtx44LightingFrustum(Matrix44* out, float t, float b, float l, float r, float n, float scaleS, float scaleT, float transS, float transT);
Matrix44* Mtx44LightingPerspective(Matrix44* out, float fovy, float aspect, float scaleS, float scaleT, float transS, float transT);
Matrix44* Mtx44LightingOrtho( Matrix44* out, float t, float b, float l, float r, float scaleS, float scaleT, float transS, float transT);


#pragma warning(default:4201)

#endif // !__MATRIX_H_
