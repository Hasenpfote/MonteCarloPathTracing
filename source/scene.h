//==============================================================================
/*!
	@file	scene.h
	@brief	シーン
    @author M.Morimoto
    @date   2010/03/17
 */
//==============================================================================
#ifndef __SCENE_H_
#define __SCENE_H_

#include "config.h"
#include "primitive.h"
#include "light.h"
#include "material.h"
#include "kdtree.h"

/*!
	@brief	シーン
	@class	Scene
 */
class Scene
{
public:
	Scene();
	~Scene();

	PrimitiveList& GetPrimitiveList(){ return prim_list; }
	MaterialList& GetMaterialList(){ return mtrl_list; }
	LightList& GetLightList(){ return light_list; }
	Color& GetBGColor(){ return back_ground; }
	AABB& GetAABB(){ return aabb; }
 #ifdef USE_KDTREE
	KdTree* GetKdTree(){ return kdtree; }
	const KdTree* GetKdTree() const { return (const KdTree*)kdtree; }
 #endif // USE_KDTREE
	void Build();

private:
	void MakeAABB();

private:
	PrimitiveList	prim_list;
	MaterialList	mtrl_list;
	LightList		light_list;
	Color			back_ground;
	AABB			aabb;
 #ifdef USE_KDTREE
	KdTree*			kdtree;
 #endif // USE_KDTREE
};

#endif // !__SCENE_H_