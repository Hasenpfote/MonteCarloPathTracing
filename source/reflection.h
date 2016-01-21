//==============================================================================
/*!
	@file	reflection.h
	@brief	”½ŽËŒvŽZ
 */
//==============================================================================
#ifndef __REFLECTION_H_
#define __REFLECTION_H_

#include "lib/math/vector.h"

void calc_tangent_binormal(Vector3* t, Vector3* b, const Vector3* n);
void calc_reflection(Vector3* r, const Vector3* in, const Vector3* n);
void random_vector_cosweight(Vector3* v, const Vector3* n);
void random_vector_cosweight(Vector3* v, const Vector3* in, const Vector3* n, float shine);

#endif // !__REFLECTION_H_