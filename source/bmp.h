//----------------------------------------------------------------------------------
/*!
	@file
    @brief  BMP 操作ユーティリティ
    @author M.Morimoto
    @date   2006/08/25
*/
//----------------------------------------------------------------------------------
#ifndef __BMP_H_
#define __BMP_H_

//----------------------------------------------------------------------------------
/*!
	@namespace bmp
	@brief     ビットマップ
    @author    M.Morimoto
    @date      2006/08/25
*/
//----------------------------------------------------------------------------------
namespace bmp
{
/*!
	@enum	FILEFORM
	@brief	ファイル保存形式
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
	@brief	ビットマップファイルヘッダ
    @author M.Morimoto
    @date   2006/08/25
*/
typedef struct tagBITMAPFILEHEADER
{
	unsigned short bfType;								//!< ファイルタイプ 'BM' - OS/2, Windows Bitmap
	unsigned long  bfSize;								//!< ファイルサイズ (byte)
	unsigned short bfReserved1;							//!< 予約領域 常に 0
	unsigned short bfReserved2;							//!< 予約領域 常に 0
	unsigned long  bfOffBits;							//!< ファイル先頭から画像データまでのオフセット (byte)
} BITMAPFILEHEADER;

/*!
	@struct	BITMAPINFOHEADER
	@brief	Windows 用情報ヘッダ
    @author M.Morimoto
    @date   2006/08/25
*/
typedef struct tagBITMAPINFOHEADER
{
    unsigned long  biSize;								//!< 情報ヘッダのサイズ (byte) 40
    long           biWidth;								//!< 画像の幅 (ピクセル)
    long           biHeight;							/*!< @brief 画像の高さ (ピクセル)
															 @note  biHeight の値が正数なら，画像データは下から上へ
															 @note  biHeight の値が負数なら，画像データは上から下へ */
    unsigned short biPlanes;							//!< プレーン数 常に 1
    unsigned short biBitCount;							/*!< @brief 1 画素あたりのデータサイズ (bit)
															 @note  1 - 2       色ビットマップ
															 @note 	4 - 16      色ビットマップ
															 @note  8 - 256     色ビットマップ
															 @note (16 - 65536  色(high color)ビットマップ 正式に対応していない)
															 @note  24 - 1677 万色(true color)ビットマップ
															 @note  32 - 1677 万色(true color)ビットマップ */
    unsigned long  biCompression;						/*!< @brief 圧縮形式 0 - BI_RGB (無圧縮)
															 @note  1 - BI_RLE8 (RunLength 8 bits/pixel)
															 @note  2 - BI_RLE4 (RunLength 4 bits/pixel)
															 @note  3 - Bitfields */
    unsigned long  biSizeImage;							/*!< @brief 画像データ部のサイズ (byte)
															 @note	96dpi ならば37800 の場合もある */
    long           biXPixPerMeter;						/*!< @brief 横方向解像度 (1mあたりの画素数)
															 @note  96dpi ならば37800 の場合もある */
    long           biYPixPerMeter;						/*!< @brief 縦方向解像度 (1mあたりの画素数)
															 @note	96dpi ならば37800 の場合もある */
    unsigned long  biClrUsed;							/*!< @brief 格納されているパレット数 (使用色数)
															 @note	0 の場合もある */
    unsigned long  biClrImporant;						/*!< @brief 重要なパレットのインデックス
															 @note	0 の場合もある */
} BITMAPINFOHEADER;

/*!
	@struct	BITMAPCOREHEADER
	@brief	OS/2 用情報ヘッダ
    @author M.Morimoto
    @date   2006/08/25
*/
typedef struct tagBITMAPCOREHEADER
{
    unsigned long  bcSize;								//!< 情報ヘッダのサイズ (byte) 12
    short          bcWidth;								//!< 画像の幅 (ピクセル)
    short          bcHeight;							/*!< @brief 画像の高さ (ピクセル)
															 @note  bcHeight の値が正数なら，画像データは下から上へ
															 @note  bcHeight の値が負数なら，画像データは上から下へ */
    unsigned short bcPlanes;							//!< プレーン数 常に 1
    unsigned short bcBitCount;							/*!< @brief 1 画素あたりのデータサイズ (bit)
															 @note   1 - 2      色ビットマップ
															 @note   4 - 16     色ビットマップ
															 @note   8 - 256    色ビットマップ
															 @note (16 - 65536  色(high color)ビットマップ 正式に対応していない)
															 @note  24 - 1677 万色(true color)ビットマップ
															 @note  32 - 1677 万色(true color)ビットマップ */
} BITMAPCOREHEADER;

/*!
	@struct	RGBQUAD
	@brief	Windows 用カラーパレット
    @author M.Morimoto
    @date   2006/08/25
*/
typedef struct tagRGBQUAD
{
    unsigned char rgbBlue;								//!< 青 0..255
    unsigned char rgbGreen;								//!< 緑 0..255
    unsigned char rgbRed;								//!< 赤 0..255
    unsigned char rgbReserved;							//!< 予約領域 0
} RGBQUAD;

/*!
	@struct	RGBTRIPLE
	@brief	OS/2 用カラーパレット
    @author M.Morimoto
    @date   2006/08/25
*/
typedef struct tagRGBTRIPLE
{
    unsigned char rgbBlue;								//!< 青 0..255
    unsigned char rgbGreen;								//!< 緑 0..255
    unsigned char rgbRed;								//!< 赤 0..255
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
	@brief	ビット深度
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

	//! 幅の取得
	unsigned long getWidth(void) const { return _width; }
	//! 高さの取得
	unsigned long getHeight(void) const { return _height; }
	//! ラインの取得
	unsigned long getLine(void) const { return _line; }
	//! ビット深度の取得
	BITPERPIXEL   getBpp(void) const { return _bpp; }
	//! パレットの取得
	RGBACOLOR* getPalette(void){ return _pPalette; }
	//! パレットの取得(READ ONLY)
	const RGBACOLOR* getPalette(void) const { return (const RGBACOLOR*)_pPalette; }
	//! バッファの取得
	unsigned char* getBuff(void){ return _pBuff; }
	//! バッファの取得(READ ONLY)
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
