//==================================================================================
/*!
	@file	math.h
    @brief  ���w�S��
    @author M.Morimoto
    @date   2004/06/11
 */
//==================================================================================
#ifndef __MATH_H_
#define __MATH_H_

#include <math.h>
#include <float.h>

//
#ifndef PI
#define PI	3.1415926535897932384626433832795f			//!< Pi
#endif // !PI
//
#ifndef PI2
#define PI2 6.28318530717958647692528676655901f			//!< 2Pi
#endif // !PI2
//
#ifndef PI_DIV2
#define PI_DIV2 1.57079632679489661923132169163975f		//!< Pi/2
#endif // !PI_DIV2
//
#ifndef PI_DIV4
#define PI_DIV4 0.785398163397448309615660845819876f	//!< Pi/4
#endif // !PI_DIV4

//----------------------------------------------------------------------------------
/*!
	@brief		�p�x�����W�A���ɕϊ�
	@param[in]	deg : �p�x
	@retval		�ϊ�����
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
static inline float DegToRad(float deg){ return deg * PI / 180.0f; }

//----------------------------------------------------------------------------------
/*!
	@brief		���W�A�����p�x�ɕϊ�
	@param[in]	rad : ���W�A��
	@retval		�ϊ�����
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
static inline float RadToDeg(float rad){ return rad * 180.0f / PI; }

#endif // !__MATH_H_
