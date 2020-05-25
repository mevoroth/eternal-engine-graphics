#ifndef _RENDER_PASS_HPP_
#define _RENDER_PASS_HPP_

#include <vector>
#include "Graphics/BlendState.hpp"
#include "Graphics/Viewport.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class View;
		class Viewport;

		struct RenderPassCreateInformation
		{
			RenderPassCreateInformation(_In_ const Viewport& InViewport, _In_ const vector<View*>& InRenderTargets, _In_ const vector<BlendState>& InBlendStates, _In_ View* InDepthStencilRenderTarget = nullptr, _In_ const LogicBlend& InLogicBlend = LogicBlendNone)
				: Viewport(InViewport)
				, RenderTargets(InRenderTargets)
				, BlendStates(InBlendStates)
				, LogicBlend(InLogicBlend)
				, DepthStencilRenderTarget(InDepthStencilRenderTarget)
			{
			}

			vector<View*> RenderTargets;
			vector<BlendState> BlendStates;
			LogicBlend LogicBlend;
			const Viewport& Viewport;
			View* DepthStencilRenderTarget;
		};

		class RenderPass
		{
		public:
			RenderPass(_In_ const RenderPassCreateInformation& CreateInformation);
			virtual ~RenderPass() {}

			const vector<View*>&		GetRenderTargets() const			{ return _RenderTargets; }
			const vector<BlendState>&	GetBlendStates() const				{ return _BlendStates; }
			const LogicBlend&			GetLogicBlend() const				{ return _LogicBlend; }
			const Viewport&				GetViewport() const					{ return _Viewport; }
			const View*					GetDepthStencilRenderTarget() const	{ return _DepthStencilRenderTarget; }

		private:
			vector<View*>		_RenderTargets;
			vector<BlendState>	_BlendStates;
			LogicBlend			_LogicBlend;
			const Viewport&		_Viewport;
			View*				_DepthStencilRenderTarget = nullptr;
		};
	}
}

#endif
