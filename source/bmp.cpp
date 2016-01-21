//----------------------------------------------------------------------------------
/*!
	@file
    @brief  BMP ���샆�[�e�B���e�B
    @author M.Morimoto
    @date   2006/08/25
*/
//----------------------------------------------------------------------------------
#include <stdio.h>
#include <assert.h>
#include "lib/lib_common.h"
#include "bmp.h"

using namespace bmp;

//! 4 �o�C�g�����R�[�h�����}�N��
#define MAKE_FOURCC(a,b,c,d)	(((d) << 24) | ((c) << 16) | ((b) << 8) | (a))
//! 2 �o�C�g�����R�[�h�����}�N��
#define MAKE_TWOCC(a,b)			(((b) << 8) | (a))


namespace bmp
{
//----------------------------------------------------------------------------------
/*!
	@brief		�`���̔���
	@param[in]	size 12:OS/2 40:Windows --:Other
	@retval		FILEFORM
    @author		M.Morimoto
    @date		2006/08/02
*/
//----------------------------------------------------------------------------------
static FILEFORM checkFileForm(long size)
{
	if(size == 12)
		return FILEFORM_OS2;
	else
	if(size == 40)
		return FILEFORM_WINDOWS;
	return FILEFORM_UNKNOWN;
}

//----------------------------------------------------------------------------------
/*!
	@brief		�g�p�F���̔���
	@param[in]	bits  :  1,   4,   8,  16,  24,  32
	@param[in]	color :  2,  16, 256,   0,   0,   0

	@retval		�g�p�F��
	@note		2^32 �� 32 bits �ł͕\���ł��Ȃ����Ƃɒ���
    @author		M.Morimoto
    @date		2006/08/02
*/
//----------------------------------------------------------------------------------
static unsigned long checkUsedColor(unsigned short bits, unsigned long color)
{
	if(color != 0)
		return color;
	return (unsigned long)(1 << bits);
}

//----------------------------------------------------------------------------------
/*!
	@brief		OS/2 �`���œǂݍ���
	@param[in]	pSurf  : �T�[�t�F�X�ւ̃|�C���^
	@param[in]	fp     : �t�@�C���|�C���^
	@param[in]	offset : �t�@�C���擪����摜�f�[�^�܂ł̃I�t�Z�b�g (byte)
	@retval		true  : ����
	@retval		false : ���s
	@note
    @author		M.Morimoto
    @date		2006/08/02
*/
//----------------------------------------------------------------------------------
static bool loadFromFileByOS2(Buffer* pBuff, FILE* fp, unsigned long offset)
{
	BITMAPCOREHEADER bcHead;							//
	unsigned long    color;								// �g�p�F��
	bool			 bRet;


	//------------------------------------------------------------------------------
	// ���w�b�_
	//------------------------------------------------------------------------------
	fread(&bcHead, sizeof(BITMAPCOREHEADER), 1, fp);	// ���w�b�_�ǂݍ���

	if(bcHead.bcBitCount == 16)							// 16 bit �͈���Ȃ�
		return false;

	//------------------------------------------------------------------------------
	// �T�[�t�F�X�̈�m��
	//------------------------------------------------------------------------------
	bRet = pBuff->allocBuffer((unsigned long)bcHead.bcWidth,
					   		  (unsigned long)bcHead.bcHeight,
							  (BITPERPIXEL)bcHead.bcBitCount);
	if(!bRet)
		return false;
	//------------------------------------------------------------------------------
	// �F���
	//------------------------------------------------------------------------------
	color = checkUsedColor(bcHead.bcBitCount, 0);
	if(bcHead.bcBitCount == 1							// �J���[�p���b�g���(1, 4, 8 bit)
	|| bcHead.bcBitCount == 4							// bit �̕��т� B, G, R, Reserved
	|| bcHead.bcBitCount == 8)							//
	{
		RGBTRIPLE rgb[256];								// �J���[�p���b�g

		fread(rgb, sizeof(RGBTRIPLE), color, fp);

		RGBACOLOR* pPalette = pBuff->getPalette();
		for(unsigned long i = 0; i < color; i++)
		{
			pPalette->r = rgb[i].rgbRed;
			pPalette->g = rgb[i].rgbGreen;
			pPalette->b = rgb[i].rgbBlue;
			pPalette->a = 0;
			pPalette++;
		}
	}
	//------------------------------------------------------------------------------
	// �摜�f�[�^
	//------------------------------------------------------------------------------
	const unsigned long line = checkBoundsAt4Bytes((unsigned long)bcHead.bcWidth, bcHead.bcBitCount);
	unsigned char* p = pBuff->getBuff();

	for(short h = bcHead.bcHeight-1; h >= 0; h--)		// �㉺���]���Ă��邽��
	{
		fseek(fp, (long)(offset + line * h), SEEK_SET);
		fread(p, sizeof(unsigned char), line, fp);		// 1 ���C���ǂݍ���
		p += line;
	}

	return true;
}

//----------------------------------------------------------------------------------
/*!
	@brief		Windows �`���œǂݍ���
	@param[in]	pSurf  : �T�[�t�F�X�ւ̃|�C���^
	@param[in]	fp     : �t�@�C���|�C���^
	@param[in]	offset : �t�@�C���擪����摜�f�[�^�܂ł̃I�t�Z�b�g (byte)
	@retval		true  : ����
	@retval		false : ���s
	@note
    @author		M.Morimoto
    @date		2006/08/02
*/
//----------------------------------------------------------------------------------
static bool loadFromFileByWindows(Buffer* pBuff, FILE* fp, unsigned long offset)
{
	BITMAPINFOHEADER biHead;							//
	unsigned long    color;								// �g�p�F��
	bool			 bRet;


	//------------------------------------------------------------------------------
	// ���w�b�_
	//------------------------------------------------------------------------------
	fread(&biHead, sizeof(BITMAPINFOHEADER), 1, fp);	// ���w�b�_�ǂݍ���

	if(biHead.biBitCount == 16							// 16 bit �͈���Ȃ�
	|| biHead.biCompression != 0)						// ���k�`���͈���Ȃ�
		return false;
	//------------------------------------------------------------------------------
	// �T�[�t�F�X�̈�m��
	//------------------------------------------------------------------------------
	bRet = pBuff->allocBuffer((unsigned long)biHead.biWidth,
					   		  (unsigned long)biHead.biHeight,
							  (BITPERPIXEL)biHead.biBitCount);
	if(!bRet)
		return false;
	//------------------------------------------------------------------------------
	// �F���
	//------------------------------------------------------------------------------
	color = checkUsedColor(biHead.biBitCount, biHead.biClrUsed);
	if(biHead.biBitCount == 1							// �J���[�p���b�g���(1, 4, 8 bit)
	|| biHead.biBitCount == 4							// bit �̕��т� B, G, R, Reserved
	|| biHead.biBitCount == 8)							//
	{
		RGBQUAD rgb[256];								// �J���[�p���b�g

		fread(rgb, sizeof(RGBQUAD), color, fp);

		RGBACOLOR* pPalette = pBuff->getPalette();
		for(unsigned long i = 0; i < color; i++)
		{
			pPalette->r = rgb[i].rgbRed;
			pPalette->g = rgb[i].rgbGreen;
			pPalette->b = rgb[i].rgbBlue;
			pPalette->a = rgb[i].rgbReserved;
			pPalette++;
		}
	}
	//------------------------------------------------------------------------------
	// �摜�f�[�^
	//------------------------------------------------------------------------------
	const unsigned long line = checkBoundsAt4Bytes(biHead.biWidth, biHead.biBitCount);
	unsigned char* p = pBuff->getBuff();

	for(long h = biHead.biHeight-1; h >= 0; h--)		// �㉺���]���Ă��邽��
	{
		fseek(fp, (long)(offset + line * h), SEEK_SET);
		fread(p, sizeof(unsigned char), line, fp);		// 1 ���C���ǂݍ���
		p += line;
	}

	return true;
}

//----------------------------------------------------------------------------------
/*!
	@brief		�t�@�C������ǂݍ���
	@param[in]	pSurf    : �T�[�t�F�X�ւ̃|�C���^
	@param[in]	filename : �t�@�C����
	@retval		true  : ����
	@retval		false : ���s
	@note
    @author		M.Morimoto
    @date		2006/08/02
*/
//----------------------------------------------------------------------------------
bool LoadFromFile(Buffer* pBuff, const char* filename)
{
	BITMAPFILEHEADER bfHead;
	FILE*			 fp;
	long			 size;
	FILEFORM		 form;
	bool			 bRet = false;


	ASSERT_MSG(pBuff != NULL, "bmp::LoadFromFile(() : pointer is NULL");
	ASSERT_MSG(filename != NULL, "bmp::LoadFromFile(() : pointer is NULL");


	pBuff->releaseBuffer();								// �O�̂��ߊJ��

	fp = fopen(filename, "rb");							// �t�@�C���ǂݍ���
	if(fp == NULL)										//
		goto error;
	//------------------------------------------------------------------------------
	// �t�@�C���w�b�_
	//------------------------------------------------------------------------------
	fread(&bfHead, sizeof(BITMAPFILEHEADER), 1, fp);	// �t�@�C���w�b�_�ǂݍ���
	if(bfHead.bfType != MAKE_TWOCC('B','M'))			// bmp �t�@�C���`�F�b�N
		goto error;
	//------------------------------------------------------------------------------
	// ���w�b�_
	//------------------------------------------------------------------------------
	fread(&size, sizeof(long), 1, fp);					// �t�@�C���`���`�F�b�N
	fseek(fp, -(long)sizeof(long), SEEK_CUR);			//
	form = checkFileForm(size);							//

	if(form == FILEFORM_OS2)							// OS/2
	{
		if(!loadFromFileByOS2(pBuff, fp, bfHead.bfOffBits))
			goto error;
	}
	else
	if(form == FILEFORM_WINDOWS)						// Windows
	{
		if(!loadFromFileByWindows(pBuff, fp, bfHead.bfOffBits))
			goto error;
	}
	else												// Other
	{
		goto error;
	}

	fclose(fp);
	return true;

error:
	fclose(fp);
	pBuff->releaseBuffer();								// �O�̂��ߊJ��
	return false;
}

//----------------------------------------------------------------------------------
/*!
	@brief		OS2 �`���ŏ�������
	@param[in]	fp    : �t�@�C���|�C���^
	@param[in]	pSurf : �T�[�t�F�X�ւ̃|�C���^
	@retval		true  : ����
	@retval		false : ���s
	@note
    @author		M.Morimoto
    @date		2006/08/02
*/
//----------------------------------------------------------------------------------
static bool SaveToFileByOS2(FILE* fp, const Buffer* pBuff)
{
	const BITPERPIXEL bpp = pBuff->getBpp();
	const unsigned long width  = pBuff->getWidth();
	const unsigned long height = pBuff->getHeight();
	const unsigned long line   = pBuff->getLine();
	unsigned long size_pal = 0;
	unsigned long size_img = 0;
	unsigned long color    = 0;
	unsigned long h;
	RGBTRIPLE rgb[256];


	//------------------------------------------------------------------------------
	// ���W�J
	//------------------------------------------------------------------------------
	size_img = line * height;
	if(size_img == 0)
		return false;

	if(bpp == BITPERPIXEL_1
	|| bpp == BITPERPIXEL_4
	|| bpp == BITPERPIXEL_8)
	{
		const RGBACOLOR* pPalette = pBuff->getPalette();

		color    = 1 << (unsigned long)bpp;
		size_pal = sizeof(RGBTRIPLE) * color;

		for(h = 0; h < color; h++)
		{
			rgb[h].rgbRed      = pPalette->r;
			rgb[h].rgbGreen    = pPalette->g;
			rgb[h].rgbBlue     = pPalette->b;
			pPalette++;
		}
	}
	//------------------------------------------------------------------------------
	// �t�@�C���w�b�_
	//------------------------------------------------------------------------------
	BITMAPFILEHEADER bfHead;

	bfHead.bfType      = MAKE_TWOCC('B','M');
	bfHead.bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPCOREHEADER) + size_pal + size_img;
	bfHead.bfReserved1 = 0;
	bfHead.bfReserved2 = 0;
	bfHead.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPCOREHEADER) + size_pal;

	fwrite((const void*)&bfHead, sizeof(BITMAPFILEHEADER), 1, fp);
	//------------------------------------------------------------------------------
	// ���w�b�_
	//------------------------------------------------------------------------------
	BITMAPCOREHEADER bcHead;

	bcHead.bcSize     = 12;
	bcHead.bcWidth    = (short)width;
	bcHead.bcHeight   = (short)height;
	bcHead.bcPlanes   = 1;
	bcHead.bcBitCount = (unsigned short)bpp;

	fwrite((const void*)&bcHead, sizeof(BITMAPCOREHEADER), 1, fp);
	//------------------------------------------------------------------------------
	// �F���
	//------------------------------------------------------------------------------
	if(size_pal > 0)
		fwrite((const void*)rgb, sizeof(RGBTRIPLE), color, fp);
	//------------------------------------------------------------------------------
	// �摜�f�[�^
	//------------------------------------------------------------------------------
	{
		const unsigned char* p = pBuff->getBuff() + size_img - line;

		for(h = 0; h < height; h++)
		{
			fwrite((const void*)p, sizeof(unsigned char), line, fp);
			p -= line;
		}
	}

	return true;
}

