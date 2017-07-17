/******************************************************************************
created:	2017-07-17   22:01
file path:	d:\EFF\Include\EFFBase\EFFString.h
author:		ChenA

purpose:
******************************************************************************/
#ifndef __EFFString_2017_07_17__
#define __EFFString_2017_07_17__


EFFBASE_BEGIN


const effUINT STRING_INDEX_BITS = 24;
const effUINT STRING_INDEX_MASK = (1 << STRING_INDEX_BITS) - 1;

const effUINT STRING_SZIE_BITS = 4;
const effUINT STRING_SIZE_MASK = (1 << STRING_SZIE_BITS) - 1;

class EFFStaticStringManager
{
public:
    EFFStaticStringManager();
    ~EFFStaticStringManager();


    effINT AddString(const effCHAR * str);
private:

    VECTOR<effCHAR> strBuffer;

    EFFFastIdMap<effCHAR> indices;

    effUINT count;
};

EFFBASE_END

#endif
