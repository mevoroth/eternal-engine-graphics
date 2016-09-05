#include "Graphics/RendererFactory.hpp"

#include "Macros/Macros.hpp"
#include "d3d11/D3D11Renderer.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Renderer* CreateRenderer(_In_ const RendererType& RendererTypeObj)
		{
			if (RendererTypeObj == RendererType::RENDERER_D3D11)
				return new D3D11Renderer();
			ETERNAL_ASSERT(false);
			return nullptr;
		}
	}
}
