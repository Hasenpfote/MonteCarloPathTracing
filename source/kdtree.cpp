
#include "kdtree.h"


KdMemory::~KdMemory()
{
	SAFE_DELETE_ARRAY(kd_array);
	SAFE_DELETE_ARRAY(obj_array);
}

/*!
	@brief		メモリ確保
	@param[i]	kd_capacity: kd ノードの許容量
	@param[i]	obj_capacity: obj リストの許容量
	@note		32 バイト境界に合わせている点に注意
 */
void KdMemory::Alloc(std::size_t kd_capacity, std::size_t obj_capacity)
{
	unsigned long addr;
	// kd
	kd_array = (char*)(new KdTreeNode[kd_capacity+5]);
	ASSERT_MSG(kd_array != NULL, "KdMemory::Alloc(): alloc failed");
	addr = (unsigned long)kd_array;
	kd_head = (KdTreeNode*)((addr + 32) & (0xffffffff - 31));
	// obj
	obj_array = (char*)(new ObjectList[obj_capacity+5]);
	ASSERT_MSG(obj_array != NULL, "KdMemory::Alloc(): alloc failed");
	addr = (unsigned long)obj_array;
	obj_head = (ObjectList*)((addr + 32) & (0xffffffff - 31));
	
	ObjectList* node = obj_head;
	for(std::size_t i = 0; i < obj_capacity-1; i++)
	{
		node->SetNext(node+1);
		node++;
	}
	node->SetNext(NULL);

	capacity = kd_capacity;
}

void KdMemory::Free()
{
	SAFE_DELETE_ARRAY(kd_array);
	SAFE_DELETE_ARRAY(obj_array);
	kd_head = NULL;
	obj_head = NULL;
	capacity = 0;
}

ObjectList* KdMemory::NewObjectList()
{
	if(!obj_head)
		return NULL;

	ObjectList* node;
	node = obj_head;
	obj_head = obj_head->GetNext();
	node->SetNext(NULL);
	node->SetPrimitive(NULL);
	return node;
}

void KdMemory::FreeObjectList(ObjectList* list)
{
	ObjectList* node = list;
	while(node->GetNext())
		node = node->GetNext();
	node->SetNext( obj_head );
	obj_head = list;
}

KdTreeNode* KdMemory::NewKdTreeNodePair()
{ 
	if(capacity < 2)
		return NULL;

	unsigned long* tmp = (unsigned long*)kd_head;
	tmp[1] = tmp[3] = 0x6;
	KdTreeNode* node = kd_head;
	kd_head += 2;		// 配列で連続して確保しているから可能
	capacity-= 2;
	return node;
}

////////////////////////////////////////////////////////////////////////////////

KdTree::KdTree() : root(NULL), work(NULL), max_depth(0)
{
}

KdTree::~KdTree()
{
	mem.Free();
	SAFE_DELETE(root);
}

void KdTree::Build(const PrimitiveList& list, const AABB& aabb, std::size_t depth)
{
	mem.Free();
	std::size_t kd_capacity = 0;
	for(std::size_t i = 0; i < (depth+1); i++)
	{
		kd_capacity += (std::size_t)powf(2.0f, (float)(depth+1));
	}
	std::size_t obj_capacity = list.size();
//	mem.Alloc(kd_capacity, obj_capacity*obj_capacity);	// 分断面を共有する可能性があるため余分に確保
	mem.Alloc(kd_capacity, obj_capacity*10);	// 分断面を共有する可能性があるため余分に確保
	max_depth = depth;

	SAFE_DELETE(root);
	root = new KdTreeNode();
	ASSERT_MSG(root != NULL, "KdTree::Build(): alloc failed");
	for(PrimitiveList::const_iterator it = list.begin(); it != list.end(); it++)
		Add(root, (*it));

	// 分断面の候補は三角形で最大 3、球で最大 2 となることから三角形に合わせる
	split_list.Alloc(obj_capacity * 3);
	work = new Work[obj_capacity];
	ASSERT_MSG(work != NULL, "KdTree::Build(): alloc failed");
	// 分割
	SubDivide(root, aabb, 0, obj_capacity);
	// 解放
	delete[] work;
	split_list.Free();

	this->aabb = aabb;
}

