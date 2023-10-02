#include "pch.h"
#include "Base.h"
#include "Logger.h"

#define AURA_BUILD_ID "v0.1"

namespace Aura
{
    namespace Core
    {
		void Initialize()
		{
			Log::Init();

			AURA_CORE_TRACE_TAG("Core", "Hazel Engine {}", AURA_BUILD_ID);
			AURA_CORE_TRACE_TAG("Core", "Initializing...");
		}

		void Shutdown()
		{
			AURA_CORE_TRACE_TAG("Core", "Shutting down...");

			Log::Shutdown();
		}
    }
}