
#include "lib/math/random.h"
#include "common.h"
#include "renderer.h"
#include "reflection.h"
#include "config.h"


#ifdef USE_MULTI_THREAD
/*!
	@brief	描画スレッド用ワーク
	@class	RenderWork
 */
class RenderWork : public Work
{
public:
	void Set(std::size_t x, std::size_t y, std::size_t w, std::size_t h, Renderer* renderer)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
		ref_renderer = renderer;
	}
public:
	std::size_t x, y, w, h;
	Renderer* ref_renderer;
};
#endif // USE_MULTI_THREAD

Renderer::Renderer() : scene(NULL), camera(NULL), max_sampling(1), max_depth(3)
{
}

Renderer::~Renderer()
{
	if(scene)
		delete scene;
	if(camera)
		delete camera;
}

void Renderer::Init()
{
	Release();
	
	scene = new Scene();
	camera = new Camera();
}

void Renderer::Release()
{
	SAFE_DELETE(scene);
	SAFE_DELETE(camera);
}

/*!
	@brief		描画メイン
 */
void Renderer::Render()
{
 #ifndef USE_MULTI_THREAD
	FrameBufferFP32& fb = camera->GetFrameBuffer();
	Render(0, 0, fb.width(), fb.height());
 #else
	FrameBufferFP32& fb = camera->GetFrameBuffer();
	const std::size_t w = fb.width();
	const std::size_t h = fb.height();

	RenderWork work[4];
	work[0].Set(0,   0,   w/2, h/2, this);
	work[1].Set(w/2, 0,   w/2, h/2, this);
	work[2].Set(0,   h/2, w/2, h/2, this);
	work[3].Set(w/2, h/2, w/2, h/2, this);

	WorkPile* wp = new WorkPile();
	wp->request(&work[0]);
	wp->request(&work[1]);
	wp->request(&work[2]);
	wp->request(&work[3]);

	wp->start(Renderer::worker_thread, 4);
	while(wp->get_left_work() > 0)
		::Sleep(10);

	delete wp;
 #endif // !USE_MULTI_THREAD
}

/*!
	@brief		描画メイン
	@param[i]	bx: 開始座標
	@param[i]	by: 開始座標
	@param[i]	ex: 終了座標
	@param[i]	yx: 終了座標
 */
void Renderer::Render(std::size_t bx, std::size_t by, std::size_t ex, std::size_t ey)
{
	FrameBufferFP32& fb = camera->GetFrameBuffer();
	const std::size_t w = fb.width();
	const std::size_t h = fb.height();
	const float inv_w = 1.0f / (float)w;
	const float inv_h = 1.0f / (float)h;
	const std::size_t smapling = max_sampling;

	Ray ray;
	ray.org = camera->GetPosture().row_vector3(3);

	Color col, accum;
	for(std::size_t y = by; y < ey; y++)
	{
		FrameBufferFP32::Data* p = fb.ptr(y) + bx;
		for(std::size_t x = bx; x < ex; x++)
		{
			ColorSet(&accum, 0.0f, 0.0f, 0.0f);
			for(std::size_t s = 0; s < smapling; s++)
			{
				const float sub_x = ((float)x + ((float)genrand_real1() - 0.5f)) * inv_w;
				const float sub_y = ((float)y + ((float)genrand_real1() - 0.5f)) * inv_h;
				camera->ShootRay(ray, sub_x, sub_y);
				Trace(col, ray, 0);
				ColorAdd3(&accum, &accum, &col);
			}
			ColorScale3(&col, &accum, 1.0f/(float)smapling);

			p->ch[0] = col.r;
			p->ch[1] = col.g;
			p->ch[2] = col.b;
			p++;		
		}
	}
}

/*!
	@brief		トレース
	@param[o]	out: 出力輝度
	@param[i]	ray: 光線
	@param[i]	depth: 深度
 */
void Renderer::Trace(Color& out, const Ray& ray, std::size_t depth)
{
	Primitive* prim = NULL;
	Primitive::Param param;

	if((depth >= max_depth) || !FindNearest(&prim, param, ray))
	{
		out = scene->GetBGColor();
		return;
	}

	Vertex v;
	prim->CalcVertex(v, param, ray);
	Material* mtrl = prim->GetMaterial();

	// emittance
	out = mtrl->e;
 #ifdef USE_LOCAL_ILLUMINATION
	// direct lighting
	Color direct;
	DirectLighting(direct, ray, v, *mtrl);
	ColorAdd3(&out, &out, &direct);
 #endif // USE_LOCAL_ILLUMINATION
 #ifdef USE_GLOBAL_ILLUMINATION
	// indirect lighting
	Color indirect;
	IndirectLighting(indirect, ray, v, *mtrl, depth);
	ColorAdd3(&out, &out, &indirect);
 #endif // USE_GLOBAL_ILLUMINATION
}

/*!
	@brief		最近傍チェック
	@param[o]	prim: プリミティブ
	@param[o]	param: パラメータ
	@param[i]	ray: 光線
 */
