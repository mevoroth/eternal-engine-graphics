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
			D3D12RenderPass(_In_ const RenderPassCreateInformation& CreateInformation);
		};
	}
}

#endif
