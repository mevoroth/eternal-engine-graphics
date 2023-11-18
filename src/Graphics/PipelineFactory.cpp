#include "Graphics/PipelineFactory.hpp"

#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "Null/NullPipeline.hpp"
#include "Proxy/ProxyPipeline.hpp"
#include "d3d12/D3D12Pipeline.hpp"
#include "Vulkan/VulkanPipeline.hpp"
#if ETERNAL_USE_PRIVATE
#include "Graphics/PipelineFactoryPrivate.hpp"
#endif

namespace Eternal
{
	namespace Graphics
	{
		template<typename PipelineCreateInformationType>
		Pipeline* CreatePipeline(_Inout_ GraphicsContext& InOutContext, _In_ const PipelineCreateInformationType& InPipelineCreateInformation)
		{
			switch (InOutContext.GetDevice().GetDeviceType())
			{
			case DeviceType::DEVICE_TYPE_NULL:
				return new NullPipeline(InOutContext, InPipelineCreateInformation);

			case DeviceType::DEVICE_TYPE_PROXY:
				return new ProxyPipeline(InOutContext, InPipelineCreateInformation);

#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
				return new D3D12Pipeline(InOutContext, InPipelineCreateInformation);
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				return new VulkanPipeline(InOutContext, InPipelineCreateInformation);
#endif
			default:
				break;
			}
			ETERNAL_BREAK();
			return nullptr;
		}

		Pipeline* CreatePipeline(_In_ GraphicsContext& InOutContext, _In_ const GraphicsPipelineCreateInformation& InPipelineCreateInformation)
		{
#if ETERNAL_USE_PRIVATE
			return CreatePipelinePrivate(InOutContext, InPipelineCreateInformation);
#endif
			return CreatePipeline<GraphicsPipelineCreateInformation>(InOutContext, InPipelineCreateInformation);
		}

		Pipeline* CreatePipeline(_In_ GraphicsContext& InOutContext, _In_ const ComputePipelineCreateInformation& InPipelineCreateInformation)
		{
#if ETERNAL_USE_PRIVATE
			return CreatePipelinePrivate(InOutContext, InPipelineCreateInformation);
#endif
			return CreatePipeline<ComputePipelineCreateInformation>(InOutContext, InPipelineCreateInformation);
		}

		Pipeline* CreatePipeline(_In_ GraphicsContext& InOutContext, _In_ const MeshPipelineCreateInformation& InPipelineCreateInformation)
		{
#if ETERNAL_USE_PRIVATE
			return CreatePipelinePrivate(InOutContext, InPipelineCreateInformation);
#endif
			return CreatePipeline<MeshPipelineCreateInformation>(InOutContext, InPipelineCreateInformation);
		}

		Pipeline* CreatePipeline(_In_ GraphicsContext& InOutContext, _In_ const RayTracingPipelineCreateInformation& InPipelineCreateInformation)
		{
#if ETERNAL_USE_PRIVATE
			return CreatePipelinePrivate(InOutContext, InPipelineCreateInformation);
#endif
			return CreatePipeline<RayTracingPipelineCreateInformation>(InOutContext, InPipelineCreateInformation);
		}

		void DestroyPipeline(_Inout_ Pipeline*& InOutPipeline)
		{
			delete InOutPipeline;
			InOutPipeline = nullptr;
		}

		void SwapPipelines(_In_ GraphicsContext& InContext, _Inout_ Pipeline* A, _Inout_ Pipeline* B)
		{
			std::swap(A->GetPipelineCreateInformation(), B->GetPipelineCreateInformation());
			switch (InContext.GetDevice().GetDeviceType())
			{
#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
			{
				D3D12Pipeline* PipelineA = static_cast<D3D12Pipeline*>(A);
				D3D12Pipeline* PipelineB = static_cast<D3D12Pipeline*>(B);
				std::swap(PipelineA->GetD3D12PipelineState(), PipelineB->GetD3D12PipelineState());
				std::swap(PipelineA->GetD3D12PrimitiveTopology(), PipelineB->GetD3D12PrimitiveTopology());
			} break;
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
			{
				VulkanPipeline* PipelineA = static_cast<VulkanPipeline*>(A);
				VulkanPipeline* PipelineB = static_cast<VulkanPipeline*>(B);
				std::swap(PipelineA->GetVulkanPipeline(), PipelineB->GetVulkanPipeline());
			} break;
#endif
			default:
				ETERNAL_BREAK();
			}
		}
	}
}
