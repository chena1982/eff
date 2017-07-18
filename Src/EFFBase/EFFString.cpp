/******************************************************************************
created:	2017-07-17   22:01
file path:	d:\EFF\Src\EFFBase\EFFString.cpp
author:		ChenA

purpose:
******************************************************************************/

#include "EFFBasePCH.h"
#include "EFFString.h"



EFFBASE_BEGIN


EFFStaticStringManager::EFFStaticStringManager()
{
    count = 0;
}


EFFStaticStringManager::~EFFStaticStringManager()
{

}


EFFINLINE effVOID GetMaxSizeAndIndex(effUINT strSize, effUINT & maxSize)
{
    maxSize = 8;

    while (strSize > maxSize)
    {
        maxSize *= 2;
    }
}

effINT EFFStaticStringManager::AddString(const effWCHAR * str)
{
    effUINT strSize = (effUINT)wcslen(str) + 1;

    effUINT maxSize;
    GetMaxSizeAndIndex(strSize, maxSize);

    effUINT bufferSize = (effUINT)strBuffer.size();
    strBuffer.resize(bufferSize + maxSize);

    wcscpy_s(&strBuffer[bufferSize], maxSize, str);

    return indices.Add(&strBuffer[bufferSize]);
}

EFFBASE_END