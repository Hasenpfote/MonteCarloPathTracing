
#include "config.h"
#include "scene.h"


Scene::Scene()
{
 #ifdef USE_KDTREE
	kdtree = NULL;
 #endif // USE_KDTREE
	ColorSet(&back_ground, 0.0f, 0.0f, 0.0f);
}

Scene::~Scene()
{
 #ifdef USE_KDTREE
	if(kdtree)
		delete kdtree;
 #endif // USE_KDTREE
	for(PrimitiveList::iterator it = prim_list.begin(); it != prim_list.end(); it++)
	{
		if((*it))
			delete (*it);
	}
	for(MaterialList::iterator it = mtrl_list.begin(); it != mtrl_list.end(); it++)
	{
		if((*it))
			delete (*it);
	}
	for(LightList::iterator it = light_list.begin(); it != light_list.end(); it++)
	{
		if((*it))
			delete (*it);
	}
}

void Scene::MakeAABB()
{
	aabb.min.set( FLT_MAX, FLT_MAX, FLT_MAX); 
	aabb.max.set(-FLT_MAX,-FLT_MAX,-FLT_MAX);
	float _min, _max;
	for(PrimitiveList::const_iterator it = prim_list.begin(); it != prim_list.end(); it++)
	{
		for(int axis = Axis_X; axis < Axis_Max; axis++)
		{
			(*it)->CalcRange(_min, _max, (Axis)axis);
			if(_min < aabb.min.v[axis])
				aabb.min.v[axis] = _min;
			if(_max > aabb.max.v[axis])
				aabb.max.v[axis] = _max;
		}
	}
}

void Scene::Build()
{
	MakeAABB();
 #ifdef USE_KDTREE
	SAFE_DELETE(kdtree);
	kdtree = new KdTree();
	ASSERT_MSG(kdtree != NULL, "Scene::Build(): alloc failed");
	kdtree->Build(prim_list, aabb, MAX_KDTREE_DEPTH);
 #endif // USE_KDTREE
}
