
#include "lib/math/random.h"
#include "config.h"
#include "camera.h"

static const float K_FILM_WIDTH  = 0.036f;
static const float K_FILM_HEIGHT = 0.024f;
static const float K_FILM_HALF_WIDTH  = 0.018f;
static const float K_FILM_HALF_HEIGHT = 0.012f;
static const float K_FILM_ASPECT_RATIO = 1.5f;


/*!
	@brief		�����Y�ʏ�̍��W���擾
	@param[o]	u: u ���W
	@param[o]	v: v ���W
	@param[i]	aperture: ���a
 */
static void GetLensUV(float& u, float&v, float aperture)
{
	const float theta = PI2 * (float)genrand_real1();
	const float r = aperture * (float)genrand_real1() * 0.5f;
	u = r * cosf(theta);
	v = r * sinf(theta);
}

////////////////////////////////////////////////////////////////////////////////

Camera::Camera()
{
	focal_length = 0.05f;
	f_stop = 2.8f;
	focal_plane = 1.0f;
	aperture = focal_length / f_stop;
	half_fov_h = atanf(K_FILM_HALF_WIDTH  / focal_length);
	half_fov_v = atanf(K_FILM_HALF_HEIGHT / focal_length);
}

Camera::~Camera()
{
}

/*!
	@brief		�������Z�o
	@param[o]	ray: ����
	@param[i]	sx: �X�N���[�� x ���W[0..1]
	@param[i]	sy: �X�N���[�� y ���W[0..1]
 */
void Camera::ShootRay(Ray& ray, float sx, float sy)
{
	const float theta = - (2.0f * sy - 1.0f) * half_fov_v;
	const float phi   =   (2.0f * sx - 1.0f) * half_fov_h * (fb.aspect_ratio() / K_FILM_ASPECT_RATIO);
	const float cos_t = cosf(theta);
	Vector3 dir;
	dir.x = cos_t * sinf(phi);
	dir.y =-sinf(theta);
	dir.z = cos_t * cosf(phi);
 #ifdef USE_DOF_BLUR
	// ���Ŗʏ�̈ʒu
	const float t = focal_plane / dir.z;
	Vector3 pos;
	Vec3Scale(&pos, &dir, t);

	// �����Y��̈ʒu
	Vector3 org;
	GetLensUV(org.x, org.y, aperture);
	org.z = 0.0f;

	// ���Ŗʏ�̈ʒu - �����Y��̈ʒu = ����
	Vec3Subtract(&dir, &pos, &org);
	Vec3Normalize(&dir, &dir);

	ray.org.x = org.x * posture._11 + org.y * posture._21 + org.z * posture._31 + posture._41;
	ray.org.y = org.x * posture._12 + org.y * posture._22 + org.z * posture._32 + posture._42;
	ray.org.z = org.x * posture._13 + org.y * posture._23 + org.z * posture._33 + posture._43;
 #else
	ray.org.x = posture._41;
	ray.org.y = posture._42;
	ray.org.z = posture._43;
 #endif // USE_DOF_BLUR
	// �ϊ�
	ray.dir.x = dir.x * posture._11 + dir.y * posture._21 + dir.z * posture._31;
	ray.dir.y = dir.x * posture._12 + dir.y * posture._22 + dir.z * posture._32;
	ray.dir.z = dir.x * posture._13 + dir.y * posture._23 + dir.z * posture._33;
}

/*!
	@brief		�œ_�����̐ݒ�
	@param[i]	flen: �œ_����
	@note		����p�ƌ��a���Z�o����
 */
void Camera::SetFocalLength(float flen)
{
	focal_length = flen;
	aperture = flen / f_stop;
	half_fov_h = atanf(K_FILM_HALF_WIDTH  / flen);
	half_fov_v = atanf(K_FILM_HALF_HEIGHT / flen);
}

/*!
	@brief		F �l�̐ݒ�
	@param[i]	fstop: F �l
	@note		���a���Z�o����
 */
void Camera::SetFStop(float fstop)
{
	f_stop = fstop;
	aperture = focal_length / fstop;
}
