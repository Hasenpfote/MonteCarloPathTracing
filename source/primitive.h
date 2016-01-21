//==============================================================================
/*!
	@file	primitive.h
	@brief	形状
    @author M.Morimoto
    @date   2010/03/17
 */
//==============================================================================
#ifndef __PRIMITIVE_H_
#define __PRIMITIVE_H_

#include <list>
#include "lib/math/vector.h"
#include "geometry.h"
#include "material.h"
#include "ray.h"

/*!
	@brief	プリミティブ
	@class	Primitive
	@note	abstract class
 */
class Primitive
{
public:
	struct Param
	{
		float t, u, v;
	};
public:
	virtual bool Intersect(const AABB& aabb) = 0;
	virtual bool Intersect(Param& param, const Ray& ray) = 0;
	virtual void CalcVertex(Vertex& v, const Param& param, const Ray& ray) = 0;
	virtual void CalcRange(float& min, float& max, Axis axis) = 0;

	void SetMaterial(Material* mtrl){ ref_mtrl = mtrl; }
	Material* GetMaterial(){ return ref_mtrl; }

	virtual std::size_t GetNumPoints() = 0;
	virtual float GetPoint(Axis axis, std::size_t idx) = 0;

private:
	Material*	ref_mtrl;
};

/*!
	@brief	球
	@class	Sphere
 */
class Sphere : public Primitive
{
public:
	bool Intersect(const AABB& aabb);
	bool Intersect(Param& param, const Ray& ray);
	void CalcVertex(Vertex& v, const Param& param, const Ray& ray);
	void CalcRange(float& min, float& max, Axis axis);

	std::size_t GetNumPoints(){ return 2; }
	float GetPoint(Axis axis, std::size_t idx){ return (idx == 0)? (p.v[axis] - r) : (p.v[axis] + r); }

public:
	Vector3	p;		//!< position
	float	r;		//!< radius
};

/*!
	@brief	三角形
	@class	Triangle
 */
class Triangle : public Primitive
{
public:
	bool Intersect(const AABB& aabb);
	bool Intersect(Param& param, const Ray& ray);
	void CalcVertex(Vertex& v, const Param& param, const Ray& ray);
	void CalcRange(float& min, float& max, Axis axis);

	std::size_t GetNumPoints(){ return 3; }
	float GetPoint(Axis axis, std::size_t idx){ return v[idx].p.v[axis]; }

public:
	Vertex	v[3];	//!< vertices
	Vector3	n;		//!< face normal
};

typedef std::list<Primitive*> PrimitiveList;

#endif // !__PRIMITIVE_H_