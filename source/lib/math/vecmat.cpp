
#include "vecmat.h"

//----------------------------------------------------------------------------------
/*!
	@brief		平行移動変換
	@param[o]	out: 出力
	@param[i]	v: 座標
	@param[i]	m: 平行移動行列
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector3* Vec3Transform(Vector3* out, const Vector3* v, const Matrix44* m)
{
	ASSERT_MSG(out != NULL, "Vec3Transform()");
	ASSERT_MSG(v   != NULL, "Vec3Transform()");
	ASSERT_MSG(m   != NULL, "Vec3Transform()");

	float x, y, z;

	x = v->x * m->_11 + v->y * m->_21 + v->z * m->_31 + m->_41;
	y = v->x * m->_12 + v->y * m->_22 + v->z * m->_32 + m->_42;
	z = v->x * m->_13 + v->y * m->_23 + v->z * m->_33 + m->_43;
//	w = v->x * m->_14 + v->y * m->_24 + v->z * m->_34 + m->_44;
	out->set( x, y, z );

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		平行移動変換
	@param[o]	out: 出力
	@param[i]	v: 座標
	@param[i]	m: 平行移動行列
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Vector4* Vec4Transform(Vector4* out, const Vector4* v, const Matrix44* m)
{
	ASSERT_MSG(out != NULL, "Vec4Transform()");
	ASSERT_MSG(v   != NULL, "Vec4Transform()");
	ASSERT_MSG(m   != NULL, "Vec4Transform()");

	float x, y, z, w;

	x = v->x * m->_11 + v->y * m->_21 + v->z * m->_31 + v->w * m->_41;
	y = v->x * m->_12 + v->y * m->_22 + v->z * m->_32 + v->w * m->_42;
	z = v->x * m->_13 + v->y * m->_23 + v->z * m->_33 + v->w * m->_43;
	w = v->x * m->_14 + v->y * m->_24 + v->z * m->_34 + v->w * m->_44;
	out->set( x, y, z, w );

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		平行移動変換
	@param[o]	out: 出力
	@param[i]	v: 座標群
	@param[i]	cnt: 座標数
	@param[i]	m: 平行移動行列
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
void Vec3Transform(Vector3* out, const Vector3* v, unsigned int cnt, const Matrix44* m)
{
	ASSERT_MSG(out != NULL, "Vec3Transform()");
	ASSERT_MSG(v   != NULL, "Vec3Transform()");
	ASSERT_MSG(m   != NULL, "Vec3Transform()");

	float x, y, z;

	for(unsigned int i = 0; i < cnt; i++)
	{
		x = v[i].x * (*m)(0,0) + v[i].y * (*m)(1,0) + v[i].z * (*m)(2,0) + (*m)(3,0);
		y = v[i].x * (*m)(0,1) + v[i].y * (*m)(1,1) + v[i].z * (*m)(2,1) + (*m)(3,1);
		z = v[i].x * (*m)(0,2) + v[i].y * (*m)(1,2) + v[i].z * (*m)(2,2) + (*m)(3,2);
		out[i].set(x, y, z);

		v++;
		out++;
	}
}

//----------------------------------------------------------------------------------
/*!
	@brief		平行移動変換
	@param[o]	out: 出力
	@param[i]	v: 座標群
	@param[i]	cnt: 座標数
	@param[i]	m: 平行移動行列
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
void Vec4Transform(Vector4* out, const Vector4* v, unsigned int cnt, const Matrix44* m)
{
	ASSERT_MSG(out != NULL, "Vec4Transform()");
	ASSERT_MSG(v   != NULL, "Vec4Transform()");
	ASSERT_MSG(m   != NULL, "Vec4Transform()");

	float x, y, z, w;

	for(unsigned int i = 0; i < cnt; i++)
	{
		x = v[i].x * (*m)(0,0) + v[i].y * (*m)(1,0) + v[i].z * (*m)(2,0) + v[i].w * (*m)(3,0);
		y = v[i].x * (*m)(0,1) + v[i].y * (*m)(1,1) + v[i].z * (*m)(2,1) + v[i].w * (*m)(3,1);
		z = v[i].x * (*m)(0,2) + v[i].y * (*m)(1,2) + v[i].z * (*m)(2,2) + v[i].w * (*m)(3,2);
		w = v[i].x * (*m)(0,3) + v[i].y * (*m)(1,3) + v[i].z * (*m)(2,3) + v[i].w * (*m)(3,3);
		out[i].set(x, y, z, w);

		v++;
		out++;
	}
}

//----------------------------------------------------------------------------------
/*!
	@brief		任意回転行列
	@param[o]	out: 出力
	@param[i]	axis: 軸
	@param[i]	angle: 角度(ラジアン)
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44RotationAxis(Matrix44* out, const Vector3* axis, float angle)
{
	ASSERT_MSG(out  != NULL, "Mtx44RotationAxis()");
	ASSERT_MSG(axis != NULL, "Mtx44RotationAxis()");

	float	nx, ny, nz;
	float	nx2,ny2,nz2;
	float	costheta, sintheta;
	float	nxyc, nxzc, nyzc;
	float	nxs, nys, nzs;

	nx = axis->x;
	ny = axis->y;
	nz = axis->z;

	nx2 = nx * nx;
	ny2 = ny * ny;
	nz2 = nz * nz;

	costheta = cosf( angle );
	sintheta = sinf( angle );

	nxyc = nx * ny - nx * ny * costheta;
	nxzc = nx * nz - nx * nz * costheta;
	nyzc = ny * nz - ny * nz * costheta;

	nxs = nx * sintheta;
	nys = ny * sintheta;
	nzs = nz * sintheta;

	out->_11 = nx2  + ( 1.0f - nx2 ) * costheta;
	out->_21 = nxyc - nzs;
	out->_31 = nxzc + nys;

	out->_12 = nxyc + nzs;
	out->_22 = ny2  + ( 1.0f - ny2 ) * costheta;
	out->_32 = nyzc - nxs;

	out->_13 = nxzc - nys;
	out->_23 = nyzc + nxs;
	out->_33 = nz2  + ( 1.0f - nz2 ) * costheta;

	out->_14 = out->_24 = out->_34 =
	out->_41 = out->_42 = out->_43 = 0.0f;
	out->_44 = 1.0f;

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		注視点行列
	@param[o]	out: 出力
	@param[i]	eye: 視点
	@param[i]	at: 注視点
	@param[i]	up: 上方向
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44LookAt(Matrix44* out, const Vector3* eye, const Vector3* at, const Vector3* up)
{
	ASSERT_MSG(out != NULL, "Mtx44LookAt()");
	ASSERT_MSG(eye != NULL, "Mtx44LookAt()");
	ASSERT_MSG(at  != NULL, "Mtx44LookAt()");
	ASSERT_MSG(up  != NULL, "Mtx44LookAt()");

	Vector3 v, u, r;	// view / up / right
	float	dot;

	Vec3Subtract(&v, at, eye);
	ASSERT_MSG(Vec3LengthSq(&v) > FLT_MIN, "Mtx44LookAt()");
	Vec3Normalize(&v, &v);

	dot = Vec3InnerProduct(up, &v);
	Vec3Scale(&u, &v, dot);
	Vec3Subtract(&u, up, &u);

	if(Vec3LengthSq(&u) < FLT_MIN)
	{
		Vector3 temp;
		u.set(0.0f, 1.0f, 0.0f);
		Vec3Scale(&temp, &v, v.y);
		Vec3Subtract(&u, &u, &temp);
		if(Vec3LengthSq( &u ) < FLT_MIN)
		{
			u.set(0.0f, 0.0f, 1.0f);
			Vec3Scale(&temp, &v, v.z);
			Vec3Subtract(&u, &u, &temp);

			if(Vec3LengthSq(&u) < FLT_MIN)
				return NULL;
		}
	}
	Vec3Normalize(&u, &u);

	Vec3OuterProduct(&r, &u, &v);

	out->_11 = r.x; out->_12 = r.y; out->_13 = r.z; out->_14 = 0.0f;
	out->_21 = u.x; out->_22 = u.y; out->_23 = u.z; out->_24 = 0.0f;
	out->_31 = v.x; out->_32 = v.y; out->_33 = v.z; out->_34 = 0.0f;
	out->_41 = eye->x;
	out->_42 = eye->y;
	out->_43 = eye->z;
	out->_44 = 1.0f;

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		視点行列
	@param[o]	out: 出力
	@param[i]	eye: 視点
	@param[i]	at: 注視点
	@param[i]	up: 上方向
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44View(Matrix44* out, const Vector3* eye, const Vector3* at, const Vector3* up)
{
	ASSERT_MSG(out != NULL, "Mtx44View()");
	ASSERT_MSG(eye != NULL, "Mtx44View()");
	ASSERT_MSG(at  != NULL, "Mtx44View()");
	ASSERT_MSG(up  != NULL, "Mtx44View()");

	Mtx44LookAt(out, eye, at, up);
	Mtx44Transpose(out, out);

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		投影行列
	@param[o]	out: 出力
	@param[i]	pln: 平面
	@param[i]	lig: 光源
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44DropShadow(Matrix44* out, const Vector4* pln, const Vector4* lig)
{
	const float dot = Vec4InnerProduct(pln, lig);

	out->_11 = dot - lig->x * pln->x;
	out->_21 =		-lig->x * pln->y;
	out->_31 =		-lig->x * pln->z;
	out->_41 =		-lig->x * pln->w;

	out->_12 =		-lig->y * pln->x;
	out->_22 = dot - lig->y * pln->y;
	out->_32 =		-lig->y * pln->z;
	out->_42 =		-lig->y * pln->w;

	out->_13 =		-lig->z * pln->x;
	out->_23 =		-lig->z * pln->y;
	out->_33 = dot - lig->z * pln->z;
	out->_43 =		-lig->z * pln->w;

	out->_14 =		-lig->w * pln->x;
	out->_24 =		-lig->w * pln->y;
	out->_34 = 		-lig->w * pln->z;
	out->_44 = dot - lig->w * pln->w;

	return out;
}
