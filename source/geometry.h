
#ifndef __GEOMETRY_H_
#define __GEOMETRY_H_

#include "lib/math/vector.h"

/*!
	@brief	Ž²
	@enum	Axis
 */
enum Axis
{
	Axis_X,		//!< x-axis
	Axis_Y,		//!< y-axis
	Axis_Z,		//!< z-axis
	Axis_Max
};

/*!
	@berif	’¸“_
	@class	Vertex
 */
class Vertex
{
public:
	Vector3	p;	//!< position
	Vector3	n;	//!< normal
};

/*!
	@brief	Axis Aligned Bounding Box
	@class	AABB
 */
class AABB
{
public:
	bool Intersect(const AABB& aabb) const;
	bool Intersect(Vector3& out, const Vector3& org, const Vector3& dir) const;
	bool Intersect(float& t_near, float& t_far, const Vector3& org, const Vector3& dir) const;
	bool Contains(const Vector3& pos) const;
	Vector3 GetSize() const { return max - min; }

public:
	Vector3 min;
	Vector3 max;
};

/*!
	@brief	•½–Ê
	@class	Plane
 */
class Plane
{
public:
	union
	{
		struct
		{
			Vector3 n;
			float	d;
		};
		float v[4];
	};
};

#endif // !__GEOMETRY_H_