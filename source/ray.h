//==============================================================================
/*!
	@file	ray.h
	@brief	Œõü
    @author M.Morimoto
    @date   2010/03/17
 */
//==============================================================================
#ifndef __RAY_H_
#define __RAY_H_

#include "lib/math/vector.h"

/*!
	@brief	Œõü
	@class	Ray
 */
class Ray
{
public:
	Vector3	org;	//!< origin
	Vector3	dir;	//!< direction
};

#endif // !__RAY_H_