#pragma once

#include "Aura/Core/Base.h"
#include "Aura/Core/UUID.h"

namespace Aura
{
    using AssetHandle = UUID;

    class Asset : RefCounted
    {
    public:
    private:
        AssetHandle m_handle;
    };
}