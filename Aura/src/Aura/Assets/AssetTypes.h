#pragma once

#include "Aura/Core/Base.h"
#include "Aura/Core/Logger.h"

namespace Aura
{
    enum class AssetFlag : uint16_t
    {
        None = 0,
        Missing = 1 << 1,
        Invalid = 1 << 2
    };

    enum class AssetType : uint16_t
    {
        None,
        Scene
    };

    
}