//==============================================================================
/*!
	@file	splitlist.h
	@brief	候補となる分断面を保持するためのリスト
    @author M.Morimoto
    @date   2010/03/19
	@note	・メモリアロケーションを頻繁にしたくない
			・メモリを再利用したい
			という理由の元で作成しているが単純な実装のため穴がある点については注意
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