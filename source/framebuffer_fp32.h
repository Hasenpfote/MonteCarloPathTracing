//==================================================================================
/*!
	@file	framebuffer_fp32.h
    @brief  fp32 フレームバッファ
    @author M.Morimoto
    @date   2010/03/09
 */
//==================================================================================
#ifndef __FRAMEBUFFER_FP32_H_
#define __FRAMEBUFFER_FP32_H_

#include <string>
#include "lib/system/framebuffer.h"

class FrameBufferFP32 : public FrameBuffer<float, 3>
{
public:
	FrameBufferFP32(){}
	~FrameBufferFP32(){}

	void Saturate();
	void GammaCorrection(float gamma = 2.2f);
	bool WriteBmpFile(const std::string& filename);

	void Exposure(float k = 1.0f);
};

#endif // !__FRAMEBUFFER_FP32_H_