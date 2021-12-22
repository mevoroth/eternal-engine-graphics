#include "Graphics/PipelineFactory.hpp"

#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "d3d12/D3D12Pipeline.hpp"
#include "Vulkan/VulkanPipeline.hpp"

namespace Eternal
{
	namespace Graphics
	{
		template<typename PipelineCreateInformationType>
		Pipeline* CreatePipeline(_In_ GraphicsContext& InContext, _In_ const PipelineCreateInformationType& InPipelineCreateInformation)
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12Pipeline(InContext.GetDevice(), InPipelineCreateInformation);
#endif
#ifdef ETERNAL_ENABLE_VULKAN
			case DeviceType::VULKAN:
				return new VulkanPipeline(InContext.GetDevice(), InPipelineCreateInformation);
#endif
			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}

		Pipeline* CreatePipeline(_In_ GraphicsContext& InContext, _In_ const GraphicsPipelineCreateInformation& InPipelineCreateInformation)
		{
			return CreatePipeline<GraphicsPipelineCreateInformation>(InContext, InPipelineCreateInformation);
		}

		Pipeline* CreatePipeline(_In_ GraphicsContext& InContext, _In_ const ComputePipelineCreateInformation& InPipelineCreateInformation)
		{
			return CreatePipeline<ComputePipelineCreateInformation>(InContext, InPipelineCreateInformation);
		}

		void DestroyPipeline(_Inout_ Pipeline*& InOutPipeline)
		{
			delete InOutPipeline;
			InOutPipeline = nullptr;
		}
	}
}
