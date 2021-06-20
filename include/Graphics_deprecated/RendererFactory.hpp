#ifndef _RENDERER_FACTORY_HPP_
#define _RENDERER_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Renderer;

		enum RendererType
		{
			RENDERER_D3D11
		};

		Renderer* CreateRenderer(_In_ const RendererType& RendererTypeObj);
	}
}

#endif
