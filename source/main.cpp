
#include <fstream>
#include <iostream>
#include "lib/math/vecmat.h"
#include "lib/math/random.h"
#include "renderer.h"
#include "config.h"
#ifdef USE_PERF_CHECK 
#include <windows.h>
#include <mmsystem.h>	// winmm.lib リンクすること
#endif // USE_PERF_CHECK
#include "3ds.h"
#include "environment.h"


/*!
	@brief		リファレンスシーンの初期化
	@param[o]	renderer:
	@param[i]	env:
 */
bool InitReferenceScene(Renderer& renderer, const Environment& env)
{
	// initialize renderer
	renderer.Init();
	renderer.SetMaxDepth(env.depth);
	renderer.SetMaxSampling(env.sample);

	// initialize scene
	Scene* scn = renderer.GetScene();
	ColorSet(&scn->GetBGColor(), env.bg_r, env.bg_g, env.bg_b);
	{
		// material
		Material* mtrl[5];

		mtrl[0] = new Material;
		ColorSet(&mtrl[0]->pd, 1.0f, 0.5f, 0.5f);
		ColorSet(&mtrl[0]->ps, 0.5f, 0.5f, 0.5f);
		ColorSet(&mtrl[0]->e, 0.0f, 0.0f, 0.0f);
		mtrl[0]->kd = 0.8f;
		mtrl[0]->ks = 0.2f;
		mtrl[0]->shine = 25.0f;
		scn->GetMaterialList().push_back(mtrl[0]);

		mtrl[1] = new Material;
		ColorSet(&mtrl[1]->pd, 0.5f, 1.0f, 0.5f);
		ColorSet(&mtrl[1]->ps, 0.5f, 0.5f, 0.5f);
		ColorSet(&mtrl[1]->e, 0.0f, 0.0f, 0.0f);
		mtrl[1]->kd = 0.8f;
		mtrl[1]->ks = 0.2f;
		mtrl[1]->shine = 25.0f;
		scn->GetMaterialList().push_back(mtrl[1]);

		mtrl[2] = new Material;
		ColorSet(&mtrl[2]->pd, 0.5f, 0.5f, 1.0f);
		ColorSet(&mtrl[2]->ps, 0.5f, 0.5f, 0.5f);
		ColorSet(&mtrl[2]->e, 0.0f, 0.0f, 0.0f);
		mtrl[2]->kd = 0.8f;
		mtrl[2]->ks = 0.2f;
		mtrl[2]->shine = 25.0f;
		scn->GetMaterialList().push_back(mtrl[2]);

		mtrl[3] = new Material;
		ColorSet(&mtrl[3]->pd, 0.3f, 0.3f, 0.3f);
		ColorSet(&mtrl[3]->ps, 0.0f, 0.0f, 0.0f);
		ColorSet(&mtrl[3]->e, 0.0f, 0.0f, 0.0f);
		mtrl[3]->kd = 1.0f;
		mtrl[3]->ks = 0.0f;
		mtrl[3]->shine = 0.0f;
		scn->GetMaterialList().push_back(mtrl[3]);

		mtrl[4] = new Material;
		ColorSet(&mtrl[4]->pd, 0.0f, 0.0f, 0.0f);
		ColorSet(&mtrl[4]->ps, 0.0f, 0.0f, 0.0f);
		ColorSet(&mtrl[4]->e, 100.0f, 100.0f, 100.0f);
		mtrl[4]->kd = 0.0f;
		mtrl[4]->ks = 0.0f;
		mtrl[4]->shine = 0.0f;
		scn->GetMaterialList().push_back(mtrl[4]);

		// primitive
		Sphere* sph;
		sph = new Sphere();
		sph->p.set(-2.0f, 1.0f, 0.0f);
		sph->r = 1.0f;
		sph->SetMaterial(mtrl[0]);
		scn->GetPrimitiveList().push_back(sph);

		sph = new Sphere();
		sph->p.set(0.0f, 1.0f, 1.0f);
		sph->r = 1.0f;
		sph->SetMaterial(mtrl[1]);
		scn->GetPrimitiveList().push_back(sph);

		sph = new Sphere();
		sph->p.set( 2.0f, 1.0f, 0.0f);
		sph->r = 1.0f;
		sph->SetMaterial(mtrl[2]);
		scn->GetPrimitiveList().push_back(sph);

		// 床
		Triangle* tri;
		tri = new Triangle();
		tri->v[0].p.set(-3.0f, 0.0f, 3.0f); tri->v[1].p.set( 3.0f, 0.0f, 3.0f); tri->v[2].p.set(-3.0f, 0.0f,-3.0f);
		tri->v[0].n.set( 0.0f, 1.0f, 0.0f); tri->v[1].n.set( 0.0f, 1.0f, 0.0f); tri->v[2].n.set( 0.0f, 1.0f, 0.0f);
		tri->n.set( 0.0f, 1.0f, 0.0f);
		tri->SetMaterial(mtrl[3]);
		scn->GetPrimitiveList().push_back(tri);

		tri = new Triangle();
		tri->v[0].p.set( 3.0f, 0.0f, 3.0f); tri->v[1].p.set( 3.0f, 0.0f,-3.0f); tri->v[2].p.set(-3.0f, 0.0f,-3.0f);
		tri->v[0].n.set( 0.0f, 1.0f, 0.0f); tri->v[1].n.set( 0.0f, 1.0f, 0.0f); tri->v[2].n.set( 0.0f, 1.0f, 0.0f);
		tri->n.set( 0.0f, 1.0f, 0.0f);
		tri->SetMaterial(mtrl[3]);
		scn->GetPrimitiveList().push_back(tri);

		// 左
		tri = new Triangle();
		tri->v[0].p.set(-3.0f, 6.0f,-3.0f); tri->v[1].p.set(-3.0f, 6.0f, 3.0f); tri->v[2].p.set(-3.0f, 0.0f,-3.0f);
		tri->v[0].n.set(1.0f, 0.0f, 0.0f);  tri->v[1].n.set(1.0f, 0.0f, 0.0f);  tri->v[2].n.set(1.0f, 0.0f, 0.0f);
		tri->n.set(1.0f, 0.0f, 0.0f);
		tri->SetMaterial(mtrl[3]);
		scn->GetPrimitiveList().push_back(tri);

		tri = new Triangle();
		tri->v[0].p.set(-3.0f, 6.0f, 3.0f); tri->v[1].p.set(-3.0f, 0.0f, 3.0f); tri->v[2].p.set(-3.0f, 0.0f,-3.0f);
		tri->v[0].n.set(1.0f, 0.0f, 0.0f); 	tri->v[1].n.set(1.0f, 0.0f, 0.0f); 	tri->v[2].n.set(1.0f, 0.0f, 0.0f);
		tri->n.set(1.0f, 0.0f, 0.0f);
		tri->SetMaterial(mtrl[3]);
		scn->GetPrimitiveList().push_back(tri);

		// 右
		tri = new Triangle();
		tri->v[0].p.set(3.0f, 6.0f, 3.0f);  tri->v[1].p.set(3.0f, 6.0f,-3.0f);  tri->v[2].p.set(3.0f, 0.0f, 3.0f);
		tri->v[0].n.set(-1.0f, 0.0f, 0.0f); tri->v[1].n.set(-1.0f, 0.0f, 0.0f); tri->v[2].n.set(-1.0f, 0.0f, 0.0f);
		tri->n.set(-1.0f, 0.0f, 0.0f);
		tri->SetMaterial(mtrl[3]);
		scn->GetPrimitiveList().push_back(tri);

		tri = new Triangle();
		tri->v[0].p.set(3.0f, 6.0f,-3.0f); 	tri->v[1].p.set(3.0f, 0.0f,-3.0f); 	tri->v[2].p.set(3.0f, 0.0f, 3.0f);
		tri->v[0].n.set(-1.0f, 0.0f, 0.0f); tri->v[1].n.set(-1.0f, 0.0f, 0.0f); tri->v[2].n.set(-1.0f, 0.0f, 0.0f);
		tri->n.set(-1.0f, 0.0f, 0.0f);
		tri->SetMaterial(mtrl[3]);
		scn->GetPrimitiveList().push_back(tri);

		// 奥
		tri = new Triangle();
		tri->v[0].p.set(-3.0f, 6.0f, 3.0f); tri->v[1].p.set( 3.0f, 6.0f, 3.0f); tri->v[2].p.set(-3.0f, 0.0f, 3.0f);
		tri->v[0].n.set(0.0f, 0.0f,-1.0f);  tri->v[1].n.set(0.0f, 0.0f,-1.0f); 	tri->v[2].n.set(0.0f, 0.0f,-1.0f);
		tri->n.set(0.0f, 0.0f,-1.0f);
		tri->SetMaterial(mtrl[3]);
		scn->GetPrimitiveList().push_back(tri);

		tri = new Triangle();
		tri->v[0].p.set( 3.0f, 6.0f, 3.0f); tri->v[1].p.set( 3.0f, 0.0f, 3.0f); tri->v[2].p.set(-3.0f, 0.0f, 3.0f);
		tri->v[0].n.set(0.0f, 0.0f,-1.0f);  tri->v[1].n.set(0.0f, 0.0f,-1.0f);  tri->v[2].n.set(0.0f, 0.0f,-1.0f);
		tri->n.set(0.0f, 0.0f,-1.0f);
		tri->SetMaterial(mtrl[3]);
		scn->GetPrimitiveList().push_back(tri);

		// 天井
		tri = new Triangle();
		tri->v[0].p.set(-3.0f, 6.0f,-3.0f); tri->v[1].p.set( 3.0f, 6.0f,-3.0f); tri->v[2].p.set(-3.0f, 6.0f, 3.0f);
		tri->v[0].n.set(0.0f, -1.0f, 0.0f); tri->v[1].n.set(0.0f, -1.0f, 0.0f); tri->v[2].n.set(0.0f, -1.0f, 0.0f);
		tri->n.set(0.0f, -1.0f, 0.0f);
		tri->SetMaterial(mtrl[3]);
		scn->GetPrimitiveList().push_back(tri);

		tri = new Triangle();
		tri->v[0].p.set( 3.0f, 6.0f,-3.0f); tri->v[1].p.set( 3.0f, 6.0f, 3.0f); tri->v[2].p.set(-3.0f, 6.0f, 3.0f);
		tri->v[0].n.set(0.0f, -1.0f, 0.0f); tri->v[1].n.set(0.0f, -1.0f, 0.0f); tri->v[2].n.set(0.0f, -1.0f, 0.0f);
		tri->n.set(0.0f, 0.0f,-1.0f);
		tri->SetMaterial(mtrl[3]);
		scn->GetPrimitiveList().push_back(tri);

		// 手前
		tri = new Triangle();
		tri->v[0].p.set( 3.0f, 6.0f,-3.0f); tri->v[1].p.set(-3.0f, 6.0f,-3.0f); tri->v[2].p.set( 3.0f, 0.0f,-3.0f);
		tri->v[0].n.set(0.0f, 0.0f, 1.0f);  tri->v[1].n.set(0.0f, 0.0f, 1.0f);  tri->v[2].n.set(0.0f, 0.0f, 1.0f);
		tri->n.set(0.0f, 0.0f, 1.0f);
		tri->SetMaterial(mtrl[3]);
		scn->GetPrimitiveList().push_back(tri);

		tri = new Triangle();
		tri->v[0].p.set(-3.0f, 6.0f,-3.0f); tri->v[1].p.set(-3.0f, 0.0f,-3.0f); tri->v[2].p.set( 3.0f, 0.0f,-3.0f);
		tri->v[0].n.set(0.0f, 0.0f, 1.0f);  tri->v[1].n.set(0.0f, 0.0f, 1.0f);  tri->v[2].n.set(0.0f, 0.0f, 1.0f);
		tri->n.set(0.0f, 0.0f, 1.0f);
		tri->SetMaterial(mtrl[3]);
		scn->GetPrimitiveList().push_back(tri);

		// 天井(光源)
		tri = new Triangle();
		tri->v[0].p.set(-1.0f, 5.9f,-1.0f); tri->v[1].p.set( 1.0f, 5.9f,-1.0f); tri->v[2].p.set(-1.0f, 5.9f, 1.0f);
		tri->v[0].n.set(0.0f, -1.0f, 0.0f); tri->v[1].n.set(0.0f, -1.0f, 0.0f); tri->v[2].n.set(0.0f, -1.0f, 0.0f);
		tri->n.set(0.0f, -1.0f, 0.0f);
		tri->SetMaterial(mtrl[4]);
		scn->GetPrimitiveList().push_back(tri);

		tri = new Triangle();
		tri->v[0].p.set( 1.0f, 5.9f,-1.0f); tri->v[1].p.set( 1.0f, 5.9f, 1.0f); tri->v[2].p.set(-1.0f, 5.9f, 1.0f);
		tri->v[0].n.set(0.0f, -1.0f, 0.0f); tri->v[1].n.set(0.0f, -1.0f, 0.0f); tri->v[2].n.set(0.0f, -1.0f, 0.0f);
		tri->n.set(0.0f, 0.0f,-1.0f);
		tri->SetMaterial(mtrl[4]);
		scn->GetPrimitiveList().push_back(tri);
	}
	Light* lig;
	lig = new Light();
#if 1
	lig->type = Light::Type_Point;
	lig->pos.set(0.0f, 5.8f, 0.0f);
#else
	lig->type = Light::Type_Directional;
	lig->dir.set(0.0f,-1.0f,-1.0f);
	Vec3Normalize(&lig->dir, &lig->dir);
#endif
	ColorSet(&lig->intensity, 100.0f, 100.0f, 100.0f);
	scn->GetLightList().push_back(lig);

	scn->Build();

	// initialize camera
	Vector3 at, eye, up;
	up.set(0.0f, 1.0f, 0.0f);
	if(env.flag)
	{
		const AABB& aabb = scn->GetAABB();
		Vector3 half;
		Vec3Subtract(&half, &aabb.max, &aabb.min);
		Vec3Scale(&half, &half, 0.5f);
		Vec3Add(&at, &aabb.min, &half);
		eye.set( 0.0f, 0.0f, -1.0f);
		Vec3Normalize(&eye, &eye);
		Vec3Scale(&eye, &eye, Vec3Length(&half)*3.0f);
		Vec3Add(&eye, &at, &eye);
	}
	else
	{
		eye.set(env.eye_x, env.eye_y, env.eye_z);
		at.set(env.at_x, env.at_y, env.at_z);
	}

	Camera* cam = renderer.GetCamera();
	Mtx44LookAt(&cam->GetPosture(), &eye, &at, &up);
	cam->SetFocalLength(env.focal_length);
	cam->SetFStop(env.f_stop);
	cam->SetFocalPlane(env.focal_plane);
	cam->GetFrameBuffer().resize(env.scr_width, env.scr_height);

	return true;
}

