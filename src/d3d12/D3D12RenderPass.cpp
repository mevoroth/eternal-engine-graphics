#include "d3d12/D3D12RenderPass.hpp"

using namespace Eternal::Graphics;

D3D12RenderPass::D3D12RenderPass(_In_ const Viewport& ViewportObj, _In_ const vector<View*>& RenderTargets, _In_ const vector<BlendState*>& BlendStates, _In_ View* DepthStencil /* = nullptr */, _In_ const LogicBlend& LogicBlendObj /* = LogicBlend() */)
	: RenderPass(RenderTargets, BlendStates, ViewportObj, LogicBlendObj)
	, _DepthStencil(DepthStencil)
{
}
