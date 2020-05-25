#ifndef _RENDER_PASS_FACTORY_HPP_
#define _RENDER_PASS_FACTORY_HPP_

#include <vector>
#include "Graphics/BlendState.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class GraphicsContext;
		class RenderPass;
		struct RenderPassCreateInformation;

		RenderPass* CreateRenderPass(_In_ GraphicsContext& Context, _In_ const RenderPassCreateInformation& CreateInformation);
	}
}

#endif