/*!
	@brief		シーンの初期化
	@param[o]	renderer:
	@param[i]	filename:
	@param[i]	env:
 */
bool Init(Renderer& renderer, const std::string& filename, const Environment& env)
{
	// initialize renderer
	renderer.Init();
	renderer.SetMaxDepth(env.depth);
	renderer.SetMaxSampling(env.sample);

	// initialize scene
	Scene* scn = renderer.GetScene();
	ColorSet(&scn->GetBGColor(), 0.0f, 0.0f, 0.0f); 
	{
		_3ds::Geometry geom;
		if(!geom.Load(filename))
		{
			std::cout << "Load failed" << std::endl;
			return false;
		}

		const _3ds::material_array& materials = geom.GetMaterials();
		Material** mtrls = new Material*[materials.size()];
		int counter = 0;
		for(_3ds::material_array::const_iterator it = materials.begin(); it != materials.end(); it++)
		{
			Material* mtrl = mtrls[counter] = new Material;
			counter++;
			ColorSet(&mtrl->pd, (*it)->diffuse.r,  (*it)->diffuse.g,  (*it)->diffuse.b);
			ColorSet(&mtrl->ps, (*it)->specular.r, (*it)->specular.g, (*it)->specular.b);
			ColorSet(&mtrl->e, 0.0f, 0.0f, 0.0f);
			float d = sqrtf(mtrl->pd.r * mtrl->pd.r + mtrl->pd.g * mtrl->pd.g + mtrl->pd.b * mtrl->pd.b);
			float s = sqrtf(mtrl->ps.r * mtrl->ps.r + mtrl->ps.g * mtrl->ps.g + mtrl->ps.b * mtrl->ps.b);
			mtrl->kd = d / (d + s);
			mtrl->ks = s / (d + s);
			mtrl->shine = (*it)->shininess;
			scn->GetMaterialList().push_back(mtrl);
		}

		const _3ds::mesh_array& meshes = geom.GetMeshes();
		for(_3ds::mesh_array::const_iterator it = meshes.begin(); it != meshes.end(); it++)
		{
			const Vector3* vertices = (*it)->vertices;
			const unsigned short num_faces = (*it)->num_faces;
			for(unsigned short i = 0; i < num_faces; i++)
			{
				Triangle* tri = new Triangle;
				tri->v[0].p = vertices[(*it)->faces[i].a];
				tri->v[1].p = vertices[(*it)->faces[i].b];
				tri->v[2].p = vertices[(*it)->faces[i].c];
				tri->v[0].n = (*it)->faces[i].n0;
				tri->v[1].n = (*it)->faces[i].n1;
				tri->v[2].n = (*it)->faces[i].n2;
				tri->n = (*it)->faces[i].n;
				tri->SetMaterial(mtrls[(*it)->faces[i].mtrl_id]);
				scn->GetPrimitiveList().push_back(tri);
			}
		}
		delete[] mtrls;
	}

	scn->Build();

	// initialize camera
	Vector3 at, eye, up;
	up.set(0.0f, 1.0f, 0.0f);
	if(env.flag)
	{
		const AABB& aabb = scn->GetAABB();
		Vector3 half;
		Vec3Subtract(&half, &aabb.max, &aabb.min);
		Vec3Scale(&half, &half, 0.5f);
		Vec3Add(&at, &aabb.min, &half);
		eye.set( 0.0f, 0.0f, -1.0f);
		Vec3Normalize(&eye, &eye);
		Vec3Scale(&eye, &eye, Vec3Length(&half)*3.0f);
		Vec3Add(&eye, &at, &eye);
	}
	else
	{
		eye.set(env.eye_x, env.eye_y, env.eye_z);
		at.set(env.at_x, env.at_y, env.at_z);
	}

	Camera* cam = renderer.GetCamera();
	Mtx44LookAt(&cam->GetPosture(), &eye, &at, &up);
	cam->SetFocalLength(env.focal_length);
	cam->SetFStop(env.f_stop);
	cam->SetFocalPlane(env.focal_plane);
	cam->GetFrameBuffer().resize(env.scr_width, env.scr_height);

	return true;
}

