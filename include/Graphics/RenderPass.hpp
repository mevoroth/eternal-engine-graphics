#ifndef _RENDER_PASS_HPP_
#define _RENDER_PASS_HPP_

#include <vector>
#include "Graphics/BlendState.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class View;
		class BlendState;
		class Viewport;

		class RenderPass
		{
		public:
			RenderPass(_In_ const vector<View*>& RenderTargets, _In_ const vector<BlendState*>& BlendStates, _In_ const Viewport& ViewportObj, _In_ const LogicBlend& LogicBlendObj);
			virtual ~RenderPass() {}

			const vector<View*>&		GetRenderTargets() const	{ return _RenderTargets; }
			const vector<BlendState*>&	GetBlendStates() const		{ return _BlendStates; }
			const Viewport&				GetViewport() const			{ return _Viewport; }

		private:
			vector<View*>		_RenderTargets;
			vector<BlendState*>	_BlendStates;
			LogicBlend			_LogicBlend;
			const Viewport&		_Viewport;
		};
	}
}

#endif
