//==============================================================================
/*!
	@file	light.h
	@brief	ŒõŒ¹
    @author M.Morimoto
    @date   2010/03/17
 */
//==============================================================================
#ifndef __LIGHT_H_
#define __LIGHT_H_

#include <list>
#include "lib/math/vector.h"
#include "lib/color/color.h"
#include "material.h"
#include "geometry.h"
#include "ray.h"

class Light
{
public:
	enum Type
	{
		Type_Point,
		Type_Directional
	};
public:
	void Lighting(Color& out, const Ray& ray, const Vertex& v, const Material& mtrl);

public:
	Type	type;
	Vector3	pos;
	Vector3	dir;
	Color	intensity;
};

typedef std::list<Light*> LightList;

#endif // !__LIGHT_H_