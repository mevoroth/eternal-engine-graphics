#include "Graphics/RenderPass.hpp"

#include "GraphicsSettings.hpp"
#include "Macros/Macros.hpp"

using namespace Eternal::Graphics;

RenderPass::RenderPass(_In_ const vector<View*>& RenderTargets, _In_ const vector<BlendState*>& BlendStates, _In_ const Viewport& ViewportObj, _In_ const LogicBlend& LogicBlendObj)
	: _RenderTargets(RenderTargets)
	, _BlendStates(BlendStates)
	, _LogicBlend(LogicBlendObj)
	, _Viewport(ViewportObj)
{
	ETERNAL_ASSERT(_RenderTargets.size() < MAX_RENDER_TARGETS);
	ETERNAL_ASSERT(_RenderTargets.size() == _BlendStates.size());
}
