#include "Graphics/PipelineFactory.hpp"

#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "Null/NullPipeline.hpp"
#include "Proxy/ProxyPipeline.hpp"
#include "d3d12/D3D12Pipeline.hpp"
#include "Vulkan/VulkanPipeline.hpp"
#if (ETERNAL_USE_PRIVATE || ETERNAL_USE_PROXY)
#include "Graphics/PipelineFactoryPrivate.hpp"
#endif

namespace Eternal
{
	namespace Graphics
	{
		template<typename PipelineCreateInformationType>
		Pipeline* CreatePipeline(_In_ const DeviceType& InDeviceType, _Inout_ GraphicsContext& InOutContext, _In_ const PipelineCreateInformationType& InPipelineCreateInformation)
		{
			switch (InDeviceType)
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
#if (ETERNAL_USE_PRIVATE || ETERNAL_USE_PROXY)
				return CreatePipelinePrivate(InDeviceType, InOutContext, InPipelineCreateInformation);
#endif
				break;
			}
			ETERNAL_BREAK();
			return nullptr;
		}

		template<typename PipelineCreateInformationType>
		Pipeline* CreatePipeline(_Inout_ GraphicsContext& InOutContext, _In_ const PipelineCreateInformationType& InPipelineCreateInformation)
		{
			return CreatePipeline(InOutContext.GetDevice().GetDeviceType(), InOutContext, InPipelineCreateInformation);
		}

		Pipeline* CreatePipeline(_In_ GraphicsContext& InOutContext, _In_ const GraphicsPipelineCreateInformation& InPipelineCreateInformation)
		{
			return CreatePipeline<GraphicsPipelineCreateInformation>(InOutContext, InPipelineCreateInformation);
		}

		Pipeline* CreatePipeline(_In_ GraphicsContext& InOutContext, _In_ const ComputePipelineCreateInformation& InPipelineCreateInformation)
		{
			return CreatePipeline<ComputePipelineCreateInformation>(InOutContext, InPipelineCreateInformation);
		}

		Pipeline* CreatePipeline(_In_ GraphicsContext& InOutContext, _In_ const MeshPipelineCreateInformation& InPipelineCreateInformation)
		{
			return CreatePipeline<MeshPipelineCreateInformation>(InOutContext, InPipelineCreateInformation);
		}

		Pipeline* CreatePipeline(_In_ GraphicsContext& InOutContext, _In_ const RayTracingPipelineCreateInformation& InPipelineCreateInformation)
		{
			return CreatePipeline<RayTracingPipelineCreateInformation>(InOutContext, InPipelineCreateInformation);
		}

		Pipeline* CreatePipeline(_Inout_ GraphicsContext& InOutContext)
		{
			switch (InOutContext.GetDevice().GetDeviceType())
			{
			case DeviceType::DEVICE_TYPE_NULL:
				return new NullPipeline(InOutContext);

			case DeviceType::DEVICE_TYPE_PROXY:
				return new ProxyPipeline(InOutContext);

#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
				return new D3D12Pipeline(InOutContext);
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				return new VulkanPipeline(InOutContext);
#endif
			default:
#if (ETERNAL_USE_PRIVATE || ETERNAL_USE_PROXY)
				return CreatePipelinePrivate(InOutContext);
#endif
				break;
			}
			ETERNAL_BREAK();
			return nullptr;
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
				break;
			}
			ETERNAL_BREAK();
		}
	}
}
