
#include "primitive.h"

/*!
	@brief		三角形の面法線を計算
	@param[o]	n: 法線
	@param[i]	p0: 点0
	@param[i]	p1: 点1
	@param[i]	p2: 点2
	@note		p0-p1-p2 順で右周りを表とする
 */
static void tri_calc_normal(Vector3* n, const Vector3* p0, const Vector3* p1, const Vector3* p2)
{
	Vector3 e0, e1;

	Vec3Subtract(&e0, p1, p0);
	Vec3Subtract(&e1, p2, p0);
	Vec3OuterProduct(n, &e0, &e1);
	Vec3Normalize(n, n);
}

/*!
	@brief		三角形の法線ベクトルを補完で求める
	@param[o]	n: 法線
	@param[i]	n0: 点0 の法線
	@param[i]	n1: 点1 の法線
	@param[i]	n2: 点2 の法線
	@param[i]	u:  n1 の重み
	@param[i]	v:  n2 の重み
	@note		n = n0*(1-u-v) + n1*u + n2*v
 */
static void tri_lerp_normal(Vector3* n, const Vector3* n0, const Vector3* n1, const Vector3* n2, float u, float v)
{
	Vector3 temp;

	Vec3Scale(n, n0, (1.0f - u - v));
	Vec3Scale(&temp, n1, u);
	Vec3Add(n, n, &temp);
	Vec3Scale(&temp, n2, v);
	Vec3Add(n, n, &temp);
}

/*!
	@brief		三角形と光線の交差判定
	@param[o]	t: 交点パラメータ
	@param[o]	u: 点1 の重み
	@param[o]	v: 点2 の重み
	@param[i]	p0: 点0
	@param[i]	p1: 点1
	@param[i]	p2: 点2
	@param[i]	org: 光線の原点
	@param[i]	dir: 光線の向き
	@retval		true: 接触
	@retval		false: 非接触
	@note		"Practical Analysis of Optimized Ray-Triangle Intersection"
				Tomas Akenine Moeller
				http://www.ce.chalmers.se/staff/tomasm/raytri/
 */
static bool tri_intersect(float* t, float*u, float*v,
						  const Vector3* p0, const Vector3* p1, const Vector3* p2,
						  const Vector3* org, const Vector3* dir)
{
	Vector3 e0, e1;
	Vec3Subtract(&e0, p1, p0);
	Vec3Subtract(&e1, p2, p0);

	Vector3 pvec;
	Vec3OuterProduct(&pvec, dir, &e1);
	float det = Vec3InnerProduct(&e0, &pvec);
	// 光線に対し背面(位置を考慮していない点に注意)か平行なら打ち切る
	if(det <= FLT_EPSILON)
		return false;

	Vector3 tvec;
	Vec3Subtract(&tvec, org, p0);
	(*u) = Vec3InnerProduct(&tvec, &pvec);
	if(((*u) < 0.0f) || ((*u) > det))
		return false;

	Vector3 qvec;
	Vec3OuterProduct(&qvec, &tvec, &e0);
	(*v) = Vec3InnerProduct(dir, &qvec);
	if(((*v) < 0.0f) || ((*u) + (*v) > det))
		return false;

	float inv_det = 1.0f / det;
	(*t) = Vec3InnerProduct(&e1, &qvec) * inv_det;
	(*u)*= inv_det;
	(*v)*= inv_det;

	return true;
}

/*!
	@brief		球と光線の交差判定
	@param[o]	t: 交点パラメータ
	@param[i]	p: 球の位置
	@param[i]	r: 球の半径
	@param[i]	org: 光線の原点
	@param[i]	dir: 光線の向き
	@retval		true: 接触
	@retval		false: 非接触
 */