bool Renderer::FindNearest(Primitive** prim, Primitive::Param& param, const Ray& ray)
{
 #ifdef USE_KDTREE
	const KdTree* kdtree = scene->GetKdTree();
	return kdtree->Traverse(prim, param, ray);
 #else
	float t = FLT_MAX;
	Primitive::Param par;

	(*prim) = NULL;

	const PrimitiveList& list = scene->GetPrimitiveList();
	for(PrimitiveList::const_iterator it = list.begin(); it != list.end(); it++)
	{
		if((*it)->Intersect(par, ray))
		{
			if(par.t < t)
			{
				(*prim) = (*it);
				param = par;
				t = par.t;
			}
		}
	}
	return (*prim)? true : false;
 #endif // USE_KDTREE
}

/*!
	@brief		光源方向への遮蔽物チェック
	@param[o]	t: 最近傍
	@param[i]	ray: 光線
 */
bool Renderer::FindOccluder(float& t, const Ray& ray)
{
	Primitive* prim = NULL;
	Primitive::Param param;

 #ifdef USE_KDTREE
	const KdTree* kdtree = scene->GetKdTree();
	bool ret = kdtree->Traverse(&prim, param, ray);
	t = param.t;
	return ret;
 #else
	if(FindNearest(&prim, param, ray))
	{
		t = param.t;
		return true;
	}
	return false;
 #endif // USE_KDTREE
}

/*!
	@brief		直接照明計算
	@param[o]	out: 出力輝度
	@param[i]	v: 着目点
	@param[i]	mtrl: マテリアル	
 */
void Renderer::DirectLighting(Color& out, const Ray& ray, const Vertex& v, const Material& mtrl)
{
	// 自己遮蔽で引っかかるため法線方向に押し出す
	const float epsilon = 0.001f;
	Ray to_lig;
	ColorSet(&out, 0.0f, 0.0f, 0.0f);

	const LightList& list = scene->GetLightList();
	for(LightList::const_iterator it = list.begin(); it != list.end(); it++)
	{
		// occlusion test
 #ifdef USE_OCCLUSION_TEST
		if((*it)->type == Light::Type_Point)
		{
			Vec3Subtract(&to_lig.dir, &(*it)->pos, &v.p);
			float d = sqrtf(Vec3InnerProduct(&to_lig.dir, &to_lig.dir));
			if(d < FLT_EPSILON)
				continue;
			Vec3Scale(&to_lig.dir, &to_lig.dir, 1.0f/d);
			Vec3Scale(&to_lig.org, &to_lig.dir, epsilon);
			Vec3Add(&to_lig.org, &v.p, &to_lig.org);
			float t;
			if(FindOccluder(t, to_lig) && (t <= (d + epsilon)))
				continue;
		}
		else
		{
			to_lig.dir = -(*it)->dir;
			Vec3Scale(&to_lig.org, &to_lig.dir, epsilon);
			Vec3Add(&to_lig.org, &v.p, &to_lig.org);
			float t;
			if(FindOccluder(t, to_lig))
				continue;
		}
 #endif // USE_OCCLUSION_TEST
		// lighting
		Color col;
		(*it)->Lighting(col, ray, v, mtrl);
		ColorAdd3(&out, &out, &col);
	}
}

/*!
	@brief		間接照明計算
	@param[o]	out: 出力輝度
	@param[i]	v: 着目点
	@param[i]	mtrl: マテリアル	
	@param[i]	depth: 深度
 */
void Renderer::IndirectLighting(Color& out, const Ray& ray, const Vertex& v, const Material& mtrl, std::size_t depth)
{
	ColorSet(&out, 0.0f, 0.0f, 0.0f);

	const float e = (float)genrand_real1();
	if(e < mtrl.kd)
	{
		Ray ray2;	// 2nd ray
		ray2.org = v.p;
		random_vector_cosweight(&ray2.dir, &v.n);

		Color ref;
		Trace(ref, ray2, depth+1);

		// out += (brdf * ref * cosθ) / (pdf * kd)
		Color temp;
		ColorModulate3(&temp, &mtrl.pd, &ref);
		ColorScale3(&out, &temp, 1.0f / mtrl.kd);
	}
	else
	if(e < (mtrl.kd + mtrl.ks))
	{
		// 
		Vector3 in = -ray.dir;
		Ray ray2;
		ray2.org = v.p;
		random_vector_cosweight(&ray2.dir, &in, &v.n, mtrl.shine);
		float cost= Vec3InnerProduct(&ray2.dir, &v.n);
		if(cost <= 0.0f)
			return;

		Color ref;
		Trace(ref, ray2, depth+1);

		// out += (brdf * ref * cosθ) / (pdf * ks)
		Color temp;
		ColorScale3(&temp, &mtrl.ps, (mtrl.shine + 2.0f)/(mtrl.shine + 1.0f) * cost);
		ColorScale3(&out, &temp, 1.0f / mtrl.ks);
	}
}

#ifdef USE_MULTI_THREAD
/*!
	@brief		描画用ワーカースレッド
	@param[i]	arg
 */
unsigned __stdcall Renderer::worker_thread(void* arg)
{
	WorkPile* wp = (WorkPile*)arg;
	RenderWork* work;

	while(wp->is_enable())
	{
		while((work = dynamic_cast<RenderWork*>(wp->get_work())))
		{
			work->set_status(Work::Status_Start);
			work->ref_renderer->Render(work->x, work->y, work->x + work->w, work->y + work->h);
			work->set_status(Work::Status_Completed);
		}
		::Sleep(10);
	}
	return 0;	
}
#endif // USE_MULTI_THREAD
