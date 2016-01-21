//----------------------------------------------------------------------------------
/*!
	@file
    @brief  BMP ���샆�[�e�B���e�B
    @author M.Morimoto
    @date   2006/08/25
*/
//----------------------------------------------------------------------------------
#ifndef __BMP_H_
#define __BMP_H_

//----------------------------------------------------------------------------------
/*!
	@namespace bmp
	@brief     �r�b�g�}�b�v
    @author    M.Morimoto
    @date      2006/08/25
*/
//----------------------------------------------------------------------------------
namespace bmp
{
/*!
	@enum	FILEFORM
	@brief	�t�@�C���ۑ��`��
    @author M.Morimoto
    @date   2006/08/25
*/
typedef enum
{
	FILEFORM_OS2,										//!< OS/2
	FILEFORM_WINDOWS,									//!< Windows
	FILEFORM_UNKNOWN									//!< Other
} FILEFORM;

#pragma pack(push, 1)

/*!
	@struct	BITMAPFILEHEADER
	@brief	�r�b�g�}�b�v�t�@�C���w�b�_
    @author M.Morimoto
    @date   2006/08/25
*/
typedef struct tagBITMAPFILEHEADER
{
	unsigned short bfType;								//!< �t�@�C���^�C�v 'BM' - OS/2, Windows Bitmap
	unsigned long  bfSize;								//!< �t�@�C���T�C�Y (byte)
	unsigned short bfReserved1;							//!< �\��̈� ��� 0
	unsigned short bfReserved2;							//!< �\��̈� ��� 0
	unsigned long  bfOffBits;							//!< �t�@�C���擪����摜�f�[�^�܂ł̃I�t�Z�b�g (byte)
} BITMAPFILEHEADER;

/*!
	@struct	BITMAPINFOHEADER
	@brief	Windows �p���w�b�_
    @author M.Morimoto
    @date   2006/08/25
*/
typedef struct tagBITMAPINFOHEADER
{
    unsigned long  biSize;								//!< ���w�b�_�̃T�C�Y (byte) 40
    long           biWidth;								//!< �摜�̕� (�s�N�Z��)
    long           biHeight;							/*!< @brief �摜�̍��� (�s�N�Z��)
															 @note  biHeight �̒l�������Ȃ�C�摜�f�[�^�͉�������
															 @note  biHeight �̒l�������Ȃ�C�摜�f�[�^�͏ォ�牺�� */
    unsigned short biPlanes;							//!< �v���[���� ��� 1
    unsigned short biBitCount;							/*!< @brief 1 ��f������̃f�[�^�T�C�Y (bit)
															 @note  1 - 2       �F�r�b�g�}�b�v
															 @note 	4 - 16      �F�r�b�g�}�b�v
															 @note  8 - 256     �F�r�b�g�}�b�v
															 @note (16 - 65536  �F(high color)�r�b�g�}�b�v �����ɑΉ����Ă��Ȃ�)
															 @note  24 - 1677 ���F(true color)�r�b�g�}�b�v
															 @note  32 - 1677 ���F(true color)�r�b�g�}�b�v */
    unsigned long  biCompression;						/*!< @brief ���k�`�� 0 - BI_RGB (�����k)
															 @note  1 - BI_RLE8 (RunLength 8 bits/pixel)
															 @note  2 - BI_RLE4 (RunLength 4 bits/pixel)
															 @note  3 - Bitfields */
    unsigned long  biSizeImage;							/*!< @brief �摜�f�[�^���̃T�C�Y (byte)
															 @note	96dpi �Ȃ��37800 �̏ꍇ������ */
    long           biXPixPerMeter;						/*!< @brief �������𑜓x (1m������̉�f��)
															 @note  96dpi �Ȃ��37800 �̏ꍇ������ */
    long           biYPixPerMeter;						/*!< @brief �c�����𑜓x (1m������̉�f��)
															 @note	96dpi �Ȃ��37800 �̏ꍇ������ */
    unsigned long  biClrUsed;							/*!< @brief �i�[����Ă���p���b�g�� (�g�p�F��)
															 @note	0 �̏ꍇ������ */
    unsigned long  biClrImporant;						/*!< @brief �d�v�ȃp���b�g�̃C���f�b�N�X
															 @note	0 �̏ꍇ������ */
} BITMAPINFOHEADER;

/*!
	@struct	BITMAPCOREHEADER
	@brief	OS/2 �p���w�b�_
    @author M.Morimoto
    @date   2006/08/25
*/
typedef struct tagBITMAPCOREHEADER
{
    unsigned long  bcSize;								//!< ���w�b�_�̃T�C�Y (byte) 12
    short          bcWidth;								//!< �摜�̕� (�s�N�Z��)
    short          bcHeight;							/*!< @brief �摜�̍��� (�s�N�Z��)
															 @note  bcHeight �̒l�������Ȃ�C�摜�f�[�^�͉�������
															 @note  bcHeight �̒l�������Ȃ�C�摜�f�[�^�͏ォ�牺�� */
    unsigned short bcPlanes;							//!< �v���[���� ��� 1
    unsigned short bcBitCount;							/*!< @brief 1 ��f������̃f�[�^�T�C�Y (bit)
															 @note   1 - 2      �F�r�b�g�}�b�v
															 @note   4 - 16     �F�r�b�g�}�b�v
															 @note   8 - 256    �F�r�b�g�}�b�v
															 @note (16 - 65536  �F(high color)�r�b�g�}�b�v �����ɑΉ����Ă��Ȃ�)
															 @note  24 - 1677 ���F(true color)�r�b�g�}�b�v
															 @note  32 - 1677 ���F(true color)�r�b�g�}�b�v */
} BITMAPCOREHEADER;

/*!
	@struct	RGBQUAD
	@brief	Windows �p�J���[�p���b�g
    @author M.Morimoto
    @date   2006/08/25
*/
typedef struct tagRGBQUAD
{
    unsigned char rgbBlue;								//!< �� 0..255
    unsigned char rgbGreen;								//!< �� 0..255
    unsigned char rgbRed;								//!< �� 0..255
    unsigned char rgbReserved;							//!< �\��̈� 0
} RGBQUAD;

/*!
	@struct	RGBTRIPLE
	@brief	OS/2 �p�J���[�p���b�g
    @author M.Morimoto
    @date   2006/08/25
*/
typedef struct tagRGBTRIPLE
{
    unsigned char rgbBlue;								//!< �� 0..255
    unsigned char rgbGreen;								//!< �� 0..255
    unsigned char rgbRed;								//!< �� 0..255
} RGBTRIPLE;

#pragma pack(pop)

/*!
	@struct	RGBACOLOR
	@brief
    @author M.Morimoto
    @date   2006/08/25
*/
typedef struct tagRGBACOLOR
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} RGBACOLOR;

