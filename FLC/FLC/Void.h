#pragma once
#include "RuntimeType.h"

namespace flc
{
    namespace types
    {
        namespace predefined
        {
            class VoidRuntimeType : public RuntimeType
            {
            public:
                VoidRuntimeType();
                ~VoidRuntimeType();
            };
        }
    }
}
