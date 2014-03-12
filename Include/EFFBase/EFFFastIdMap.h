/******************************************************************************
	created:	2013-9-19   1:24
	file path:	d:\EFF\Include\EFFBase\EFFFastIdMap.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFFastIdMap_2013_9_19__
#define __EFFFastIdMap_2013_9_19__

EFFBASE_BEGIN

//快速存取id，value的一种map，id固定是int

template<typename T>
class EFFFastIdMap
{
public:
	EFFFastIdMap(effINT initCount)
	{
		map = EFFNEW effINT[initCount];
		memset(map, 0, sizeof(effINT) * initCount);
		count = initCount;

		first = -1;
		current = -1;
	}

	effVOID Add(effINT index, T * value)
	{
		if ( index < 0 )
		{
			return;
		}

		if ( index >= count )
		{
			effINT newCount = count * 2;
			effINT * newMap = EFFNEW effINT[newCount];
			memset(newMap, 0, sizeof(effINT) * newCount);

			memcpy(newMap, map, sizeof(effINT) * count);

			SFT_DELETE(map);
			map = newMap;
			count = newCount;
		}

		map[index] = (effINT)(effVOID *)value;
		if ( first == -1 )
		{
			first = index;
		}

		if ( first > index )
		{
			effINT oldFirst = first;
			first = index;
			while ( ++index < oldFirst )
			{
				if ( map[index] < count )
				{
					map[index] = oldFirst;
					break;
				}
			}
		}
	}

	effVOID Remove(effINT index)	
	{
#ifdef _DEBUG
		if ( index >= count || index < 0 )
		{
			return;
		}

		if ( first == -1 )
		{
			return;
		}
#endif

		if ( map[index] < count )
		{
			return;
		}

		if ( first == index )
		{
			effINT next = index + 1;

			if ( next < count )
			{
				if ( map[next] > count )
				{
					first = next;
					map[index] = 0;
				}
				else
				{
					first = map[next];
					map[next] = 0;
					map[index] = 0;
				}
			}
			else
			{
				first = -1;
				map[index] = 0;
			}
		}
		else
		{
			int pre = index - 1;
			while ( pre >= first )
			{
				if ( map[pre] > count )
				{
					effINT next = index + 1;
					if ( next < count )
					{
						if ( map[next] > count )
						{
							map[index] = next;
						}
						else
						{
							map[index] = map[next];
							map[next] = 0;
						}
					}
					else
					{
						map[index] = 0;
					}
					
					break;
				}
				else if ( map[pre] == 0 )
				{
					pre--;
					continue;
				}
				else
				{
					effINT next = index + 1;
					if ( next < count )
					{
						if ( map[next] > count )
						{
							map[index] = 0;
							map[pre] = next;
						}
						else
						{
							map[index] = 0;
							map[pre] = map[next];
							map[next] = 0;
						}
					}
					else
					{
						map[pre] = 0;
						map[index] = 0;
					}

					break;
				}
			}
		}
		

	}

	T * operator [] (effINT index)
	{
#ifdef _DEBUG
		if ( index >= count || index < 0 )
		{
			return NULL;
		}
#endif
		return (T *)(effVOID *)map[index];
	}


	T * GetFirst()
	{
		if ( first == -1 )
		{
			return NULL;
		}

		current = first;
		return (T *)(effVOID *)map[first];
	}

	T * GetNext()
	{
		current++;
		while ( current < count )
		{
			if ( map[current] > count )
			{
				return (T *)(effVOID *)map[current];
			}
			else if ( map[current] == 0 )
			{
				return NULL;
			}
			else
			{
				current = map[current];
			}
		}

		return NULL;
	}
protected:
	effINT *	map;
	effINT		count;
	effINT		first;
	effINT		current;
};


EFFBASE_END



#endif