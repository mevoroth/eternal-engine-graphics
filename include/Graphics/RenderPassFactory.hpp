#ifndef _RENDER_PASS_FACTORY_HPP_
#define _RENDER_PASS_FACTORY_HPP_

#include <vector>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class RenderPass;
		class View;

		RenderPass* CreateRenderPass(_In_ Device& DeviceObj, _In_ const vector<View*>& RenderTargets, _In_ View* DepthStencil = nullptr);
	}
}

#endif