//----------------------------------------------------------------------------------
/*!
	@brief		Windows �`���ŏ�������
	@param[in]	fp    : �t�@�C���|�C���^
	@param[in]	pSurf : �T�[�t�F�X�ւ̃|�C���^
	@retval		true  : ����
	@retval		false : ���s
	@note
    @author		M.Morimoto
    @date		2006/08/02
*/
//----------------------------------------------------------------------------------
static bool SaveToFileByWindows(FILE* fp, const Buffer* pBuff)
{
	const BITPERPIXEL bpp = pBuff->getBpp();
	const unsigned long width  = pBuff->getWidth();
	const unsigned long height = pBuff->getHeight();
	const unsigned long line   = pBuff->getLine();
	unsigned long size_pal = 0;
	unsigned long size_img = 0;
	unsigned long color    = 0;
	unsigned long h;
	RGBQUAD   rgb[256];


	//------------------------------------------------------------------------------
	// ���W�J
	//------------------------------------------------------------------------------
	size_img = line * height;
	if(size_img == 0)
		return false;

	if(bpp == BITPERPIXEL_1
	|| bpp == BITPERPIXEL_4
	|| bpp == BITPERPIXEL_8)
	{
		const RGBACOLOR* pPalette = pBuff->getPalette();

		color    = 1 << (unsigned long)bpp;
		size_pal = sizeof(RGBQUAD) * color;

		for(h = 0; h < color; h++)
		{
			rgb[h].rgbRed      = pPalette->r;
			rgb[h].rgbGreen    = pPalette->g;
			rgb[h].rgbBlue     = pPalette->b;
			rgb[h].rgbReserved = pPalette->a;
			pPalette++;
		}
	}
	//------------------------------------------------------------------------------
	// �t�@�C���w�b�_
	//------------------------------------------------------------------------------
	BITMAPFILEHEADER bfHead;

	bfHead.bfType      = MAKE_TWOCC('B','M');
	bfHead.bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size_pal + size_img;
	bfHead.bfReserved1 = 0;
	bfHead.bfReserved2 = 0;
	bfHead.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size_pal;

	fwrite((const void*)&bfHead, sizeof(BITMAPFILEHEADER), 1, fp);
	//------------------------------------------------------------------------------
	// ���w�b�_
	//------------------------------------------------------------------------------
	BITMAPINFOHEADER biHead;

	biHead.biSize         = 40;
	biHead.biWidth        = (long)width;
	biHead.biHeight       = -(long)height;
	biHead.biPlanes       = 1;
	biHead.biBitCount     = (unsigned short)bpp;
	biHead.biCompression  = 0;
    biHead.biSizeImage    = size_img;
    biHead.biXPixPerMeter = 0;
	biHead.biYPixPerMeter = 0;
	biHead.biClrUsed      = color;
	biHead.biClrImporant  = 0;

	fwrite((const void*)&biHead, sizeof(BITMAPINFOHEADER), 1, fp);
	//------------------------------------------------------------------------------
	// �F���
	//------------------------------------------------------------------------------
	if(size_pal > 0)
		fwrite((const void*)rgb, sizeof(RGBQUAD), color, fp);
	//------------------------------------------------------------------------------
	// �摜�f�[�^
	//------------------------------------------------------------------------------
	{
		const unsigned char* p = pBuff->getBuff() + size_img - line;

		for(h = 0; h < height; h++)
		{
			fwrite((const void*)p, sizeof(unsigned char), line, fp);
			p -= line;
		}
	}

	return true;
}

