//==============================================================================
/*!
	@file	common.h
    @author M.Morimoto
    @date   2010/03/17
 */
//==============================================================================
#ifndef __COMMON_H_
#define __COMMON_H_

#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr)		if(ptr){ delete ptr; ptr = NULL; }
#endif // !SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(ptr)	if(ptr){ delete[] ptr; ptr = NULL; }
#endif // !SAFE_DELETE_ARRAY

#endif // !__COMMON_H_