void KdTree::SubDivide(KdTreeNode* node, const AABB& aabb, std::size_t depth, std::size_t num_prims)
{
	split_list.Clear();

	// 分断軸を決める(デフォルトは z 軸)
	Vector3 size = aabb.GetSize();
	if((size.x >= size.y) && (size.x >= size.z))
		node->SetAxis(Axis_X);
	else if((size.y >= size.x) && (size.y >= size.z))
		node->SetAxis(Axis_Y);
	const Axis axis = node->GetAxis(); 

	// 分断候補リストの作成
	ObjectList* list = node->GetList();
	const float min = aabb.min.v[axis];
	const float max = aabb.max.v[axis];
	int idx = 0;
	while(list)
	{
		Work* w = &work[idx];
		w->ref_prim = list->GetPrimitive();
		w->right = true;
		w->ref_prim->CalcRange(w->min, w->max, axis);
		idx++;
	
		const std::size_t num_points = w->ref_prim->GetNumPoints();
		for(std::size_t i = 0; i < num_points; i++)
		{
			float split = w->ref_prim->GetPoint(axis, i);
			if((split >= min) && (split <= max))
				split_list.Insert(split);
		}
		list = list->GetNext();
	}

	// 分断面の左右のカウント
	SplitListNode* snode = split_list.GetHead();
	AABB l_aabb = aabb;
	AABB r_aabb = aabb;
	while(snode)
	{
		l_aabb.max.v[axis] = snode->GetSplitPos();
		r_aabb.min.v[axis] = snode->GetSplitPos();
		std::size_t l_count = 0;
		std::size_t r_count = 0;
		for(std::size_t i = 0; i < num_prims; i++)
		{
			Work* w = &work[i];
			if(w->right)
			{
				if((w->min <= l_aabb.max.v[axis]) && (w->max >= l_aabb.min.v[axis]))
				{
					if(w->ref_prim->Intersect(l_aabb))
						l_count++;
				}
				if((w->min <= r_aabb.max.v[axis]) && (w->max >= r_aabb.min.v[axis]))
				{
					if(w->ref_prim->Intersect(r_aabb))
						r_count++;
					else
						w->right = false;
				}
			}
			else
			{
				l_count++;
			}
		}
		snode->SetLeftCount(l_count);
		snode->SetRightCount(r_count);
		snode = snode->GetNext();
	}
	// 最適な分断面を決定する
	const float SAV = 1.0f / (size.x * size.z + size.x * size.y + size.z * size.y);
	const float Cleaf = (float)num_prims;
	float low_cost = FLT_MAX;
	float best_pos = 0.0f;
	snode = split_list.GetHead();
	while(snode)
	{
		l_aabb.max.v[axis] = snode->GetSplitPos();
		r_aabb.min.v[axis] = snode->GetSplitPos();
		Vector3 l_size = l_aabb.GetSize();
		Vector3 r_size = r_aabb.GetSize();
		float SAL = l_size.x * l_size.z + l_size.x * l_size.y + l_size.z * l_size.y;
		float SAR = r_size.x * r_size.z + r_size.x * r_size.y + r_size.z * r_size.y;
		// ToDo: 後ほど見直す
		float cost = SAL * SAV * snode->GetLeftCount() + SAR * SAV * snode->GetRightCount();
		if(cost < low_cost)
		{
			low_cost = cost;
			best_pos = snode->GetSplitPos();
		}
		snode = snode->GetNext();
	}
	if(low_cost > Cleaf)
		return;
	node->SetSplitPos(best_pos);
	mem.FreeObjectList(node->GetList());

	// 子ノードの構築
	KdTreeNode* l_node = mem.NewKdTreeNodePair();
	ASSERT_MSG(l_node != NULL, "KdTree::SubDivide(): alloc failed");
	l_aabb.max.v[axis] = node->GetSplitPos();
	r_aabb.min.v[axis] = node->GetSplitPos();
	std::size_t l_count = 0;
	std::size_t r_count = 0;
	for(std::size_t i = 0; i < num_prims; i++)
	{
		Work* w = &work[i];
		if((w->min <= l_aabb.max.v[axis]) && (w->max >= l_aabb.min.v[axis]))
		{
			if(w->ref_prim->Intersect(l_aabb))
			{
				Add(l_node, w->ref_prim);
				l_count++;
			}
		}
		if((w->min <= r_aabb.max.v[axis]) && (w->max >= r_aabb.min.v[axis]))
		{
			if(w->ref_prim->Intersect(r_aabb))
			{
				Add((l_node+1), w->ref_prim);
				r_count++;
			}
		}
	}
	node->SetLeft(l_node);
	node->SetLeaf(false);

	// 子ノードの分割
	if(depth < max_depth)
	{
		if(l_count > 2)	SubDivide(l_node,     l_aabb, depth+1, l_count);
		if(r_count > 2)	SubDivide((l_node+1), r_aabb, depth+1, r_count);
	}
}

