#pragma once

#include "Graphics/RenderPass.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class View;

		class D3D12RenderPass final : public RenderPass
		{
		public:
			D3D12RenderPass(_In_ const RenderPassCreateInformation& CreateInformation);
		};
	}
}
