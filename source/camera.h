//==============================================================================
/*!
	@file	camera.h
	@brief	ÉJÉÅÉâ
    @author M.Morimoto
    @date   2010/03/17
 */
//==============================================================================
#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "lib/math/matrix.h"
#include "framebuffer_fp32.h"
#include "ray.h"

class Camera
{
public:
	Camera();
	~Camera();

	void ShootRay(Ray& ray, float sx, float sy);
	Matrix44& GetPosture(){ return posture; }
	FrameBufferFP32& GetFrameBuffer(){ return fb; }

	void SetFocalLength(float flen);
	void SetFStop(float fstop);
	void SetFocalPlane(float fplane){ focal_plane = fplane; }

private:
	Matrix44		posture;
	float			focal_length;	//!< è≈ì_ãóó£
	float			half_fov_h;		//!< êÖïΩéãñÏäpÇÃîºï™
	float			half_fov_v;		//!< êÇíºéãñÏäpÇÃîºï™
	float			f_stop;			//!< F íl
	float			focal_plane;	//!< éBâeãóó£(çáè≈à íu)
	float			aperture;		//!< ÉåÉìÉYå˚åa
	FrameBufferFP32	fb;
};

#endif // !__CAMERA_H_