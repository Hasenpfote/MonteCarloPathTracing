//==============================================================================
/*!
	@file	config.h
	@brief	ê›íËä÷òA
    @author M.Morimoto
    @date   2010/03/17
 */
//==============================================================================
#ifndef __CONFIG_H_
#define __CONFIG_H_

#define USE_MULTI_THREAD
#define USE_LOCAL_ILLUMINATION
#define USE_GLOBAL_ILLUMINATION
#define USE_PERF_CHECK
#define USE_KDTREE
#define USE_OCCLUSION_TEST
#define USE_DOF_BLUR
#define USE_ENV_FILE

#define SCR_WIDTH			360
#define SCR_HEIGHT			240
#define MAX_DEPTH			3
#define MAX_SAMPLING		300
#define MAX_KDTREE_DEPTH	5

#endif // !__CONFIG_H_