//==============================================================================
/*!
	@file	material.h
	@brief	çﬁéø
    @author M.Morimoto
    @date   2010/03/17
 */
//==============================================================================
#ifndef __MATERIAL_H_
#define __MATERIAL_H_

#include "lib/color/color.h"

/*!
	@brief	çﬁéø
	@class	Material
 */
class Material
{
public:
	Color	pd;		//!< diffuse reflectance
	Color	ps;		//!< specular reflectance
	Color	e;		//!< emittance
	float	kd;		//!< diffuse coefficient 
	float	ks;		//!< specular coefficient 
	float	shine;
};

typedef std::list<Material*> MaterialList;

#endif // !__MATERIAL_H_