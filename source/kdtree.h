
#ifndef __KDTREE_H_
#define __KDTREE_H_

#include "primitive.h"
#include "splitlist.h"


/*!
	@brief	オブジェクトリスト
	@class	ObjectList
	@note	kd 木で使用する
 */
class ObjectList
{
public:
	ObjectList() : ref_prim(NULL), next(NULL) {}

	void SetPrimitive(Primitive* prim){ ref_prim = prim; }
	Primitive* GetPrimitive(){ return ref_prim; }
	void SetNext(ObjectList* node){ next = node; }
	ObjectList* GetNext(){ return next; }
	
private:
	Primitive*	ref_prim;
	ObjectList*	next;
};

/*!
	@brief	kd 木ノード
	@class	KdTreeNode
	@note	data の bit 割り振りは以下
				0- 1: 軸
				2- 2: リーフかどうかを示す
				3-31: アドレス 
			Left と Right は連続していることに注意
			リーフとそうでない場合に振る舞いが異なる点に注意
 */
#define MASK_AXIS	0x3
#define MASK_LEAF	0x4
#define MASK_DATA	(MASK_LEAF | MASK_AXIS)
#define MASK_ADDR	0xfffffff8
 
class KdTreeNode
{
public:
	KdTreeNode() : data(0x6) {}

	void SetSplitPos(float split){ this->split = split; }
	float GetSplitPos() const { return split; }
	
	void SetAxis(Axis axis){ data = (data & ~MASK_AXIS) + axis; }
	Axis GetAxis() const { return (Axis)(data & MASK_AXIS); }

	void SetLeft(KdTreeNode* node){ data = (unsigned long)node + (data & MASK_DATA); }
	KdTreeNode* GetLeft(){ return (KdTreeNode*)(data & MASK_ADDR); }
	KdTreeNode* GetRight(){ return GetLeft() + 1; }
	const KdTreeNode* GetLeft() const { return (KdTreeNode*)(data & MASK_ADDR); }
	const KdTreeNode* GetRight() const { return GetLeft() + 1; }

	bool IsLeaf() const { return (data & MASK_LEAF)? true : false; }
	void SetLeaf(bool leaf){ data = (leaf)? (data | MASK_LEAF) : (data & ~MASK_LEAF); }

	void SetList(ObjectList* list){ data = (unsigned long)list + (data & MASK_DATA); }
	ObjectList* GetList(){ return (ObjectList*)(data & MASK_ADDR); }
	const ObjectList* GetList() const { return (ObjectList*)(data & MASK_ADDR); }
	
private:
	float			split;
	unsigned long 	data;
};

/*!
	@brief	kd 木用のメモリ管理クラス
	@class	KdMemory
 */
class KdMemory
{
public:
	KdMemory() : kd_array(NULL), obj_array(NULL), kd_head(NULL), obj_head(NULL), capacity(0) {}
	~KdMemory();

	void Alloc(std::size_t kd_capacity, std::size_t obj_capacity);
	void Free();

	ObjectList* NewObjectList();
	void FreeObjectList(ObjectList* list);
	KdTreeNode* NewKdTreeNodePair();

private:
	char*		kd_array;
	char*		obj_array;
	KdTreeNode*	kd_head;
	ObjectList*	obj_head;
	std::size_t capacity;
};

/*!
	@brief	kd 木
	@class	KdTree
 */
class KdTree
{
public:
	KdTree();
	~KdTree();

	void Build(const PrimitiveList& list, const AABB& aabb, std::size_t depth);
	bool Traverse(Primitive** prim, Primitive::Param& param, const Ray& ray) const;

private:
	void SubDivide(KdTreeNode* node, const AABB& aabb, std::size_t depth, std::size_t num_prims);
	void Add(KdTreeNode* node, Primitive* prim);
	bool Traverse(Primitive** prim, Primitive::Param& param, const Ray& ray, const KdTreeNode* node, const Vector3& entry_pos, const Vector3& exit_pos) const;
	void CalcSplitPos(Vector3& split_pos, const Vector3& enrty_pos, const Vector3& exit_pos, float split, Axis axis) const;

private:
	KdTreeNode* root;		//!< root node
	std::size_t max_depth;
	KdMemory	mem;
	SplitList	split_list;
	struct Work
	{
		float		min;
		float		max;
		Primitive*	ref_prim;
		bool		right;
	} *work;
	AABB		aabb;
};

#endif // !__KDTREE_H_