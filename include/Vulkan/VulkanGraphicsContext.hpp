#pragma once

#include "NextGenGraphics/Context.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class VulkanGraphicsContext : public GraphicsContext
		{
		public:

			VulkanGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation);
		};
	}
}
