//==============================================================================
/*!
	@file	renderer.h
	@brief	レイトレースベースレンダラ
    @author M.Morimoto
    @date   2010/03/17
 */
//==============================================================================
#ifndef __RENDERER_H_
#define __RENDERER_H_

#include "scene.h"
#include "camera.h"
#include "config.h"
#ifdef USE_MULTI_THREAD
#include "lib/system/thread.h"
#endif	// USE_MULTI_THREAD

class Scene;
class Camera;
/*!
	@brief	レンダラ
	@class	Renderer
 */
class Renderer
{
public:
	Renderer();
	~Renderer();

	void Init();
	void Release();
	void Render();
	void Render(std::size_t bx, std::size_t by, std::size_t ex, std::size_t ey);

	Scene* GetScene(){ return scene; }
	Camera* GetCamera(){ return camera; }

	void SetMaxSampling(std::size_t sampling){ max_sampling = sampling; }
	void SetMaxDepth(std::size_t depth){ max_depth = depth; }

private:
	void Trace(Color& out, const Ray& ray, std::size_t depth);
	bool FindNearest(Primitive** prim, Primitive::Param& param, const Ray& ray);
	bool FindOccluder(float& t, const Ray& ray);
	void DirectLighting(Color& out, const Ray& ray, const Vertex& v, const Material& mtrl);
	void IndirectLighting(Color& out, const Ray& ray, const Vertex& v, const Material& mtrl, std::size_t depth);

 #ifdef USE_MULTI_THREAD
	static unsigned __stdcall worker_thread(void*);
 #endif	// USE_MULTI_THREAD

private:
	Scene*	scene;
	Camera*	camera;

	std::size_t max_sampling;
	std::size_t max_depth;
};

#endif // !__RENDERER_H_
