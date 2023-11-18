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

		RenderPass* CreateRenderPass(_In_ GraphicsContext& InContext, _In_ const RenderPassCreateInformation& InCreateInformation);
		void DestroyRenderPass(_Inout_ RenderPass*& InOutRenderPass);
	}
}
