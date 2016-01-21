//==============================================================================
/*!
	@file	splitlist.h
	@brief	���ƂȂ镪�f�ʂ�ێ����邽�߂̃��X�g
    @author M.Morimoto
    @date   2010/03/19
	@note	�E�������A���P�[�V������p�ɂɂ������Ȃ�
			�E���������ė��p������
			�Ƃ������R�̌��ō쐬���Ă��邪�P���Ȏ����̂��ߌ�������_�ɂ��Ă͒���
 */
//==============================================================================
#ifndef __SPLITLIST_H_
#define __SPLITLIST_H_

#include <cstddef>


class SplitListNode
{
public:
	SplitListNode() : next(NULL), l_count(0), r_count(0) {}

	void SetNext(SplitListNode* node){ next = node; }
	SplitListNode* GetNext(){ return next; }
	void SetSplitPos(float split){ this->split = split; }
	float GetSplitPos(){ return split; }
	void SetLeftCount(std::size_t count){ l_count = count; }
	std::size_t GetLeftCount(){ return l_count; }
	void SetRightCount(std::size_t count){ r_count = count; }
	std::size_t GetRightCount(){ return r_count; }

private:
	float			split;
	std::size_t		l_count;
	std::size_t		r_count;
	SplitListNode*	next;
};

class SplitList
{
public:
	SplitList() : pool(NULL), free(NULL), head(NULL) {}
	~SplitList();

	SplitListNode* GetHead(){ return head; }

	void Alloc(std::size_t capacity);
	void Free();
	void Insert(float split);
	void Clear();

private:
	SplitListNode*	pool;
	SplitListNode*	free;
	SplitListNode*	head;
};

#endif // !__SPLITLIST_H_