//----------------------------------------------------------------------------------
/*!
	@brief		�t�@�C���ɏ�������
	@param[in]	filename : �t�@�C����
	@param[in]	pBuff    : �o�b�t�@�ւ̃|�C���^
	@param[in]	form	 : �t�@�C���`��
	@retval		true  : ����
	@retval		false : ���s
	@note
    @author		M.Morimoto
    @date		2006/08/02
*/
//----------------------------------------------------------------------------------
bool SaveToFile(const char* filename, const Buffer* pBuff, FILEFORM form)
{
	FILE* fp;


	ASSERT_MSG(filename != NULL, "bmp::SaveToFile() : pointer is NULL");
	ASSERT_MSG(pBuff != NULL, "bmp::SaveToFile() : pointer is NULL");

	fp = fopen(filename, "wb");							// �t�@�C����������
	if(fp == NULL)
		goto error;

	//------------------------------------------------------------------------------
	// ��������
	//------------------------------------------------------------------------------
	if(form == FILEFORM_OS2)
	{
		if(!SaveToFileByOS2(fp, pBuff))
			goto error;
	}
	else
	if(form == FILEFORM_WINDOWS)
	{
		if(!SaveToFileByWindows(fp, pBuff))
			goto error;
	}
	else
	{
		goto error;
	}

	fclose(fp);
	return true;

error:
	fclose(fp);
	return false;
}

