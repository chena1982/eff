/******************************************************************************
	created:	2013-9-19   1:24
	file path:	d:\EFF\Include\EFFBase\EFFFastIdMap.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFFastIdMap_2013_9_19__
#define __EFFFastIdMap_2013_9_19__

EFFBASE_BEGIN

//快速存取id，value的一种map

template<typename T>
class EFFFastIdMap
{
public:
	EFFFastIdMap(effINT initCount = 4)
	{
		mapValue = EFFNEW effSIZE[initCount];
		memset(mapValue, 0, sizeof(effSIZE) * initCount);
		count = initCount;

		first = -1;
		current = -1;
		nextId = -1;
	}

	effINT Add(T * value)
	{
		CalculateNextId();
        effUINT index = nextId;

		if (index >= count)
		{
			effINT newCount = count * 2;
            effSIZE * newMap = EFFNEW effSIZE[newCount];
			memset(newMap, 0, sizeof(effSIZE) * newCount);

			memcpy(newMap, mapValue, sizeof(effSIZE) * count);

			SFT_DELETE(mapValue);
			mapValue = newMap;
			count = newCount;
		}

		mapValue[index] = (effSIZE)(effVOID *)value;
		if (first == -1)
		{
			first = index;
		}

		//如果新插入的值的索引index在first前面
		if (first > index)
		{
            effUINT oldFirst = first;
			first = index;
			while (++index < oldFirst)
			{
				//指向有值的index
				if (mapValue[index] < count)
				{
					mapValue[index] = oldFirst;
					break;
				}
			}
		}

		return index;
	}

	effVOID Remove(effUINT index)	
	{
#ifdef _DEBUG
		if (index >= count || index < 0)
		{
			return;
		}

		if (first == -1)
		{
			return;
		}
#endif
		// 如果map[index]是索引
		if (mapValue[index] < count)
		{
			return;
		}

		// 如果移除第一个指针
		if (first == index)
		{
            effUINT next = index + 1;

			if (next < count)
			{
				// 如果map[next]是指针
				if (mapValue[next] > count)
				{
					first = next;
					mapValue[index] = 0;
				}
				// 如果map[next]是索引
				else
				{
					first = (effINT)mapValue[next];
					mapValue[next] = 0;
					mapValue[index] = 0;
				}
			}
			// 如果first是最后一个(next == count)
			else
			{
				first = -1;
				mapValue[index] = 0;
			}
		}
		else
		{
            effUINT pre = index - 1;
			// 向前遍历到第一个指针
			while (pre >= first)
			{
				// 如果map[pre]是指针
				if (mapValue[pre] > count)
				{
                    effUINT next = index + 1;
					// index不是最后一个
					if ( next < count )
					{
						// 如果map[next]是指针
						if (mapValue[next] > count)
						{
							mapValue[index] = next;
						}
						// 如果map[next]是0或者索引
						else
						{
							mapValue[index] = mapValue[next];
							mapValue[next] = 0;
						}
					}
					// index是最后一个
					else
					{
						mapValue[index] = 0;
					}
					
					break;
				}
				else if (mapValue[pre] == 0)
				{
					pre--;
					continue;
				}
				// 如果map[pre]是索引
				else
				{
                    effUINT next = index + 1;
					// index不是最后一个
					if (next < count)
					{
						// 如果map[next]是指针
						if (mapValue[next] > count)
						{
							mapValue[index] = 0;
							mapValue[pre] = next;
						}
						// 如果map[next]是0或者索引
						else
						{
							mapValue[index] = 0;
							mapValue[pre] = mapValue[next];
							mapValue[next] = 0;
						}
					}
					// index是最后一个
					else
					{
						mapValue[pre] = 0;
						mapValue[index] = 0;
					}

					break;
				}
			}
		}
		
		recycledIds.push_back(index);
	}

	T * operator [] (effUINT index)
	{
#ifdef _DEBUG
		if (index >= count || index < 0)
		{
			return NULL;
		}
#endif
		return (T *)(effVOID *)mapValue[index];
	}


	T * GetFirst()
	{
		if (first == -1)
		{
			return NULL;
		}

		current = first;
		return (T *)(effVOID *)mapValue[first];
	}

	T * GetNext()
	{
		current++;
		while (current < count)
		{
			if (mapValue[current] > count)
			{
				return (T *)(effVOID *)mapValue[current];
			}
			else if (mapValue[current] == 0)
			{
				return NULL;
			}
			else
			{
				// 下一个指针的索引
				current = (effINT)mapValue[current];
			}
		}

		return NULL;
	}

protected:
	effVOID	CalculateNextId()
	{
		if (recycledIds.size() == 0)
		{
			nextId++;
			return;
		}

		nextId = recycledIds[0];
		recycledIds.erase(recycledIds.begin());
	}
protected:
	// map存的有三种值，一种是0，一种是指针，另一种是下一个指针的索引
	// 如果map的中间有空洞，每个空洞的第一个元素的值是下一个指针的索引
    effSIZE *	mapValue;
    effUINT		count;
    effUINT		first;
    effUINT		current;
    effUINT		nextId;

	VECTOR<effUINT>	recycledIds;
};


EFFBASE_END



#endif