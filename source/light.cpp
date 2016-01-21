
#include "light.h"
#include "reflection.h"

void Light::Lighting(Color& out, const Ray& ray, const Vertex& v, const Material& mtrl)
{
	ColorSet(&out, 0.0f, 0.0f, 0.0f);

	if(type == Type_Point)
	{
		Vector3 lig = pos - v.p;
		float inv_lensq = 1.0f / Vec3LengthSq(&lig);
		Vec3Normalize(&lig, &lig);
		float dot;
		// diffuse
		dot = Vec3InnerProduct(&v.n, &lig);
		if(dot <= 0.0f)
			return;
		Color brdf, temp;
		ColorScale3(&brdf, &mtrl.pd, 1.0f/PI);
		ColorScale3(&temp, &intensity, dot);
		ColorModulate3(&out, &brdf, &temp);
		ColorScale3(&out, &out, inv_lensq);
		// specular
		Vector3 ref, inv_dir;
		inv_dir = -ray.dir;
		calc_reflection(&ref, &inv_dir, &v.n);
		dot = Vec3InnerProduct(&ref, &lig);
		if(dot <= 0.0f)
			return;
		ColorScale3(&brdf, &mtrl.ps, (mtrl.shine + 2.0f) / PI2 * powf(dot, mtrl.shine));
		ColorModulate3(&temp, &brdf, &intensity);
		ColorScale3(&temp, &temp, inv_lensq);
		ColorAdd3(&out, &out, &temp);
	}
	else
//	if(type == Type_Directional)
	{
		Vector3 lig = -dir;
		float dot;
		// diffuse
		dot = Vec3InnerProduct(&lig, &v.n);
		if(dot <= 0.0f)
			return;
		Color brdf, temp;
		ColorScale3(&brdf, &mtrl.pd, 1.0f/PI);
		ColorScale3(&temp, &intensity, dot);
		ColorModulate3(&out, &brdf, &temp);
		// specular
		Vector3 ref, inv_dir;
		inv_dir = -ray.dir;
		calc_reflection(&ref, &inv_dir, &v.n);
		dot = Vec3InnerProduct(&ref, &lig);
		if(dot <= 0.0f)
			return;
		ColorScale3(&brdf, &mtrl.ps, (mtrl.shine + 2.0f) / PI2 * powf(dot, mtrl.shine));
		ColorModulate3(&temp, &brdf, &intensity);
		ColorAdd3(&out, &out, &temp);
	}
}
