#pragma once

#include "Graphics/RenderPass.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class NullRenderPass : public RenderPass
		{
		public:

			NullRenderPass(_In_ const RenderPassCreateInformation& InRenderPassCreateInformation)
				: RenderPass(InRenderPassCreateInformation)
			{
			}
		};
	}
}