/*!
	@brief		光源の初期化
	@param[o]	sce: シーン
	@param[i]	filename: ファイル名
 */
void InitLight(Scene& scn, const std::string& filename)
{
	std::ifstream ifs;
	ifs.open(filename.c_str(), std::ios::in | std::ios::binary);
	if(!ifs)
	{
		Light* lig;
		lig = new Light();
 #if 0
		lig->type = Light::Type_Point;
		lig->pos.set(0.0f,100.0f,-100.0f);
	//	lig->pos.set(0.0f, 5.8f, 0.0f);
 #else
		lig->type = Light::Type_Directional;
		lig->dir.set(0.0f,-1.0f,-1.0f);
		Vec3Normalize(&lig->dir, &lig->dir);
 #endif
		ColorSet(&lig->intensity, 100.0f, 100.0f, 100.0f);
		scn.GetLightList().push_back(lig);
	}
	else
	{
		struct LightInfo
		{
			unsigned long enbale;
			unsigned long type;
			Vector3	pos;
			Vector3	rot;
			Vector3 col;
		}info;

		ifs.seekg(0, std::ios_base::end);
		const unsigned long length = ifs.tellg();
		const unsigned long num = length / sizeof(Light);
		ifs.seekg(0, std::ios_base::beg);
		for(unsigned long i = 0; i < num; i++)
		{
			ifs.read((char*)&info, sizeof(LightInfo));
			if(!info.enbale)
				continue;

			Light* lig = new Light();
			if(info.type == 0)
			{
				lig->type = Light::Type_Point;
				lig->pos = info.pos;
			}
			else
			{
				lig->type = Light::Type_Directional;

				Vector3 dir;
				dir.set(0.0f, 0.0f, 1.0f);
				Matrix44 mat;
				info.rot.x = DegToRad(info.rot.x);
				info.rot.y = DegToRad(info.rot.y);
				info.rot.z = DegToRad(info.rot.z);
				Mtx44RotationYawPitchRoll(&mat, info.rot.y, info.rot.x, info.rot.z);
				Vec3Transform(&lig->dir, &dir, &mat);
			}
			ColorSet(&lig->intensity, info.col.x, info.col.y, info.col.z);

			scn.GetLightList().push_back(lig);
		}
		ifs.close();
	}
}