/*!
	@enum	BITPERPIXEL
	@brief	�r�b�g�[�x
    @author M.Morimoto
    @date   2006/08/25
*/
typedef enum
{
	BITPERPIXEL_1  =  1,								//!<  1bit
	BITPERPIXEL_4  =  4,								//!<  4bit
	BITPERPIXEL_8  =  8,								//!<  8bit
	BITPERPIXEL_24 = 24,								//!< 24bit
	BITPERPIXEL_32 = 32,								//!< 32bit
	BITPERPIXEL_UNKNOWN
} BITPERPIXEL;

/*!
	@class	Buffer
	@brief
    @author M.Morimoto
    @date   2006/08/25
*/
class Buffer
{
private:
	unsigned long  _width;
	unsigned long  _height;
	unsigned long  _line;
	BITPERPIXEL    _bpp;
	RGBACOLOR*	   _pPalette;
	unsigned char* _pBuff;
public:
	Buffer() : _width(0), _height(0), _line(0), _bpp(BITPERPIXEL_UNKNOWN), _pPalette(NULL), _pBuff(NULL) {};
	~Buffer();

	bool allocBuffer(unsigned long width, unsigned long height, BITPERPIXEL bpp);
	void releaseBuffer(void);

	//! ���̎擾
	unsigned long getWidth(void) const { return _width; }
	//! �����̎擾
	unsigned long getHeight(void) const { return _height; }
	//! ���C���̎擾
	unsigned long getLine(void) const { return _line; }
	//! �r�b�g�[�x�̎擾
	BITPERPIXEL   getBpp(void) const { return _bpp; }
	//! �p���b�g�̎擾
	RGBACOLOR* getPalette(void){ return _pPalette; }
	//! �p���b�g�̎擾(READ ONLY)
	const RGBACOLOR* getPalette(void) const { return (const RGBACOLOR*)_pPalette; }
	//! �o�b�t�@�̎擾
	unsigned char* getBuff(void){ return _pBuff; }
	//! �o�b�t�@�̎擾(READ ONLY)
	const unsigned char* getBuff(void) const { return (const unsigned char*)_pBuff; }

	unsigned char* operator[](unsigned long y){ return _pBuff + y * _line; }
	const unsigned char* operator[](unsigned long y) const { return (const unsigned char*)(_pBuff + y * _line); }
};

//

unsigned long checkBoundsAt4Bytes(unsigned long w, unsigned short bits);
bool Copy(Buffer* pDst, const Buffer* pSrc);
void FillBuffer(Buffer* pDst, unsigned char r, unsigned char g, unsigned char b, unsigned char a);

bool LoadFromFile(Buffer* pBuff, const char* filename);
bool SaveToFile(const char* filename, const Buffer* pBuff, FILEFORM form);

}	/* namespace bmp */

#endif /* __BMP_H_ */
