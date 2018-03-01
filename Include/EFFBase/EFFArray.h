/******************************************************************************
created:	2018-02-10   00:31
file path:	d:\EFF\EFFEngine\Include\EFFBase\EFFArray.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFArray_H__
#define __EFFArray_H__


EFFBASE_BEGIN

struct ArrayHeader
{
    effUINT Count;
    effUINT Capacity;
};

#define GetArrayHeader(Array) ((ArrayHeader *)((effCHAR *)(Array) - sizeof(ArrayHeader)))
#define GetArrayCount(Array) ((Array) ? GetArrayHeader(Array)->Count : 0)

#define IsArrayNeedGrow(Array, AddCount) ((Array) == NULL || (Size + AddCount) > Capacity)



#define ArrayPush(Array, Element) \
    IsArrayNeedGrow(Array, 1) ? Array = ArrayGrow(Array, 1, sizeof(*Array)) : 0, \
    Array[GetArrayHeader(Array)->Count++] = Element


effVOID * ArrayGrow(effVOID * Array, effUINT AddCount, effUINT ElementSize);

EFFBASE_END

#endif