#ifndef _CONSTANT_FACTORY_HPP_
#define _CONSTANT_FACTORY_HPP_

#include "Graphics/Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Constant;
		class Resource;

		Constant* CreateConstant(_In_ size_t BufferSize, _In_ const Resource::Usage& UsageObj, _In_ const Resource::CPUAccess& CPUMode);
		Constant* CreateConstant(_In_ size_t BufferSize, _In_ const Resource::Usage& UsageObj, _In_ const Resource::CPUAccess& CPUMode, _In_ void* Data);
	}
}

#endif