//----------------------------------------------------------------------------------
/*!
	@brief		�f�X�g���N�^
    @author		M.Morimoto
    @date		2006/08/02
*/
//----------------------------------------------------------------------------------
Buffer::~Buffer()
{
	if(_pPalette != NULL)
		delete[] _pPalette;
	if(_pBuff != NULL)
		delete[] _pBuff;
}

//----------------------------------------------------------------------------------
/*!
	@brief		�o�b�t�@�m��
	@param[in]	width  : �o�b�t�@�̕�
	@param[in]	height : �o�b�t�@�̍���
	@param[in]	bpp    : bit per pixel
	@retval		true  : ����
	@retval		false : ���s
    @author		M.Morimoto
    @date		2006/08/02
*/
//----------------------------------------------------------------------------------
bool Buffer::allocBuffer(unsigned long width, unsigned long height, BITPERPIXEL bpp)
{
	if(_pPalette != NULL || _pBuff != NULL)
		releaseBuffer();

	if( bpp == BITPERPIXEL_1							//  2^1 �F
	 || bpp == BITPERPIXEL_4							//  2^4 �F
	 || bpp == BITPERPIXEL_8)							//  2^8 �F
	{
		_pPalette = new RGBACOLOR[1<<bpp];
		if(_pPalette== NULL)
			goto error;
	}

	unsigned long line;

	line = checkBoundsAt4Bytes(width, (unsigned short)bpp);
	_pBuff = new unsigned char[line * height];
	if(_pBuff == NULL)
		goto error;

	_width  = width;
	_height = height;
	_line   = line;
	_bpp    = bpp;
	return true;

error:
	releaseBuffer();
	return false;
}

