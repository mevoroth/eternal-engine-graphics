#ifndef _RENDER_PASS_FACTORY_HPP_
#define _RENDER_PASS_FACTORY_HPP_

#include <vector>
#include "Graphics/BlendState.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class RenderPass;
		class View;
		class Viewport;

		RenderPass* CreateRenderPass(_In_ Device& DeviceObj, _In_ const Viewport& ViewportObj, _In_ const vector<View*>& RenderTargets, _In_ const vector<BlendState*>& BlendStates, _In_ View* DepthStencil = nullptr, _In_ const LogicBlend& LogicBlendObj = LogicBlend());
	}
}

#endif
