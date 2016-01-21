
#include "lib/lib_common.h"
#include "common.h"
#include "splitlist.h"


SplitList::~SplitList()
{
	if(pool){ delete[] pool; }
}

void SplitList::Alloc(std::size_t capacity)
{
	Free();
	pool = new SplitListNode[capacity];
	ASSERT_MSG(pool != NULL, "SplitList::Alloc(): alloc failed");
	std::size_t i;
	for(i = 0; i < capacity-1; i++)
		pool[i].SetNext(&pool[i+1]);
	pool[i].SetNext(NULL);
	free = pool;
}

void SplitList::Free()
{
	SAFE_DELETE_ARRAY(pool);
	free = NULL;
	head = NULL;
}

void SplitList::Clear()
{
	if(head)
	{
		SplitListNode* node = head;
		while(node->GetNext())
			node = node->GetNext();
		node->SetNext(free);
		free = head;
		head = NULL;
	}
}

void SplitList::Insert(float split)
{
	ASSERT_MSG(free != NULL, "SplitList::Insert(): alloc failed");
	SplitListNode* entry = free;
	free = free->GetNext();
	entry->SetNext(NULL);
	entry->SetSplitPos(split);
	entry->SetLeftCount(0);
	entry->SetRightCount(0);

	if(!head)
	{
		head = entry;
		return;
	}

	if(split < head->GetSplitPos())
	{
		entry->SetNext(head);
		head = entry;
	}
	else
	if(split == head->GetSplitPos())
	{
		entry->SetNext(free);
		free = entry;
	}
	else
	{
		SplitListNode* node = head;
		while(node->GetNext() && (split >= node->GetNext()->GetSplitPos()))
		{
			if(split == node->GetNext()->GetSplitPos())
			{
				entry->SetNext(free);
				free = entry;
				return;
			}
			node = node->GetNext();
		}
		entry->SetNext(node->GetNext());
		node->SetNext(entry);
	}
}
