
#include "lib/math/random.h"
#include "reflection.h"

/*!
	@brief		���ځA�]�@���x�N�g�������߂�
	@param[o]	t: ���ڃx�N�g��
	@param[o]	b: �]�@���x�N�g��
	@param[i]	n: �@���x�N�g��
 */
void calc_tangent_binormal(Vector3* t, Vector3* b, const Vector3* n)
{
	int i = -1;
	float val, min = FLT_MAX;

	val = fabs(n->x);
	if(val < min){ i = 0; min = val; }
	val = fabs(n->y);
	if(val < min){ i = 1; min = val; }
	val = fabs(n->z);
	if(val < min){ i = 2; min = val; }

	switch(i)
	{
	case 0:
		t->set(0.0f, -n->z, n->y);
		break;
	case 1:
		t->set(-n->z, 0.0f, n->x);
		break;
	default:
		t->set(-n->y, n->x, 0.0f);
		break;
	}
	Vec3Normalize(t, t);
	Vec3OuterProduct(b, n, t);
}

/*!
	@brief		�������̃����_���x�N�g��
	@param[o]	v: �����_���x�N�g��
	@param[i]	n: �@���x�N�g��
 */
void random_vector_cosweight(Vector3* v, const Vector3* n)
{
	float r1 = (float)genrand_real1();	// 0.0 �` 1.0
	float r2 = (float)genrand_real1();	// 0.0 �` 1.0
	float theta = acosf(sqrtf(r1));
	float phi = PI2 * r2;

	Vector3 t, b, r;
	calc_tangent_binormal(&t, &b, n);

	r.x = sinf(theta) * cosf(phi);
	r.y = sinf(theta) * sinf(phi);
	r.z = cosf(theta);
	
	// D = T*sin��cos�� + B*sin��sin�� + N*cos��
	Vec3OrthogonalTransform(v, &r, &t, &b, n);
}

/*!
	@brief		�����˃x�N�g�������߂�
	@param[o]	r: ���˃x�N�g��
	@param[i]	in:���˃x�N�g��(�\�ߔ��]���Ă�������)
	@param[i]	n: �@���x�N�g��
 */
void calc_reflection(Vector3* r, const Vector3* in, const Vector3* n)
{
	float dot = Vec3InnerProduct(in, n);
	Vec3Scale(r, n, dot * 2.0f);
	Vec3Subtract(r, r, in);
}

/*!
	@brief		�����_�����˃x�N�g��
	@param[o]	v: �����_���x�N�g��
	@param[i]	in:���˃x�N�g��(�\�ߔ��]���Ă�������)
	@param[i]	n: �@���x�N�g��
	@param[i]	shine: 
 */
void random_vector_cosweight(Vector3* v, const Vector3* in, const Vector3* n, float shine)
{
	float r1 = (float)genrand_real1();	// 0.0 �` 1.0
	float r2 = (float)genrand_real1();	// 0.0 �` 1.0
	
	float cos_theta = powf(r1, 1.0f / (shine + 1.0f));
	float sin_theta = sqrtf(1.0f - cos_theta * cos_theta);
	float phi = PI2 * r2;

	Vector3 t, b, r;
	calc_tangent_binormal(&t, &b, in);

	r.x = sin_theta * cosf(phi);
	r.y = sin_theta * sinf(phi);
	r.z = cos_theta;

	// D = T*sin��cos�� + B*sin��sin�� + N*cos��
	Vector3 rin;
	Vec3OrthogonalTransform(&rin, &r, &t, &b, in);

	calc_reflection(v, &rin, n);
}
