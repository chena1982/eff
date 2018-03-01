/******************************************************************************
	created:	2013-9-19   1:24
	file path:	d:\EFF\Include\EFFBase\EFFFastIdMap.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFFastIdMap_2013_9_19__
#define __EFFFastIdMap_2013_9_19__

EFFBASE_BEGIN

//���ٴ�ȡid��value��һ��map

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

		//����²����ֵ������index��firstǰ��
		if (first > index)
		{
            effUINT oldFirst = first;
			first = index;
			while (++index < oldFirst)
			{
				//ָ����ֵ��index
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
		// ���map[index]������
		if (mapValue[index] < count)
		{
			return;
		}

		// ����Ƴ���һ��ָ��
		if (first == index)
		{
            effUINT next = index + 1;

			if (next < count)
			{
				// ���map[next]��ָ��
				if (mapValue[next] > count)
				{
					first = next;
					mapValue[index] = 0;
				}
				// ���map[next]������
				else
				{
					first = (effINT)mapValue[next];
					mapValue[next] = 0;
					mapValue[index] = 0;
				}
			}
			// ���first�����һ��(next == count)
			else
			{
				first = -1;
				mapValue[index] = 0;
			}
		}
		else
		{
            effUINT pre = index - 1;
			// ��ǰ��������һ��ָ��
			while (pre >= first)
			{
				// ���map[pre]��ָ��
				if (mapValue[pre] > count)
				{
                    effUINT next = index + 1;
					// index�������һ��
					if ( next < count )
					{
						// ���map[next]��ָ��
						if (mapValue[next] > count)
						{
							mapValue[index] = next;
						}
						// ���map[next]��0��������
						else
						{
							mapValue[index] = mapValue[next];
							mapValue[next] = 0;
						}
					}
					// index�����һ��
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
				// ���map[pre]������
				else
				{
                    effUINT next = index + 1;
					// index�������һ��
					if (next < count)
					{
						// ���map[next]��ָ��
						if (mapValue[next] > count)
						{
							mapValue[index] = 0;
							mapValue[pre] = next;
						}
						// ���map[next]��0��������
						else
						{
							mapValue[index] = 0;
							mapValue[pre] = mapValue[next];
							mapValue[next] = 0;
						}
					}
					// index�����һ��
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
				// ��һ��ָ�������
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
	// map���������ֵ��һ����0��һ����ָ�룬��һ������һ��ָ�������
	// ���map���м��пն���ÿ���ն��ĵ�һ��Ԫ�ص�ֵ����һ��ָ�������
    effSIZE *	mapValue;
    effUINT		count;
    effUINT		first;
    effUINT		current;
    effUINT		nextId;

	VECTOR<effUINT>	recycledIds;
};


EFFBASE_END



#endif