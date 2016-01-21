//==============================================================================
/*!
	@file	environment.h
	@brief	ŠÂ‹«
    @author M.Morimoto
    @date   2010/03/17
 */
//==============================================================================
#ifndef __ENVIRONMENT_H_
#define __ENVIRONMENT_H_

#include <string>

struct Environment
{
	unsigned long	scr_width;
	unsigned long	scr_height;
	float			bg_r, bg_g, bg_b;
	unsigned long	depth;
	unsigned long	sample;
	unsigned long	kd_depth;
	float			focal_length;
	float			f_stop;
	float			focal_plane;
	unsigned long	flag;
	float			eye_x, eye_y, eye_z;
	float			at_x, at_y, at_z;
};

bool LoadEnvironmentFile(Environment* env, const std::string& filename);

#endif // __ENVIRONMENT_H_