bool sph_intersect(float* t, const Vector3* p, float r, const Vector3* org, const Vector3* dir)
{
	Vector3 vec;
	Vec3Subtract(&vec, org, p);
	const float b = 2.0f * Vec3InnerProduct(dir, &vec);
	const float c = Vec3InnerProduct(&vec, &vec) - (r * r);
	const float d = b * b - 4.0f * c;
	if(d < 0.0f)
		return false;

	const float sq_d = sqrtf(d);
	const float tn = (-b - sq_d) * 0.5f;	
	const float tp = (-b + sq_d) * 0.5f;
	if(tn < 0.0f)
	{
		if(tp < 0.0f)
			return false;
		(*t) = tp;
	}
	else
	{
		(*t) = tn;
	}
	return true;
}

bool Sphere::Intersect(const AABB& aabb)
{
	float dist = 0.0f;
	for(int i = Axis_X; i < Axis_Max; i++)
	{
		if(p.v[i] < aabb.min.v[i])
		{
			dist += (p.v[i] - aabb.min.v[i]) * (p.v[i] - aabb.min.v[i]);
		}
		else
		if(p.v[i] > aabb.max.v[i])
		{
			dist += (p.v[i] - aabb.max.v[i]) * (p.v[i] - aabb.max.v[i]);
		}
	}
	return (dist <= (r*r))? true : false;
}

bool Sphere::Intersect(Param& param, const Ray& ray)
{
	float t;
	if(sph_intersect(&t, &p, r, &ray.org, &ray.dir))
	{
		param.t = t;
		return true;
	}
	return false;
}

void Sphere::CalcVertex(Vertex& v, const Param& param, const Ray& ray)
{
	// 位置
	Vec3Scale(&v.p, &ray.dir, param.t);
	Vec3Add(&v.p, &ray.org, &v.p);
	// 法線
	Vec3Subtract(&v.n, &v.p, &p);
	Vec3Normalize(&v.n, &v.n);
}

void Sphere::CalcRange(float& min, float& max, Axis axis)
{
	min = p.v[axis] - r;
	max = p.v[axis] + r;
}

/*!
	@brief		
	@param[i]	aabb:
	@note		"Fast 3D Triangle-Box Overlap Testing"
				Tomas Akenine Moeller
 */