void KdTree::Add(KdTreeNode* node, Primitive* prim)
{
	ObjectList* obj_node = mem.NewObjectList();
	if(obj_node == NULL)
	{
		int a ;
		a = 0;
	}
	ASSERT_MSG(obj_node != NULL, "KdTree::Add(): alloc failed");
	obj_node->SetPrimitive(prim);
	obj_node->SetNext(node->GetList());
	node->SetList(obj_node);
}

bool KdTree::Traverse(Primitive** prim, Primitive::Param& param, const Ray& ray) const
{
	float t_near;
	float t_far;
	if(!aabb.Intersect(t_near, t_far, ray.org, ray.dir))
		return false;

	Vector3 entry_pos, exit_pos;
	Vec3Scale(&entry_pos, &ray.dir, t_near);
	Vec3Add(&entry_pos, &ray.org, &entry_pos);
	Vec3Scale(&exit_pos, &ray.dir, t_far);
	Vec3Add(&exit_pos, &ray.org, &exit_pos);

	(*prim) = NULL;
	return Traverse(prim, param, ray, root, entry_pos, exit_pos);
}

bool KdTree::Traverse(Primitive** prim, Primitive::Param& param, const Ray& ray, const KdTreeNode* node, const Vector3& entry_pos, const Vector3& exit_pos) const
{
	if(node->IsLeaf())
	{
		// ToDo: 交差判定の実装
		Primitive::Param temp;
		float t = FLT_MAX;
		ObjectList* obj_node = const_cast<ObjectList*>(node->GetList());
		while(obj_node)
		{
			Primitive* p = obj_node->GetPrimitive();
			if(p->Intersect(temp, ray))
			{
				if(temp.t < t)
				{
					(*prim) = p;
					param = temp;
					t = temp.t;
				}
			}
			obj_node = obj_node->GetNext();
		}
		return (*prim)? true : false;
	}

	const float split = node->GetSplitPos();
	const Axis axis = node->GetAxis();

	if(entry_pos.v[axis] <= split)
	{
		if(exit_pos.v[axis] <= split)
		{
			return Traverse(prim, param, ray, node->GetLeft(), entry_pos, exit_pos);
		}
		Vector3 split_pos;
		CalcSplitPos(split_pos, entry_pos, exit_pos, split, axis);
		// exit_pos を修正(分断面でクリップ)
		if(Traverse(prim, param, ray, node->GetLeft(), entry_pos, split_pos))
			return true;
		// entry_pos を修正(分断面でクリップ)
		if(Traverse(prim, param, ray, node->GetRight(), split_pos, exit_pos))
			return true;
	}
	else
	{
		if(exit_pos.v[axis] > split)
		{
			return Traverse(prim, param, ray, node->GetRight(), entry_pos, exit_pos);
		}
		Vector3 split_pos;
		CalcSplitPos(split_pos, entry_pos, exit_pos, split, axis);
		// exit_pos を修正(分断面でクリップ)
		if(Traverse(prim, param, ray, node->GetRight(), entry_pos, exit_pos))
			return true;
		// entry_pos を修正(分断面でクリップ)
		if(Traverse(prim, param, ray, node->GetLeft(), entry_pos, exit_pos))
			return true;
	}
	return false;	// まずここにはこない
}

void KdTree::CalcSplitPos(Vector3& split_pos, const Vector3& entry_pos, const Vector3& exit_pos, float split, Axis axis) const
{
	Vector3 sub;
	Vec3Subtract(&sub, &exit_pos, &entry_pos);
	float t = (split - entry_pos.v[axis]) / sub.v[axis];
	Axis prev_axis = (Axis)((axis+2) % Axis_Max);
	Axis next_axis = (Axis)((axis+1) % Axis_Max);
	split_pos.v[prev_axis] = entry_pos.v[prev_axis] + sub.v[prev_axis] * t;
	split_pos.v[next_axis] = entry_pos.v[next_axis] + sub.v[next_axis] * t;
	split_pos.v[axis] = split;
}
