//==================================================================================
/*!
	@file	vecmat.h
    @brief  Vector & Matrix ëÄçÏ
    @author M.Morimoto
    @date   2004/06/11
 */
//==================================================================================
#ifndef __VECMAT_H_
#define __VECMAT_H_

#include "matrix.h"

Vector3* Vec3Transform(Vector3* out, const Vector3* v, const Matrix44* m);
Vector4* Vec4Transform(Vector4* out, const Vector4* v, const Matrix44* m);
void Vec3Transform(Vector3* out, const Vector3* v, unsigned int cnt, const Matrix44* m);
void Vec4Transform(Vector4* out, const Vector4* v, unsigned int cnt, const Matrix44* m);
Matrix44* Mtx44RotationAxis(Matrix44* out, const Vector3* axis, float angle);
Matrix44* Mtx44LookAt(Matrix44* out, const Vector3* eye, const Vector3* at, const Vector3* up);
Matrix44* Mtx44View(Matrix44* out, const Vector3* eye, const Vector3* at, const Vector3* up);
Matrix44* Mtx44DropShadow(Matrix44* out, const Vector4* pln, const Vector4* lig);

#endif // !__VECMAT_H_
