#pragma once

#include "Ref.h"

#define HZ_BIND_EVENT_FN(fn) std::bind(&##fn, this, std::placeholders::_1)

namespace Aura
{
    namespace Core
    {
        void Initialize();
        void Shutdown();
    }
}