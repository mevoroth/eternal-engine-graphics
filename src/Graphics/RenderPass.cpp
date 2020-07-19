#include "Graphics/RenderPass.hpp"

#include "GraphicsSettings.hpp"

using namespace Eternal::Graphics;

RenderPass::RenderPass(_In_ const RenderPassCreateInformation& CreateInformation)
	: _RenderTargets(CreateInformation.RenderTargets)
	, _BlendStates(CreateInformation.BlendStates)
	, _LogicBlend(CreateInformation.LogicBlend)
	, _Viewport(CreateInformation.Viewport)
	, _DepthStencilRenderTarget(CreateInformation.DepthStencilRenderTarget)
{
	ETERNAL_ASSERT(_RenderTargets.size() < MAX_RENDER_TARGETS);
	ETERNAL_ASSERT(_RenderTargets.size() == _BlendStates.size());
}
