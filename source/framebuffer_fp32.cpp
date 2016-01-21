
#include <stdio.h>
#include <math.h>
#include "lib/color/color.h"
#include "bmp.h"
#include "framebuffer_fp32.h"


void FrameBufferFP32::Saturate()
{
	const std::size_t size = w * h;
	for(std::size_t i = 0; i < size; i++)
	{
		if(data[i].ch[0] < 0.0f) data[i].ch[0] = 0.0f;
		else if(data[i].ch[0] > 1.0f) data[i].ch[0] = 1.0f;
		if(data[i].ch[1] < 0.0f) data[i].ch[1] = 0.0f;
		else if(data[i].ch[1] > 1.0f) data[i].ch[1] = 1.0f;
		if(data[i].ch[2] < 0.0f) data[i].ch[2] = 0.0f;
		else if(data[i].ch[2] > 1.0f) data[i].ch[2] = 1.0f;
	}
}

void FrameBufferFP32::GammaCorrection(float gamma)
{
	const float inv_gamma = 1.0f / gamma;
	const std::size_t size = w * h;
	for(std::size_t i = 0; i < size; i++)
	{
		data[i].ch[0] = powf(data[i].ch[0], inv_gamma);
		data[i].ch[1] = powf(data[i].ch[1], inv_gamma);
		data[i].ch[2] = powf(data[i].ch[2], inv_gamma);
	}
}

bool FrameBufferFP32::WriteBmpFile(const std::string& filename)
{
	const std::size_t width = w;
	const std::size_t height = h;

	bmp::Buffer buff;
	if(!buff.allocBuffer((unsigned long)width, (unsigned long)height, bmp::BITPERPIXEL_24))
		return false;

	for(std::size_t _h = 0; _h < height; _h++)
	{
		const FrameBufferFP32::Data* pixel = ptr(_h);
		unsigned char* p = buff[(unsigned long)_h];
		for(std::size_t _w = 0; _w < width; _w++)
		{
			p[0] = (unsigned char)(pixel->ch[2] * 255.0f);
			p[1] = (unsigned char)(pixel->ch[1] * 255.0f);	
			p[2] = (unsigned char)(pixel->ch[0] * 255.0f);	
			p+=3;
			pixel++;
		}
	}
	
	return SaveToFile(filename.c_str(), &buff, bmp::FILEFORM_WINDOWS);
}

void FrameBufferFP32::Exposure(float k)
{
	const float K_RGB2XYZ[3][3] = {{ 0.4124f, 0.3576f, 0.1805f},
								   { 0.2126f, 0.7152f, 0.0722f},
								   { 0.0193f, 0.1192f, 0.9505f}};
	const float K_XYZ2RGB[3][3] = {{ 3.2410f,-1.5374f,-0.4986f},
								   {-0.9692f, 1.8760f, 0.0416f},
								   { 0.0556f,-0.2040f, 1.5070f}};

	Color rgb, xyz, yxy;
	const std::size_t size = w * h;
	for(std::size_t i = 0; i < size; i++)
	{
		ColorSet(&rgb, data[i].ch[0], data[i].ch[1], data[i].ch[2]);
		ColorConv(&xyz, &rgb, K_RGB2XYZ);
		ColorConvXYZtoYxy(&yxy, &xyz);
		yxy.r = 1.0f - expf(-yxy.r * k);
		ColorConvYxytoXYZ(&xyz, &yxy);
		ColorConv(&rgb, &xyz, K_XYZ2RGB);

		data[i].ch[0] = rgb.r;
		data[i].ch[1] = rgb.g;
		data[i].ch[2] = rgb.b;
	}
}
