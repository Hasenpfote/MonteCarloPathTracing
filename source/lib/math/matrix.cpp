//==================================================================================
/*!
	@file	matrix.cpp
    @brief  Matrix 操作
    @author M.Morimoto
    @date   2004/06/11
*/
//==================================================================================
#include <iostream>
#include <iomanip>
#include <memory.h>		// for memcpy
#include "matrix.h"

using namespace std;

//----------------------------------------------------------------------------------
/*!
	@brief		コピーコンストラクタ
	@param[i]	m:
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44::Matrix44(const Matrix44& m)
{
	*this = m;
}

//----------------------------------------------------------------------------------
/*!
	@brief		コピーコンストラクタ
	@param[i]	m:
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44::Matrix44(const float* m)
{
	ASSERT_MSG(m != NULL, "Matrix44::Matrix44()");
	for(int i = 0; i < 16; i++){ m1[i] = m[i]; }
}

//----------------------------------------------------------------------------------
/*!
	@brief		コピーコンストラクタ
	@param[i]	_11 :
	@param[i]	_12 :
	@param[i]	_13 :
	@param[i]	_14 :
	@param[i]	_21 :
	@param[i]	_22 :
	@param[i]	_23 :
	@param[i]	_24 :
	@param[i]	_31 :
	@param[i]	_32 :
	@param[i]	_33 :
	@param[i]	_34 :
	@param[i]	_41 :
	@param[i]	_42 :
	@param[i]	_43 :
	@param[i]	_44 :
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44::Matrix44(float _11, float _12, float _13, float _14,
				   float _21, float _22, float _23, float _24,
				   float _31, float _32, float _33, float _34,
				   float _41, float _42, float _43, float _44)
{
	this->_11 = _11; this->_12 = _12; this->_13 = _13; this->_14 = _14;
	this->_21 = _21; this->_22 = _22; this->_23 = _23; this->_24 = _24;
	this->_31 = _31; this->_32 = _32; this->_33 = _33; this->_34 = _34;
	this->_41 = _41; this->_42 = _42; this->_43 = _43; this->_44 = _44;
}

//----------------------------------------------------------------------------------
/*!
	@brief		代入オペレータ
	@param[i]	m:
	@retval		参照
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44& Matrix44::operator  = (const Matrix44& m)
{
	if( this != &m ){ for(int i = 0; i < 16; i++){ m1[i] = m.m1[i]; } }
	return *this;
}

//----------------------------------------------------------------------------------
/*!
	@brief		加算代入オペレータ
	@param[i]	m:
	@retval		参照
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44& Matrix44::operator += (const Matrix44& m)
{
	for(int i = 0; i < 16; i++){ m1[i] += m.m1[i]; }
	return *this;
}

//----------------------------------------------------------------------------------
/*!
	@brief		減算代入オペレータ
	@param[i]	m:
	@retval		参照
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44& Matrix44::operator -= (const Matrix44& m)
{
	for( int i = 0; i < 16; i++ ){ m1[i] -= m.m1[i]; }
	return *this;
}

//----------------------------------------------------------------------------------
/*!
	@brief		積算代入オペレータ
	@param[i]	m:
	@retval		参照
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44& Matrix44::operator *= (const Matrix44& m)
{
	(*this) = (*this) * m;
	return *this;
}

//----------------------------------------------------------------------------------
/*!
	@brief		スケール代入オペレータ
	@param[i]	s:
	@retval		参照
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44& Matrix44::operator *= (float s)
{
	for(int i = 0; i < 16; i++){ m1[i] *= s; }
	return *this;
}

//----------------------------------------------------------------------------------
/*!
	@brief		加算オペレータ
	@param[i]	m:
	@retval		加算結果
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
const Matrix44 Matrix44::operator + (const Matrix44& m) const
{
	Matrix44	temp;

	for(int i = 0; i < 16; i++)
		temp.m1[i] = m1[i] + m.m1[i];
	return temp;
}

//----------------------------------------------------------------------------------
/*!
	@brief		減算オペレータ
	@param[i]	m:
	@retval		減算結果
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
const Matrix44 Matrix44::operator - (const Matrix44& m) const
{
	Matrix44	temp;

	for(int i = 0; i < 16; i++)
		temp.m1[i] = m1[i] - m.m1[i];
	return temp;
}

//----------------------------------------------------------------------------------
/*!
	@brief		積算オペレータ
	@param[i]	m:
	@retval		積算結果
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
const Matrix44 Matrix44::operator * (const Matrix44& m) const
{
	int			i, j, k;
	Matrix44	temp;

	Mtx44Zero(&temp);

	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			for(k = 0; k < 4; k++)
				temp.m2[i][j] += m2[i][k] * m.m2[k][j];
	return temp;
}

//----------------------------------------------------------------------------------
/*!
	@brief		スケールオペレータ
	@param[i]	s:
	@retval		スケール結果
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
const Matrix44 Matrix44::operator * (float s) const
{
	Matrix44 temp;

	for(int i = 0; i < 16; i++)
		temp.m1[i] = m1[i] * s;
	return temp;
}

//----------------------------------------------------------------------------------
/*!
	@brief		スケールオペレータ
	@param[i]	s:
	@param[i]	m:
	@retval		スケール結果
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
const Matrix44 operator * (float s, const Matrix44& m)
{
	Matrix44 temp;

	for(int i = 0; i < 16; i++)
		temp.m1[i] = s * m.m1[i];
	return temp;
}

//----------------------------------------------------------------------------------
/*!
	@brief		比較オペレータ
	@param[o]	m:
	@retval		true  : ＝
	@retval		false : ≠
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
bool Matrix44::operator == (const Matrix44& m) const
{
	return
	( _11 == m._11 && _12 == m._12 && _13 == m._13 && _14 == m._14 &&
	  _21 == m._21 && _22 == m._22 && _23 == m._23 && _24 == m._24 &&
	  _31 == m._31 && _32 == m._32 && _33 == m._33 && _34 == m._34 &&
	  _41 == m._41 && _42 == m._42 && _43 == m._43 && _44 == m._44 );
}

//----------------------------------------------------------------------------------
/*!
	@brief		比較オペレータ
	@param[o]	m:
	@retval		true  : ≠
	@retval		false : ＝
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
bool Matrix44::operator != (const Matrix44& m) const
{
	return
	( _11 != m._11 || _12 != m._12 || _13 != m._13 || _14 != m._14 ||
	  _21 != m._21 || _22 != m._22 || _23 != m._23 || _24 != m._24 ||
	  _31 != m._31 || _32 != m._32 || _33 != m._33 || _34 != m._34 ||
	  _41 != m._41 || _42 != m._42 || _43 != m._43 || _44 != m._44 );
}

//----------------------------------------------------------------------------------
/*!
	@brief		デバッグ出力
	@note
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
void Matrix44::debug_print() const
{
	cout << "< Matrix44 >" << endl
		 << setprecision( 6 )
		 << setiosflags( ios::fixed );

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			cout << "[" << (i+1) << (j+1) <<"]" << setw(15) << m2[i][j] << " ";
		}
		cout << endl;
	}
}

//----------------------------------------------------------------------------------
/*!
	@brief		0 行列
	@param[o]	m:
	@retval		m と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44Zero(Matrix44* m)
{
	ASSERT_MSG(m != NULL, "Mtx44Zero()");

	for(int i = 0; i < 16; i++)
		m->m1[i] = 0.0f;
	return m;
}

//----------------------------------------------------------------------------------
/*!
	@brief		単位行列
	@param[o]	m:
	@retval		m と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44Identity(Matrix44* m)
{
	ASSERT_MSG(m != NULL, "Mtx44Identity()");

	Mtx44Zero(m);
	for(int i = 0; i < 4; i++)
		m->m2[i][i] = 1.0f;
	return m;
}

//----------------------------------------------------------------------------------
/*!
	@brief		単位行列判定
	@param[o]	m:
	@retval		true  : 単位行列である
	@retval		false : 単位行列でない
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
bool Mtx44IsIdentity(const Matrix44* m)
{
	ASSERT_MSG(m != NULL, "Mtx44IsIdentity()");

	if( m->_11 == 1.0f && m->_12 == 0.0f && m->_13 == 0.0f && m->_14 == 0.0f
	 && m->_21 == 0.0f && m->_22 == 1.0f && m->_23 == 0.0f && m->_24 == 0.0f
	 && m->_31 == 0.0f && m->_32 == 0.0f && m->_33 == 1.0f && m->_34 == 0.0f
	 && m->_41 == 0.0f && m->_42 == 0.0f && m->_43 == 0.0f && m->_44 == 1.0f )
	 return true;

	return false;
}

//----------------------------------------------------------------------------------
/*!
	@brief		加算
	@param[o]	out: m1 + m2 の結果
	@param[i]	m1:
	@param[i]	m2:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44Add(Matrix44* out, const Matrix44* m1, const Matrix44* m2)
{
	ASSERT_MSG(out != NULL, "Mtx44Add()");
	ASSERT_MSG(m1  != NULL, "Mtx44Add()");
	ASSERT_MSG(m2  != NULL, "Mtx44Add()");

	for(int i = 0; i < 16; i++)
		out->m1[i] = m1->m1[i] + m2->m1[i];
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		減算
	@param[o]	out: m1 - m2 の結果
	@param[i]	m1:
	@param[i]	m2:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44Subtract(Matrix44* out, const Matrix44* m1, const Matrix44* m2)
{
	ASSERT_MSG(out != NULL, "Mtx44Subtract()");
	ASSERT_MSG(m1  != NULL, "Mtx44Subtract()");
	ASSERT_MSG(m2  != NULL, "Mtx44Subtract()");

	for(int i = 0; i < 16; i++)
		out->m1[i] = m1->m1[i] - m2->m1[i];
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		積算
	@param[o]	out: m1 * m2 の結果
	@param[i]	m1:
	@param[i]	m2:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44Mul(Matrix44* out, const Matrix44* m1, const Matrix44* m2)
{
	ASSERT_MSG(out != NULL, "Mtx44Mul()");
	ASSERT_MSG(m1  != NULL, "Mtx44Mul()");
	ASSERT_MSG(m2  != NULL, "Mtx44Mul()");

	int			i, j, k;
	Matrix44	temp;

	Mtx44Zero(&temp);

	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			for(k = 0; k < 4; k++)
				temp.m2[i][j] += m1->m2[i][k] * m2->m2[k][j];

	(*out) = temp;

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		スケール
	@param[o]	out: m * s の結果
	@param[i]	m:
	@param[i]	s:
	@retval		out と同じ
	@note
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44Scale(Matrix44* out, const Matrix44* m, float s)
{
	ASSERT_MSG(out != NULL, "Mtx44Scale()");
	ASSERT_MSG(m   != NULL, "Mtx44Scale()");

	for(int i = 0; i < 16; i++)
		out->m1[i] = m->m1[i] * s;
	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		転置行列
	@param[o]	out: 転置後
	@param[i]	m: 転置前
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44Transpose(Matrix44* out, const Matrix44* m)
{
	ASSERT_MSG(out != NULL, "Mtx44Transpose()");
	ASSERT_MSG(m   != NULL, "Mtx44Transpose()");

	Matrix44  temp;
	Matrix44* p;

	if(out == m)	p = &temp;
	else			p = out;

	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; i++)
			p->m2[i][j] = m->m2[j][i];

	p->m2[0][3] = p->m2[1][3] = p->m2[2][3] = 0.0f;

	p->m2[3][0] = -( m->m2[3][0] * p->m2[0][0] + m->m2[3][1] * p->m2[1][0] + m->m2[3][2] * p->m2[2][0] );
	p->m2[3][1] = -( m->m2[3][0] * p->m2[0][1] + m->m2[3][1] * p->m2[1][1] + m->m2[3][2] * p->m2[2][1] );
	p->m2[3][2] = -( m->m2[3][0] * p->m2[0][2] + m->m2[3][1] * p->m2[1][2] + m->m2[3][2] * p->m2[2][2] );
	p->m2[3][3] = 1.0f;

	// overwrite mtx if needed
	if(p == &temp)
		*out = temp;

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		逆行列
	@param[o]	out: 出力
	@param[i]	m:
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44Inverse(Matrix44* out, const Matrix44* m)
{
	int		x, y, y2;
	int		offset;
	float	pOut[16],
			pGauss[16];
	float	coef[3];	// 行列の要素を0にするときに使う係数

	// 出力側を単位行列に
	memset(pOut, 0, sizeof( float ) * 16);
	pOut[0] = pOut[5] = pOut[10] = pOut[15] = 1.0;

	// copy
	memcpy(pGauss, m, sizeof(Matrix44));

	// 対角要素を１にする、対角要素の下半分を0にする
	for(y = 0; y < 4; y++)
	{
		offset = y*4+y;
		coef[0] = pGauss[offset];	// 対角要素を係数にする

		if(coef[0] < FLT_EPSILON && coef[0] > -FLT_EPSILON)
			return NULL;			// 係数が極小の時は逆行列計算不可能

		for(x = 0; x < 4; x++)
		{
			offset = y*4+x;
			pGauss[offset] /= coef[0];
			pOut[offset]   /= coef[0];
		}
		// 注目している対角要素の下の行に関するループ
		for(y2 = y+1; y2 < 4; y2++)
		{
			offset = y2*4+y;
			coef[1] = pGauss[offset];	// 注目している対角要素の下にある要素を係数とす

			for(x = 0; x < 4; x++)		// 注目している行の列に対するループ
			{
				offset = y2*4+x;
				int offset2 = y*4+x;
				pGauss[offset] -= pGauss[offset2] *	coef[1];
				pOut[offset]   -= pOut[offset2]	  *	coef[1];
			}
		}
	}
	// 対角要素の上半分を0にするループ
	for(y = 3; y >= 0; y--)
		for(y2 = y - 1; y2 >= 0; y2--)
		{
			offset  = y2*4+y;
			coef[2] = pGauss[offset];
			for(x = 0; x < 4; x++)
			{
				offset = y2*4+x;
				int offset2 = y*4+x;
				pGauss[offset] -= pGauss[offset2] *	coef[2];
				pOut[offset]   -= pOut[offset2]	  *	coef[2];
			}
		}

	memcpy(out, pOut, sizeof(float)*16);

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		平行移動行列
	@param[o]	out: 出力
	@param[i]	x: x 軸
	@param[i]	y: y 軸
	@param[i]	z: z 軸
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44Translation(Matrix44* out, float x, float y, float z)
{
	ASSERT_MSG(out != NULL, "Mtx44Translation()");

	Mtx44Identity(out);

	out->_41 = x;
	out->_42 = y;
	out->_43 = z;

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		スケール行列
	@param[o]	out: 出力
	@param[i]	x: x 軸
	@param[i]	y: y 軸
	@param[i]	z: z 軸
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44Scaling(Matrix44* out, float x, float y, float z)
{
	ASSERT_MSG(out != NULL, "Mtx44Scaling()");

	Mtx44Identity(out);

	out->_11 = x;
	out->_22 = y;
	out->_33 = z;

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		x 軸回転行列
	@param[o]	out: 出力
	@param[i]	theta: 角度(ラジアン)
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44RotationX(Matrix44* out, float theta)
{
	ASSERT_MSG(out != NULL, "Mtx44RotationX()");

	const float s = sinf(theta);
	const float c = cosf(theta);

	Mtx44Identity(out);

	out->_22 = c;
	out->_23 = s;
	out->_32 =-s;
	out->_33 = c;

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		y 軸回転行列
	@param[o]	out: 出力
	@param[i]	theta: 角度(ラジアン)
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44RotationY(Matrix44* out, float theta)
{
	ASSERT_MSG(out != NULL, "Mtx44RotationY()");

	const float s = sinf(theta);
	const float c = cosf(theta);

	Mtx44Identity(out);

	out->_11 = c;
	out->_13 =-s;
	out->_31 = s;
	out->_33 = c;

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		z 軸回転行列
	@param[o]	out: 出力
	@param[i]	theta: 角度(ラジアン)
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44RotationZ(Matrix44* out, float theta)
{
	ASSERT_MSG(out != NULL, "Mtx44RotationZ()");

	const float s = sinf(theta);
	const float c = cosf(theta);

	Mtx44Identity(out);

	out->_11 = c;
	out->_12 = s;
	out->_21 =-s;
	out->_22 = c;

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		回転行列
	@param[o]	out: 出力
	@param[i]	yaw:	ヨー回転角度(ラジアン)
	@param[i]	pitch: ピッチ回転角度(ラジアン)
	@param[i]	roll: ロール回転角度(ラジアン)
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44RotationYawPitchRoll(Matrix44* out, float yaw, float pitch, float roll)
{
	ASSERT_MSG(out != NULL, "Mtx44RotationYawPitchRoll()");

	const float	zs = sinf(roll);
	const float	zc = cosf(roll);
	const float	xs = sinf(pitch);
	const float	xc = cosf(pitch);
	const float	ys = sinf(yaw);
	const float	yc = cosf(yaw);

	out->_11 = zc * yc + zs * xs * ys;
	out->_12 = zs * xc;
	out->_13 =-zc * ys + zs * xs * yc;
	out->_14 = 0.0f;

	out->_21 =-zs * yc + zc * xs * ys;
	out->_22 = zc * xc;
	out->_23 = zs * ys + zc * xs * yc;
	out->_24 = 0.0f;

	out->_31 = xc * ys;
	out->_32 =-xs;
	out->_33 = xc * yc;
	out->_34 = 0.0f;

	out->_41 = 0.0f;
	out->_42 = 0.0f;
	out->_43 = 0.0f;
	out->_44 = 1.0f;

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		視錐体行列
	@param[o]	out: 出力
	@param[i]	t:
	@param[i]	b:
	@param[i]	l:
	@param[i]	r:
	@param[i]	n: 近面
	@param[i]	f: 遠面
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44Frustum(Matrix44* out, float t, float b, float l, float r, float n, float f)
{
	ASSERT_MSG( out != NULL, "Mtx44Frustum()" );
    ASSERT_MSG( t != b, "Mtx44Frustum() : 't' and 'b' clipping planes are equal" );
    ASSERT_MSG( l != r, "Mtx44Frustum() : 'l' and 'r' clipping planes are equal" );
    ASSERT_MSG( n != f, "Mtx44Frustum() : 'n' and 'f' clipping planes are equal" );

	float w, h, q, woff, hoff, temp;

	temp = 2.0f * n;
	w	 = temp / (r-l);
	h	 = temp / (t-b);
	q	 = f / (f-n);
	woff = -(r+l) / (r-l);
	hoff = -(t+b) / (t-b);

	Mtx44Zero( out );

	out->_11 = w;
	out->_22 = h;
	out->_31 = woff;
	out->_32 = hoff;
	out->_33 = q;
	out->_34 = 1.0f;
	out->_43 = -n * q;
//	out->_44 = 0.0f;

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		射影行列
	@param[o]	out: 出力
	@param[i]	fovy: y 軸視野角(ラジアン)
	@param[i]	aspect: アスペクト比
	@param[i]	n: 近面
	@param[i]	f: 遠面
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44Perspective(Matrix44* out, float fovy, float aspect, float n, float f)
{
	ASSERT_MSG(out != NULL, "Mtx44Perspective()");
	ASSERT_MSG((fovy>0.0f)&&(fovy<PI), "Mtx44Perspective() : 'fovy' out of range");
	ASSERT_MSG(aspect > 0.0f, "Mtx44Perspective() : 'aspect' is 0");
    ASSERT_MSG(n != f, "Mtx44Perspective() : 'n' and 'f' clipping planes are equal");

	const float fovyh = fovy * 0.5f;
	const float cot   = 1.0f / tanf(fovyh);
	const float q     = f / (f - n);

	Mtx44Zero(out);

	out->_11 = cot / aspect;
	out->_22 = cot;
	out->_33 = q;
	out->_34 = 1.0f;
	out->_43 = -n * q;
//	out->_44 = 0.0f;

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		正投影行列
	@param[o]	out: 出力
	@param[i]	t:
	@param[i]	b:
	@param[i]	l:
	@param[i]	r:
	@param[i]	n: 近面
	@param[i]	f: 遠面
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44Ortho(Matrix44* out, float t, float b, float l, float r, float n, float f)
{
	ASSERT_MSG(out != NULL, "Mtx44Ortho()" );
    ASSERT_MSG(t != b, "Mtx44Ortho() : 't' and 'b' clipping planes are equal");
    ASSERT_MSG(l != r, "Mtx44Ortho() : 'l' and 'r' clipping planes are equal");
    ASSERT_MSG(n != f, "Mtx44Ortho() : 'n' and 'f' clipping planes are equal");

	float w, h, q, woff, hoff;

	w = 2.0f / (r-l);
	h = 2.0f / (t-b);
	q = 1.0f / (f-n);
	woff = -(r+l) / (r-l);
	hoff = -(t+b) / (t-b);

	Mtx44Zero(out);

	out->_11 = w;
	out->_22 = h;
	out->_33 = q;
	out->_41 = woff;
	out->_42 = hoff;
	out->_43 = -n * q;
	out->_44 = 1.0f;

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief
	@param[o]	out: 出力
	@param[i]	fovy: y 軸視野角(ラジアン)
	@param[i]	aspect: アスペクト比
	@param[i]	n: 近面
	@param[i]	f: 遠面
	@param[i]	pixdx: x 方向のピクセルずらし量
	@param[i]	pixdy: y 方向のピクセルずらし量
	@param[i]	vpw: ビューポートの幅
	@param[i]	vph: ビューポートの高さ
	@retval		out と同じ
	@note
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44AdjProjection( Matrix44* out, float fovy, float aspect, float n, float f, float pixdx, float pixdy, float vpw, float vph)
{
	ASSERT_MSG(out != NULL, "Mtx44AdjProjection()");
	ASSERT_MSG((fovy>0.0f)&&(fovy<PI), "Mtx44AdjProjection() : 'fovy' out of range");
	ASSERT_MSG(aspect > 0.0f, "Mtx44AdjProjection() : 'aspect' is 0");
    ASSERT_MSG(n != f, "Mtx44AdjProjection() : 'n' and 'f' clipping planes are equal");

	const float fovyh = fovy * 0.5f;	// harf fovy
	const float scale = tanf(fovyh) * n;
	float t, b, l, r, w, h, dx, dy;

	t = scale;
	b =-scale;
	l =-scale * aspect;
	r = scale * aspect;
	w = r - l;
	h = b - t;
	dx = -(pixdx * w / vpw);
	dy = -(pixdy * h / vph);
	Mtx44Frustum(out, t+dy, b+dy, l+dx, r+dx, n, f);

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		光源視錐体行列
	@param[o]	out: 出力
	@param[i]	t:
	@param[i]	b:
	@param[i]	l:
	@param[i]	r:
	@param[i]	n: 近面
	@param[i]	scaleS: S 座標のスケール量
	@param[i]	scaleT: T 座標のスケール量
	@param[i]	transS: S 座標の平行移動量
	@param[i]	transT: T 座標の平行移動量
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44LightingFrustum(Matrix44* out, float t, float b, float l, float r, float n, float scaleS, float scaleT, float transS, float transT)
{
	ASSERT_MSG(out != NULL, "Mtx44LightingFrustum()");
    ASSERT_MSG(t != b, "Mtx44LightingFrustum() : 't' and 'b' clipping planes are equal");
    ASSERT_MSG(l != r, "Mtx44LightingFrustum() : 'l' and 'r' clipping planes are equal");

	float w, h, woff, hoff;
	float temp;

	temp = 2.0f * n;
	w	 = (temp / (r-l)) * scaleS;
	h	 = (temp / (t-b)) * scaleT;
	woff = (-(r+l) / (r-l)) * scaleS + transS;
	hoff = (-(t+b) / (t-b)) * scaleT + transT;

	Mtx44Zero(out);

	out->_11 = w;
	out->_22 = h;
	out->_31 = woff;
	out->_32 = hoff;
	out->_33 = 1.0f;
	out->_44 = 1.0f;

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		光源射影行列
	@param[o]	out: 出力
	@param[i]	fovy: y 軸視野角(ラジアン)
	@param[i]	aspect: アスペクト比
	@param[i]	scaleS: S 座標のスケール量
	@param[i]	scaleT: T 座標のスケール量
	@param[i]	transS: S 座標の平行移動量
	@param[i]	transT: T 座標の平行移動量
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44LightingPerspective(Matrix44* out, float fovy, float aspect, float scaleS, float scaleT, float transS, float transT)
{
	ASSERT_MSG(out != NULL, "Mtx44LightingPerspective()");
	ASSERT_MSG((fovy>0.0f)&&(fovy<PI), "Mtx44LightingPerspective() : 'fovy' out of range");
	ASSERT_MSG(aspect > 0.0f, "Mtx44LightingPerspective() : 'aspect' is 0");

	const float fovyh = fovy * 0.5f;
	const float cot   = 1.0f / tanf(fovyh);

	Mtx44Zero(out);

	out->_11 = (cot/aspect)*scaleS;
	out->_22 = cot*scaleT;
	out->_31 = transS;
	out->_32 = transT;
	out->_33 = 1.0f;
	out->_44 = 1.0f;

	return out;
}

//----------------------------------------------------------------------------------
/*!
	@brief		光源正投影行列
	@param[o]	out: 出力
	@param[i]	t:
	@param[i]	b:
	@param[i]	l:
	@param[i]	r:
	@param[i]	scaleS: S 座標のスケール量
	@param[i]	scaleT: T 座標のスケール量
	@param[i]	transS: S 座標の平行移動量
	@param[i]	transT: T 座標の平行移動量
	@retval		out と同じ
    @author		M.Morimoto
    @date
*/
//----------------------------------------------------------------------------------
Matrix44* Mtx44LightingOrtho(Matrix44* out, float t, float b, float l, float r, float scaleS, float scaleT, float transS, float transT)
{
	ASSERT_MSG(out != NULL, "Mtx44LightingOrtho()");
    ASSERT_MSG(t != b, "Mtx44LightingOrtho() : 't' and 'b' clipping planes are equal");
    ASSERT_MSG(l != r, "Mtx44LightingOrtho() : 'l' and 'r' clipping planes are equal");

	float w, h, woff, hoff;

	w	 = (2.0f / (r-l)) * scaleS;
	h	 = (2.0f / (t-b)) * scaleT;
	woff = (-(r+l) / (r-l)) * scaleS + transS;
	hoff = (-(t+b) / (t-b)) * scaleT + transT;

	Mtx44Zero(out);

	out->_11 = w;
	out->_22 = h;
	out->_41 = woff;
	out->_42 = hoff;
	out->_43 = 1.0f;
	out->_44 = 1.0f;

	return out;
}
