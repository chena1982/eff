/* ----------------------------------------------------------------------------
** Copyright (c) 2016 Austin Brunkhorst, All Rights Reserved.
**
** ReflectionModule.h
** --------------------------------------------------------------------------*/

#pragma once

#include "ReflectionDatabase.h"


#ifndef MODULE_EXPORTS
    #define MODULE_API		__declspec(dllimport)
#else
    #define MODULE_API		__declspec(dllexport)
#endif

#define DECLARE_REFLECTION_MODULE(module)                            \
    namespace meta_generated                                         \
    {                                                                \
        class MODULE_API Module##module : public ursine::meta::ReflectionModule \
        {                                                            \
        public:                                                      \
             Module##module(ursine::meta::ReflectionDatabase &db);   \
            ~Module##module(void);                                   \
        };                                                           \
    }                                                                \

#define UsingModule(module) meta_generated::Module##module _##module( db );

namespace ursine
{
    namespace meta
    {
        class MODULE_API ReflectionModule
        {
        public:
            ReflectionModule(ReflectionDatabase &db);

        protected:
            ReflectionDatabase &db;
        };
    }
}