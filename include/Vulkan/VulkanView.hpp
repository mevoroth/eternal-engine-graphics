#pragma once

#include "Graphics/View.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class VulkanView : public View
		{
		public:
			VulkanView(_In_ const RenderTargetViewCreateInformation& InViewCreateInformation);
			~VulkanView();
		};
	}
}
