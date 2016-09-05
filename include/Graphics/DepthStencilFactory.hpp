#ifndef _DEPTH_STENCIL_FACTORY_HPP_
#define _DEPTH_STENCIL_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class DepthStencil;
		class DepthTest;
		class StencilTest;

		DepthStencil* CreateDepthStencil(_In_ const DepthTest& DepthTestState, _In_ const StencilTest& StencilTestState);
	}
}

#endif
