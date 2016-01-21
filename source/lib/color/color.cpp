
#include "Color.h"


Color* ColorSet(Color* out, float r, float g, float b, float a)
{
	out->r = r;
	out->g = g;
	out->b = b;
	out->a = a;
	return out;
}

Color* ColorSaturate(Color* c)
{
	if(c->r < 0.0f) c->r = 0.0f;
	else if(c->r > 1.0f) c->r = 1.0f;
	if(c->g < 0.0f) c->g = 0.0f;
	else if(c->g > 1.0f) c->g = 1.0f;
	if(c->b < 0.0f) c->b = 0.0f;
	else if(c->b > 1.0f) c->b = 1.0f;
	if(c->a < 0.0f) c->a = 0.0f;
	else if(c->a > 1.0f) c->a = 1.0f;
	return c;
}

Color* ColorAdd3(Color* out, const Color* c0, const Color* c1)
{
	out->r = c0->r + c1->r;
	out->g = c0->g + c1->g;
	out->b = c0->b + c1->b;
	return out;
}

Color* ColorSubtract3(Color* out, const Color* c0, const Color* c1)
{
	out->r = c0->r - c1->r;
	out->g = c0->g - c1->g;
	out->b = c0->b - c1->b;
	return out;
}

Color* ColorLerp3(Color* out, const Color* c0, const Color* c1, float s)
{
	out->r = c0->r + s * (c1->r - c0->r);
	out->g = c0->g + s * (c1->g - c0->g);
	out->b = c0->b + s * (c1->b - c0->b);
	return out;
}

Color* ColorModulate3(Color* out, const Color* c0, const Color* c1)
{
	out->r = c0->r * c1->r;
	out->g = c0->g * c1->g;
	out->b = c0->b * c1->b;
	return out;
}

Color* ColorScale3(Color* out, const Color* c, float s)
{
	out->r = c->r * s;
	out->g = c->g * s;
	out->b = c->b * s;
	return out;
}

//------------------------------------------------------------------------------
/*!
	@brief		d‚İ‚Â‚«•ÏŠ·
	@param[o]	out: o—Í
	@param[i]	c: “ü—Í
	@param[i]	weights: d‚İ
 */
//------------------------------------------------------------------------------
void ColorConv(Color* out, const Color* c, const float weights[3][3])
{
	for(int i = 0; i < 3; i++)
		out->ch[i] = weights[i][0] * c->ch[0] + weights[i][1] * c->ch[1] + weights[i][2] * c->ch[2];
}

//------------------------------------------------------------------------------
/*!
	@brief		XYZ ‚©‚ç Yxy ‚É•ÏŠ·
	@param[o]	out: Yxy
	@param[i]	c: XYZ
 */
//------------------------------------------------------------------------------
void ColorConvXYZtoYxy(Color* out, const Color* c)
{
	const float w = c->ch[0] + c->ch[1] + c->ch[2];
	if(w > 0.0f)
	{
		out->ch[1] = c->ch[0] / w;
		out->ch[2] = c->ch[1] / w;
	}
	else
	{
		out->ch[1] =
		out->ch[2] = 0.0f;
	}
	out->ch[0] = c->ch[1];
}

//------------------------------------------------------------------------------
/*!
	@brief		Yxy ‚©‚ç XYZ ‚É•ÏŠ·
	@param[o]	out: XYZ
	@param[i]	c: Yxy
 */
//------------------------------------------------------------------------------
void ColorConvYxytoXYZ(Color* out, const Color* c)
{
	if((c->ch[0] > 0.0f) && (c->ch[1] > 0.0f) && (c->ch[2] > 0.0f))
	{
		const float f = c->ch[0] / c->ch[2];
		out->ch[0] = f * c->ch[1];
		out->ch[2] = f * (1.0f - c->ch[1] - c->ch[2]);
	}
	else
	{
		out->ch[0] =
		out->ch[2] = 0.0f;
	}
	out->ch[1] = c->ch[0];
}
