#ifndef _STRUCTURED_BUFFER_FACTORY_HPP_
#define _STRUCTURED_BUFFER_FACTORY_HPP_

#include "Graphics_deprecated/Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class StructuredBuffer;

		StructuredBuffer* CreateStructuredBuffer(_In_ const Resource::Usage& UsageObj, _In_ const Resource::CPUAccess& CPUMode, _In_ size_t Stride, _In_ uint32_t ElementsCount, _In_ const void* Data = nullptr);
	}
}

#endif
