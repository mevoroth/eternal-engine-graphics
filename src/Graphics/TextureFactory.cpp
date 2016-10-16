#include "Graphics/TextureFactory.hpp"

#include "Graphics/Format.hpp"
#include "d3d11/D3D11Texture.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Texture* CreateTexture(_In_ const Format& FormatObj, _In_ const Resource::Usage& UsageObj, _In_ const Resource::CPUAccess& CPUMode, _In_ uint32_t Width, _In_ uint32_t Height, _In_ const void* Data /*= nullptr*/)
		{
			return new D3D11Texture(FormatObj, UsageObj, CPUMode, Width, Height, Data);
		}
	}
}