//----------------------------------------------------------------------------------
/*!
	@brief		�o�b�t�@�J��
    @author		M.Morimoto
    @date		2006/08/02
*/
//----------------------------------------------------------------------------------
void Buffer::releaseBuffer(void)
{
	if(_pPalette != NULL)
	{
		delete[] _pPalette;
		_pPalette = NULL;
	}
	if(_pBuff != NULL)
	{
		delete[] _pBuff;
		_pBuff = NULL;
	}

	_width  =
	_height =
	_line   = 0;
	_bpp    = BITPERPIXEL_UNKNOWN;
}

//----------------------------------------------------------------------------------
/*!
	@brief		4 �o�C�g���E�`�F�b�N
	@param[in]	w    : ��
	@param[in]	bits : �s�N�Z���̃r�b�g��
	@retval		4 �o�C�g���E�ɑ����� 1 ���C���̃o�C�g��
    @author		M.Morimoto
    @date		2006/08/02
*/
//----------------------------------------------------------------------------------
unsigned long checkBoundsAt4Bytes(unsigned long w, unsigned short bits)
{
	unsigned long l;
	unsigned long x;

	x = (unsigned long)((8-(long)bits)/(long)bits);
	l = (w + x) * bits / 8;
	if((l % 4) != 0)
		l = ((l / 4) + 1) * 4;
	return l;
}

