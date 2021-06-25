#include "Graphics_deprecated/PipelineFactory.hpp"

#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "d3d12/D3D12Pipeline.hpp"
#include "Vulkan/VulkanPipeline.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Pipeline* CreatePipeline(_In_ GraphicsContext& Context, _In_ const PipelineCreateInformation& CreateInformation)
		{
			switch (Context.GetDevice().GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12Pipeline(Context.GetDevice(), CreateInformation);
#endif

			case DeviceType::VULKAN:
				return new VulkanPipeline(Context.GetDevice(), CreateInformation);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
