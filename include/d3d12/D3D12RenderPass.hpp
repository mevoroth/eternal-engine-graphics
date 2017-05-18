#ifndef _D3D12_RENDER_PASS_HPP_
#define _D3D12_RENDER_PASS_HPP_

#include <vector>
#include "Graphics/RenderPass.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class View;

		class D3D12RenderPass : public RenderPass
		{
		public:
			D3D12RenderPass(_In_ const Viewport& ViewportObj, _In_ const vector<View*>& RenderTargets, _In_ const vector<BlendState*>& BlendStates, _In_ View* DepthStencil = nullptr, _In_ const LogicBlend& LogicBlendObj = LogicBlend());

			View*					GetDepthStencil() const { return _DepthStencil; }

		private:
			View*					_DepthStencil = nullptr;
		};
	}
}

#endif
