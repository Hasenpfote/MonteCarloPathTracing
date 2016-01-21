
#include "geometry.h"


bool AABB::Intersect(const AABB& aabb) const
{
	if((aabb.min.x > max.x) || (aabb.max.x < min.x)
	 ||(aabb.min.y > max.y) || (aabb.max.y < min.y)
	 ||(aabb.min.z > max.z) || (aabb.max.z < min.z))
		return false;
	return true;
}

/*!
	@brief		光線との交差判定
	@param[o]	out: 交差位置
	@prram[i]	org: 基点
	@prram[i]	dir: 方向
	@retval		true: 交差している
	@retval		false: 交差していない
	@note		Fast Ray-Box Intersection
				by Andrew Woo
				from "Graphics Gems", Academic Press, 1990
 */
bool AABB::Intersect(Vector3& out, const Vector3& org, const Vector3& dir) const
{
	enum Quadrant
	{
		Quadrant_Left,
		Quadrant_Middle,
		Quadrant_Right,
	};
	bool inside = true;
	Quadrant quadrant[3];		// 各軸の象限
	float candidate_plane[3];	// 各軸の候補面
	float max_t[3];				// 
	int i;

	// Find candidate planes; this loop can be avoided if
	// rays cast all from the eye(assume perpsective view)
	for(i = 0; i < 3; i++)
	{
		if(org.v[i] < min.v[i])
		{
			quadrant[i] = Quadrant_Left;
			candidate_plane[i] = min.v[i];
			inside = false;
		}
		else
		if(org.v[i] > max.v[i])
		{
			quadrant[i] = Quadrant_Right;
			candidate_plane[i] = max.v[i];
			inside = false;
		}
		else
		{
			quadrant[i] = Quadrant_Middle;
		}
	}
	// Ray origin inside bounding box
	if(inside)
	{
		out = org;
		return true;
	}

	// Calculate T distances to candidate planes
	for(i = 0; i < 3; i++)
		if(quadrant[i] != Quadrant_Middle && dir.v[i] != 0.0f)
			max_t[i] = (candidate_plane[i] - org.v[i]) / dir.v[i];
		else
			max_t[i] = -1.0f;

	// Get largest of the maxT's for final choice of intersection
	int which_plane = 0;
	for(i = 1; i < 3; i++)
		if (max_t[which_plane] < max_t[i])
			which_plane = i;

	// Check final candidate actually inside box
	if(max_t[which_plane] < 0.0f)
		return false;
	for(i = 0; i < 3; i++)
	{
		if(which_plane != i)
		{
			out.v[i] = org.v[i] + max_t[which_plane] * dir.v[i];
			if(out.v[i] < min.v[i] || out.v[i] > max.v[i])
				return false;
		}
		else
		{
			out.v[i] = candidate_plane[i];
		}
	}
	return true;
}

/*!
	@brief		セグメントとの交差
	@param[o]	t_near: 光線方向の近点パラメータ
	@param[o]	t_far: 光線方向の遠点パラメータ
	@param[i]	org: 光線の基点
	@param[i]	dir: 光線の向き
 */
bool AABB::Intersect(float& t_near, float& t_far, const Vector3& org, const Vector3& dir) const
{
	t_near = -FLT_MAX;
	t_far  =  FLT_MAX;

	for(int i = 0; i < 3; i++)
	{
		float t_min, t_max;
		if(fabsf(dir.v[i]) < FLT_EPSILON)
		{
			if((org.v[i] < min.v[i]) || (org.v[i] > max.v[i]))
				return false;
		}
		else
		{
			t_min = (min.v[i] - org.v[i]) / dir.v[i];
			t_max = (max.v[i] - org.v[i]) / dir.v[i];

			if(t_min > t_max)
			{
				float temp = t_min;
				t_min = t_max;
				t_max = temp;
			}
			if(t_min > t_near)
				t_near = t_min;
			if(t_max < t_far)
				t_far = t_max;
			if(t_near > t_far)
				return false;
		}
	}

	if(t_far < 0.0f)
		return false;

	if(t_near < 0.0f)
		t_near = 0.0f;

	return true;
}

bool AABB::Contains(const Vector3& pos) const
{
	if((pos.x < min.x) || (pos.x > max.x)
	 ||(pos.y < min.y) || (pos.y > max.y)
	 ||(pos.z < min.z) || (pos.y > max.z))
		return false;
	return true;
}
