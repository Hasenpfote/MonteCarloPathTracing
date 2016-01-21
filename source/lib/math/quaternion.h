//==================================================================================
/*!
	@file	quaternion.h
    @brief  Quaternion ‘€ì
    @author M.Morimoto
    @date   2004/06/15
 */
//==================================================================================
#ifndef __QUATERNION_H_
#define __QUATERNION_H_

#include "vector.h"

#pragma warning(disable:4201) // anonymous unions warning


/*!
	@class	Quaternion
	@brief	quaternion
	@author	M.Morimoto
	@date	2004/06/15
*/
class Quaternion
{
public:
	//----------------------------------
	//	CONSTRUCTOR
	//----------------------------------
	Quaternion(){}
	//
	Quaternion(const Quaternion& q)
	{
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
	}
	//
	Quaternion(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	//
	Quaternion(const float* q)
	{
		ASSERT_MSG(q != NULL, "Quaternion::Quaternion()");
		for(int i = 0; i < 4; i++)
			this->q[i] = q[i];
	}
	//----------------------------------
	//	CASTING
	//----------------------------------
	operator float* () { return (float*)&x; }
	//
	operator const float* () const { return (const float*)&x; }
	//----------------------------------
	//
	//----------------------------------
	Quaternion& operator = (const float* q)
	{
		ASSERT_MSG(q != NULL, "Quaternion::operator = ()");
		for(int i = 0; i < 4; i++)
			this->q[i] = q[i];
		return *this;
	}
	//
	Quaternion& operator = (const Quaternion& q)
	{
		for(int i = 0; i < 4; i++)
			this->q[i] = q.q[i];
		return *this;
	}
	//----------------------------------
	// ASSIGNMENT OPERATORS
	//----------------------------------
	Quaternion& operator += (const Quaternion& q) { return *this; }
	//
	Quaternion& operator -= (const Quaternion& q) { return *this; }
	//
	Quaternion& operator *= (float s)
	{
		for(int i = 0; i < 4; i++)
			q[i] *= s;
		return *this;
	}
	//----------------------------------
	// UNARY OPERATORS
	//----------------------------------
	const Quaternion operator + () const { return *this; }
	//
	const Quaternion operator - () const { return Quaternion(-x, -y, -z, -w); }
	//----------------------------------
	//
	//----------------------------------
	Quaternion& set(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
		return *this;
	}
	//----------------------------------
	//	DEBUG
	//----------------------------------
	void debug_print() const;
public:
	union
	{
		struct
		{
			float x, y, z, w;
		};
		struct
		{
			Vector3 v;
			float   s;
		};
		float q[4];
	};
};
//--------------------------------------
//	OPERATIONS
//--------------------------------------
static inline Quaternion* QuatZero( Quaternion* q )
{
	q->x = q->y = q->z = q->w = 0.0f;
	return q;
}

static inline Quaternion* QuatIdentity( Quaternion* q )
{
	q->x = q->y = q->z = 0.0f;
	q->w = 1.0f;
	return q;
}

static inline Quaternion* QuatCnj(Quaternion* out, const Quaternion* q)
{
	for(int i = 0; i < 3; i++)
		out->q[i] = -q->q[i];
	return out;
}

static inline Quaternion* QuatAbs(Quaternion* out, const Quaternion* q)
{
	for(int i = 0; i < 4; i++)
		out->q[i] = fabsf(q->q[i]);
	return out;
}

static inline Quaternion* QuatMul(Quaternion* out, const Quaternion* q1, const Quaternion* q2)
{
	out->x = q1->w * q2->x + q1->x * q2->w + q1->y * q2->z - q1->z * q2->y;
	out->y = q1->w * q2->y + q1->y * q2->w + q1->z * q2->x - q1->x * q2->z;
	out->z = q1->w * q2->z + q1->z * q2->w + q1->x * q2->y - q1->y * q2->x;
	out->w = q1->w * q2->w - q1->x * q2->x - q1->y * q2->y - q1->z * q2->z;
	return out;
}

static inline float QuatInnerProduct(const Quaternion* q1, const Quaternion* q2)
{
	return q1->x * q2->x + q1->y * q2->y + q1->z * q2->z + q1->w * q2->w;
}

static inline float QuatLengthSq(const Quaternion* q)
{
	return q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w;
}

static inline float QuatLength(const Quaternion* q)
{
	return sqrtf(QuatLengthSq(q));
}

static inline float QuatInverse(Quaternion* out, const Quaternion* q)
{
	float l, m;

	l = QuatLength(q);

	if(fabsf(l) < FLT_EPSILON)
	{
		out->x = out->y = out->z = 0.0f;
		out->w = 1.0f;
	}
	else
	{
		m = 1.0f / l;
		out->x = -out->x * m;
		out->y = -out->y * m;
		out->z = -out->z * m;
		out->w =  out->w * m;
	}
}

static inline float QuatNormalize(Quaternion* out, const Quaternion* q)
{
	float l, m;

	l = QuatLength(q);

	if(fabsf(l) < FLT_EPSILON)
	{
		out->x = out->y = out->z = 0.0f;
		out->w = 1.0f;
	}
	else
	{
		m = 1.0f / l;
		for(int i = 0; i < 4; i++)
		{
			out->q[i] = out->q[i] * m;
		}
	}
}


#pragma warning(default:4201)

#endif // !__QUATERNION_H_




#if 0
00075 void
00076 lib3ds_quat_axis_angle(Lib3dsQuat c, Lib3dsVector axis, Lib3dsFloat angle)
00077 {
00078   Lib3dsDouble omega,s;
00079   Lib3dsDouble l;
00080
00081   l=sqrt(axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2]);
00082   if (l<LIB3DS_EPSILON) {
00083     c[0]=c[1]=c[2]=0.0f;
00084     c[3]=1.0f;
00085   }
00086   else {
00087     omega=-0.5*angle;
00088     s=sin(omega)/l;
00089     c[0]=(Lib3dsFloat)s*axis[0];
00090     c[1]=(Lib3dsFloat)s*axis[1];
00091     c[2]=(Lib3dsFloat)s*axis[2];
00092     c[3]=(Lib3dsFloat)cos(omega);
00093   }
00094 }
00241 void
00242 lib3ds_quat_ln(Lib3dsQuat c)
00243 {
00244   Lib3dsDouble om,s,t;
00245
00246   s=sqrt(c[0]*c[0] + c[1]*c[1] + c[2]*c[2]);
00247   om=atan2(s,c[3]);
00248   if (fabs(s)<LIB3DS_EPSILON) {
00249     t=0.0f;
00250   }
00251   else {
00252     t=om/s;
00253   }
00254   {
00255     int i;
00256     for (i=0; i<3; ++i) {
00257       c[i]=(Lib3dsFloat)(c[i]*t);
00258     }
00259     c[3]=0.0f;
00260   }
00261 }
00267 void
00268 lib3ds_quat_ln_dif(Lib3dsQuat c, Lib3dsQuat a, Lib3dsQuat b)
00269 {
00270   Lib3dsQuat invp;
00271
00272   lib3ds_quat_copy(invp, a);
00273   lib3ds_quat_inv(invp);
00274   lib3ds_quat_mul(c, invp, b);
00275   lib3ds_quat_ln(c);
00276 }
00282 void
00283 lib3ds_quat_exp(Lib3dsQuat c)
00284 {
00285   Lib3dsDouble om,sinom;
00286
00287   om=sqrt(c[0]*c[0] + c[1]*c[1] + c[2]*c[2]);
00288   if (fabs(om)<LIB3DS_EPSILON) {
00289     sinom=1.0f;
00290   }
00291   else {
00292     sinom=sin(om)/om;
00293   }
00294   {
00295     int i;
00296     for (i=0; i<3; ++i) {
00297       c[i]=(Lib3dsFloat)(c[i]*sinom);
00298     }
00299     c[3]=(Lib3dsFloat)cos(om);
00300   }
00301 }
00307 void
00308 lib3ds_quat_slerp(Lib3dsQuat c, Lib3dsQuat a, Lib3dsQuat b, Lib3dsFloat t)
00309 {
00310   Lib3dsDouble l;
00311   Lib3dsDouble om,sinom;
00312   Lib3dsDouble sp,sq;
00313   Lib3dsQuat q;
00314
00315   l=a[0]*b[0] + a[1]*b[1] + a[2]*b[2] + a[3]*b[3];
00316   if ((1.0+l)>LIB3DS_EPSILON) {
00317     if (fabs(l)>1.0f) l/=fabs(l);
00318     om=acos(l);
00319     sinom=sin(om);
00320     if (fabs(sinom)>LIB3DS_EPSILON) {
00321       sp=sin((1.0f-t)*om)/sinom;
00322       sq=sin(t*om)/sinom;
00323     }
00324     else {
00325       sp=1.0f-t;
00326       sq=t;
00327     }
00328     c[0]=(Lib3dsFloat)(sp*a[0] + sq*b[0]);
00329     c[1]=(Lib3dsFloat)(sp*a[1] + sq*b[1]);
00330     c[2]=(Lib3dsFloat)(sp*a[2] + sq*b[2]);
00331     c[3]=(Lib3dsFloat)(sp*a[3] + sq*b[3]);
00332   }
00333   else {
00334     q[0]=-a[1];
00335     q[1]=a[0];
00336     q[2]=-a[3];
00337     q[3]=a[2];
00338     sp=sin((1.0-t)*LIB3DS_HALFPI);
00339     sq=sin(t*LIB3DS_HALFPI);
00340     c[0]=(Lib3dsFloat)(sp*a[0] + sq*q[0]);
00341     c[1]=(Lib3dsFloat)(sp*a[1] + sq*q[1]);
00342     c[2]=(Lib3dsFloat)(sp*a[2] + sq*q[2]);
00343     c[3]=(Lib3dsFloat)(sp*a[3] + sq*q[3]);
00344   }
00345 }
00351 void
00352 lib3ds_quat_squad(Lib3dsQuat c, Lib3dsQuat a, Lib3dsQuat p, Lib3dsQuat q,
00353   Lib3dsQuat b, Lib3dsFloat t)
00354 {
00355   Lib3dsQuat ab;
00356   Lib3dsQuat pq;
00357
00358   lib3ds_quat_slerp(ab,a,b,t);
00359   lib3ds_quat_slerp(pq,p,q,t);
00360   lib3ds_quat_slerp(c,ab,pq,2*t*(1-t));
00361 }
00367 void
00368 lib3ds_quat_tangent(Lib3dsQuat c, Lib3dsQuat p, Lib3dsQuat q, Lib3dsQuat n)
00369 {
00370   Lib3dsQuat dn,dp,x;
00371   int i;
00372
00373   lib3ds_quat_ln_dif(dn, q, n);
00374   lib3ds_quat_ln_dif(dp, q, p);
00375
00376   for (i=0; i<4; i++) {
00377     x[i]=-1.0f/4.0f*(dn[i]+dp[i]);
00378   }
00379   lib3ds_quat_exp(x);
00380   lib3ds_quat_mul(c,q,x);
00381 }
#endif