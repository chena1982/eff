/******************************************************************************
created:	2018-02-10   00:31
file path:	d:\EFF\EFFEngine\Src\EFFBase\EFFArray.cpp
author:		ChenA

purpose:
******************************************************************************/

#include "EFFBasePCH.h"
#include "EFFArray.h"

EFFBASE_BEGIN

effUINT GetCapacity(effUINT NewCount)
{
    return (effINT)(exp2(std::min(2.0f, ceil(log2((float)NewCount)))));
}

effVOID * ArrayGrow(effVOID * Array, effUINT AddCount, effUINT ElementSize)
{
    effBYTE * NewArray = NULL;


    effUINT NewCount = 0;
    effUINT NewCapacity = 0;
    ArrayHeader * Header = NULL;

    if (Array == NULL)
    {
        NewCount = AddCount;
        NewCapacity = GetCapacity(AddCount);
    }
    else
    {
        Header = GetArrayHeader(Array);
        NewCount = Header->Count + AddCount;
        NewCapacity = GetCapacity(NewCount);
    }



    NewArray = EFFNEW effBYTE[sizeof(ArrayHeader) + ElementSize * NewCapacity];

    if (Array != NULL)
    {
        memcpy(NewArray + sizeof(ArrayHeader), Array, ElementSize * Header->Count);
    }

    ArrayHeader * NewHeader = (ArrayHeader *)NewArray;
    NewHeader->Count = NewCount;
    NewHeader->Capacity = NewCapacity;

    return (effVOID *)(NewArray + sizeof(ArrayHeader));
}


EFFBASE_END