//----------------------------------------------------------------------------------
/*!
	@brief		�R�s�[
	@param[in]	pDst : �R�s�[��ւ̃|�C���^
	@param[in]	pSrc : �R�s�[���ւ̃|�C���^
	@retval		true  : ����
	@retval		false : ���s
    @author		M.Morimoto
    @date		2006/08/02
*/
//----------------------------------------------------------------------------------
bool Copy(Buffer* pDst, const Buffer* pSrc)
{
	ASSERT_MSG(pSrc != NULL, "bmp::Copy() : pSrc is NULL");
	ASSERT_MSG(pDst != NULL, "bmp::Copy() : pDst is NULL");
	ASSERT_MSG(pSrc != pDst, "bmp::Copy() : pSrc is pDst");

	pDst->releaseBuffer();

	if(!pDst->allocBuffer(pSrc->getWidth(), pSrc->getHeight(), pSrc->getBpp()))
		return false;

	if(pDst->getPalette() != NULL)
		memcpy((void*)pDst->getPalette(), (const void*)pSrc->getPalette(), sizeof(RGBACOLOR) * (1 << pSrc->getBpp()));

	memcpy((void*)pDst->getBuff(), (const void*)pSrc->getBuff(), sizeof(unsigned char) * (pSrc->getLine()*pSrc->getHeight()));

	return true;
}

//----------------------------------------------------------------------------------
/*!
	@brief		�h��Ԃ�
	@param[in]	pDst : �Ώۂւ̃|�C���^
	@param[in]	r    : �� [0,255]
	@param[in]	g    : �� [0,255]
	@param[in]	b    : �� [0,255]
	@param[in]	a    : �� [0,255]
    @author		M.Morimoto
    @date		2006/08/02
*/
//----------------------------------------------------------------------------------
void FillBuffer(Buffer* pDst, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	ASSERT_MSG(pDst != NULL, "bmp::FillBuffer() : pDst is NULL");

	if((pDst->getPalette() != NULL)
	&& (pDst->getBpp() == BITPERPIXEL_1
	 || pDst->getBpp() == BITPERPIXEL_4
	 || pDst->getBpp() == BITPERPIXEL_8))
	{
		const unsigned long color = 1 << pDst->getBpp();
		RGBACOLOR* p = pDst->getPalette();

		for(unsigned long i = 0; i < color; i++)
		{
			p->r = r;
			p->g = g;
			p->b = b;
			p->a = a;
			p++;
		}
	}
	else
	if((pDst->getBuff() != NULL)
	&& (pDst->getBpp() == BITPERPIXEL_24
	 || pDst->getBpp() == BITPERPIXEL_32))
	{
		const unsigned char rgba[] = {r, g, b, a};
		const unsigned long width  = pDst->getWidth();
		const unsigned long height = pDst->getHeight();
		const unsigned long bytes  = pDst->getBpp() / 8;
		const unsigned long line   = pDst->getLine();
		const unsigned long line_w = bytes * width;
		unsigned char* p =pDst->getBuff();
		unsigned long  l;

		for(unsigned long h = 0; h < height; h++)
		{
			for(l = 0; l < line_w; l += bytes)
			{
				for(unsigned long b = 0; b < bytes; b++)
					p[b] = rgba[b];
				p += bytes;
			}
			for(l = line_w; l < line; l++)
			{
				*p = 0;
				p++;
			}
		}
	}
}

}	/* namespace bmp */