/*!
	@brief		エントリー
	@param[i]	argc: 引数の数
	@param[i]	argv: 引数
 */
int main(int argc, const char* argv[])
{	
	Environment env;
 #ifdef USE_ENV_FILE
	if(!LoadEnvironmentFile(&env, "env.dat"))
	{
		std::cout << "env.dat load failed" << std::endl;
		return 0;
	}
 #else
	env.scr_width	= SCR_WIDTH;
	env.scr_height	= SCR_HEIGHT;
	env.depth		= MAX_DEPTH;
	env.sample		= MAX_SAMPLING;
	env.kd_depth	= MAX_KDTREE_DEPTH;
	env.bg_r		= 0.0f;
	env.bg_g		= 0.0f;
	env.bg_b		= 0.0f;
	env.focal_length= 0.05f;
	env.f_stop		= 2.8f;
	env.focal_plane	= 1.0f;
	env.flag		= 0;
 #endif // USE_ENV_FILE

	bool resource = false;
	std::string ifilename, ofilename;
	if(argc > 1)
	{
		ifilename = argv[1];
		ofilename = ifilename.substr(0, ifilename.rfind('.')).append(".bmp");
		resource = true;
	}
	else
	{
		ofilename = "result.bmp";
	}

 #ifdef USE_PERF_CHECK
	timeBeginPeriod(1);
 #endif // USE_PERF_CHECK

	// initialize
	genrand_int32();
	Renderer renderer;

	// Setup
	{
		std::cout << ">>> setup start" << std::endl;
 #ifdef USE_PERF_CHECK
		DWORD begin_time = timeGetTime();
 #endif // USE_PERF_CHECK
		if(resource)
		{
			if(!Init(renderer, argv[1], env))
				return 0;
			Scene* scn = renderer.GetScene();
			InitLight(*scn, "lig.dat");
		}
		else
		{
			InitReferenceScene(renderer, env);
		}
 #ifdef USE_PERF_CHECK
		DWORD time = (timeGetTime() - begin_time);
		std::cout << "lapsed time[ms] = " << time << std::endl;
 #endif // USE_PERF_CHECK
		std::cout << "<<< setup end" << std::endl;
	}
	// render
	{
		std::cout << ">>> render start" << std::endl;
 #ifdef USE_PERF_CHECK
		DWORD begin_time = timeGetTime();
 #endif // USE_PERF_CHECK
		renderer.Render();
 #ifdef USE_PERF_CHECK
		DWORD time = (timeGetTime() - begin_time);
		std::cout << "lapsed time[ms] = " << time << std::endl;
 #endif // USE_PERF_CHECK
		std::cout << "<<< render end" << std::endl;
	}
	// post process
	{
		std::cout << ">>> post process start" << std::endl;
 #ifdef USE_PERF_CHECK
		DWORD begin_time = timeGetTime();
 #endif // USE_PERF_CHECK
		Camera* cam = renderer.GetCamera();
		FrameBufferFP32& fb = cam->GetFrameBuffer();
		fb.Exposure(0.2f);
		fb.Saturate();
		fb.GammaCorrection();
		fb.WriteBmpFile(ofilename);
 #ifdef USE_PERF_CHECK
		DWORD time = (timeGetTime() - begin_time);
		std::cout << "lapsed time[ms] = " << time << std::endl;
 #endif // USE_PERF_CHECK
		std::cout << "<<< post process end" << std::endl;
	}
	// release
	renderer.Release();

 #ifdef USE_PERF_CHECK
	timeEndPeriod(1);
 #endif // USE_PERF_CHECK
	return 1;
}