#pragma once

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
