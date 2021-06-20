#ifndef _GRAPHICS_TEXTURE_FACTORY_HPP_
#define _GRAPHICS_TEXTURE_FACTORY_HPP_

#include "Graphics_deprecated/Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Texture;
		enum Format;
		
		Texture* CreateTexture(_In_ const Format& FormatObj, _In_ const Resource::Usage& UsageObj, _In_ const Resource::CPUAccess& CPUMode, _In_ uint32_t Width, _In_ uint32_t Height, _In_ const void* Data = nullptr);
	}
}

#endif
