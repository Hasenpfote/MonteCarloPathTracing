//==============================================================================
/*!
	@file	color.h
	@brief	êFä÷òA
 */
//==============================================================================
#ifndef __COLOR_H_
#define __COLOR_H_

/*!
	@brief	êF
	@struct Color
 */
struct Color
{
	union
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		};
		float ch[4];
	};
};

//----------------------------------------------------------------------------------
// 	OPERATIONS
//----------------------------------------------------------------------------------
Color* ColorSet(Color* out, float r, float g, float b, float a = 1.0f);
Color* ColorSaturate(Color* c);
Color* ColorAdd3(Color* out, const Color* c0, const Color* c1);
Color* ColorSubtract3(Color* out, const Color* c0, const Color* c1);
Color* ColorLerp3(Color* out, const Color* c0, const Color* c1, float s);
Color* ColorModulate3(Color* out, const Color* c0, const Color* c1);
Color* ColorScale3(Color* out, const Color* c, float s);

void ColorConv(Color* out, const Color* c, const float weights[3][3]);
void ColorConvXYZtoYxy(Color* out, const Color* c);
void ColorConvYxytoXYZ(Color* out, const Color* c);

#endif