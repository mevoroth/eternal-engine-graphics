#include "Graphics/RenderPass.hpp"

#include "GraphicsSettings.hpp"

namespace Eternal
{
	namespace Graphics
	{
		const RenderTargetOperator RenderTargetOperator::NoLoad_NoStore	(LoadOperator::NO_LOAD,	StoreOperator::NO_STORE);
		const RenderTargetOperator RenderTargetOperator::Load_NoStore	(LoadOperator::LOAD,	StoreOperator::NO_STORE);
		const RenderTargetOperator RenderTargetOperator::Load_Store		(LoadOperator::LOAD,	StoreOperator::STORE);
		const RenderTargetOperator RenderTargetOperator::Load_Resolve	(LoadOperator::LOAD,	StoreOperator::RESOLVE);
		const RenderTargetOperator RenderTargetOperator::Clear_NoStore	(LoadOperator::CLEAR,	StoreOperator::NO_STORE);
		const RenderTargetOperator RenderTargetOperator::Clear_Store	(LoadOperator::CLEAR,	StoreOperator::STORE);
		const RenderTargetOperator RenderTargetOperator::Clear_Resolve	(LoadOperator::CLEAR,	StoreOperator::RESOLVE);

		RenderPass::RenderPass(_In_ const RenderPassCreateInformation& InRenderPassCreateInformation)
			: _RenderPassCreateInformation(InRenderPassCreateInformation)
		{
			ETERNAL_ASSERT(_RenderPassCreateInformation.RenderTargets.size() < MAX_RENDER_TARGETS);
			ETERNAL_ASSERT((InRenderPassCreateInformation.RenderTargets.size() + (InRenderPassCreateInformation.DepthStencilRenderTarget ? 1 : 0)) > 0);
		}
	}
}