bool Triangle::Intersect(const AABB& aabb)
{
	Vector3 c, h;
	Vec3Subtract(&h, &aabb.max, &aabb.min);
	Vec3Scale(&h, &h, 0.5f);
	Vec3Add(&c, &aabb.min, &h);

	Vector3 v0, v1, v2;
	Vec3Subtract(&v0, &v[0].p, &c);
	Vec3Subtract(&v1, &v[1].p, &c);
	Vec3Subtract(&v2, &v[2].p, &c);

	Vector3 f0, f1, f2;
	Vec3Subtract(&f0, &v1, &v0);
	Vec3Subtract(&f1, &v2, &v1);
	Vec3Subtract(&f2, &v0, &v2);

	float p0, p1, p2, min, max, rad, abs_x, abs_y, abs_z;

	abs_x = fabsf(f0.x);
	abs_y = fabsf(f0.y);
	abs_z = fabsf(f0.z);

	// a00 = e0 x f0
	p0 = -f0.z * v0.y + f0.y * v0.z;
	p2 = -f0.z * v2.y + f0.y * v2.z;
	if(p0 < p2){ min = p0; max = p2; }
	else{ min = p2; max = p0; }
	rad = h.y * abs_z + h.z * abs_y;
	if((min > rad) || (max < -rad))
		return false;

	// a10 = e1 * f0
	p0 = f0.z * v0.x - f0.x * v0.z;
	p2 = f0.z * v2.x - f0.x * v2.z;
	if(p0 < p2){ min = p0; max = p2; }
	else{ min = p2; max = p0; }
	rad = h.x * abs_z + h.z * abs_x;
	if((min > rad) || (max < -rad))
		return false;

	// a20 = e2 * f0
	p0 = -f0.y * v0.x + f0.x * v0.y;
	p2 = -f0.y * v2.x + f0.x * v2.y;
	if(p0 < p2){ min = p0; max = p2; }
	else{ min = p2; max = p0; }
	rad = h.x * abs_y + h.y * abs_x;
	if((min > rad) || (max < -rad))
		return false;

	abs_x = fabsf(f1.x);
	abs_y = fabsf(f1.y);
	abs_z = fabsf(f1.z);

	// a01 = e0 x f1
	p0 = -f1.z * v0.y + f1.y * v0.z;
	p1 = -f1.z * v1.y + f1.y * v1.z;
	if(p0 < p1){ min = p0; max = p1; }
	else{ min = p1; max = p0; }
	rad = h.y * abs_z + h.z * abs_y;
	if((min > rad) || (max < -rad))
		return false;

	// a11 = e1 x f1
	p0 = f1.z * v0.x - f1.x * v0.z;
	p1 = f1.z * v1.x - f1.x * v1.z;
	if(p0 < p1){ min = p0; max = p1; }
	else{ min = p1; max = p0; }
	rad = h.x * abs_z + h.z * abs_x;
	if((min > rad) || (max < -rad))
		return false;

	// a21 = e2 x f1
	p0 = -f1.y * v0.x + f1.x * v0.y;
	p1 = -f1.y * v1.x + f1.x * v1.y;
	if(p0 < p1){ min = p0; max = p1; }
	else{ min = p1; max = p0; }
	rad = h.x * abs_y + h.y * abs_x;
	if((min > rad) || (max < -rad))
		return false;

	abs_x = fabsf(f2.x);
	abs_y = fabsf(f2.y);
	abs_z = fabsf(f2.z);

	// a02 = e0 x f2
	p0 = -f2.z * v0.y + f2.y * v0.z;
	p1 = -f2.z * v1.y + f2.y * v1.z;
	if(p0 < p1){ min = p0; max = p1; }
	else{ min = p1; max = p0; }
	rad = h.y * abs_z + h.z * abs_y;
	if((min > rad) || (max < -rad))
		return false;

	// a12 = e1 x f2
	p0 = f2.z * v0.x - f2.x * v0.z;
	p1 = f2.z * v1.x - f2.x * v1.z;
	if(p0 < p1){ min = p0; max = p1; }
	else{ min = p1; max = p0; }
	rad = h.x * abs_z + h.z * abs_x;
	if((min > rad) || (max < -rad))
		return false;

	// a22 = e2 x f2
	p0 = -f2.y * v0.x + f2.x * v0.y;
	p1 = -f2.y * v1.x + f2.x * v1.y;
	if(p0 < p1){ min = p0; max = p1; }
	else{ min = p1; max = p0; }
	rad = h.x * abs_y + h.y * abs_x;
	if((min > rad) || (max < -rad))
		return false;

	return true;
}

bool Triangle::Intersect(Param& param, const Ray& ray)
{
	float t, u, v;
	if(tri_intersect(&t, &u, &v, &this->v[0].p, &this->v[1].p, &this->v[2].p, &ray.org, &ray.dir))
	{
		if(t >= 0.0f)
		{
			param.t = t;
			param.u = u;
			param.v = v;
			return true;
		}
	}
	return false;
}

void Triangle::CalcVertex(Vertex& v, const Param& param, const Ray& ray)
{
	// 位置
	Vec3Scale(&v.p, &ray.dir, param.t);
	Vec3Add(&v.p, &ray.org, &v.p);
	// 法線
	tri_lerp_normal(&v.n, &this->v[0].n, &this->v[1].n, &this->v[2].n, param.u, param.v);
}

void Triangle::CalcRange(float& min, float& max, Axis axis)
{
	min = max = v[0].p.v[axis];
	for(int i = 1; i < 3; i++)
	{
		float t = v[i].p.v[axis];
		if(t < min) min = t;
		else if(t > max) max = t;
	}
}
