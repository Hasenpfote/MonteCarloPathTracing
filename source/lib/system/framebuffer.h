//==================================================================================
/*!
	@file	framebuffer.h
    @brief  フレームバッファ
    @author M.Morimoto
    @date   2010/03/02
 */
//==================================================================================
#ifndef __FRAME_BUFFER_H_
#define __FRAME_BUFFER_H_

#include <cstddef>

/*!
	@class	FrameBuffer
 */
template <class T, std::size_t SIZE = 3>
class FrameBuffer
{
public:
	struct Data
	{
		T ch[SIZE];
	};

public:
	FrameBuffer();
	virtual ~FrameBuffer();

	void resize(std::size_t w, std::size_t h);
	void erase();

	Data* ptr(std::size_t y){ return &data[y * w]; }
	const Data* ptr(std::size_t y) const { return (const Data*)&data[y * w]; }
	std::size_t width() const { return w; }
	std::size_t height() const { return h; }
	float aspect_ratio() const { return aspect; }

protected:
	std::size_t	w, h;
	float aspect;
	Data* data;
};

/*!
	@brief		コンストラクタ
 */
template <class T, std::size_t size>
FrameBuffer<T, size>::FrameBuffer() : w(0), h(0), data(NULL)
{
}

/*!
	@brief		デストラクタ
 */
template <class T, std::size_t size>
FrameBuffer<T, size>::~FrameBuffer()
{
	if(data)
		delete[] data;
}

/*!
	@brief		リサイズ
 */
template <class T, std::size_t size>
void FrameBuffer<T, size>::resize(std::size_t w, std::size_t h)
{
	erase();
	data = new Data[w * h];
	this->w = w;
	this->h = h;
	this->aspect = (float)w / (float)h;
}

/*!
	@brief		消去
 */
template <class T, std::size_t size>
void FrameBuffer<T, size>::erase()
{
	if(data)
	{
		delete[] data;
		data = NULL;
		w = h = 0;
	}
}

#endif // !__FRAME_BUFFER_H_
