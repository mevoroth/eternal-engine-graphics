#include "Graphics/ViewFactory.hpp"

#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/View.hpp"
#include "d3d12/D3D12View.hpp"
#include "Vulkan/VulkanView.hpp"

namespace Eternal
{
	namespace Graphics
	{
		template<typename ViewCreateInformationType>
		static View* CreateView(_In_ const ViewCreateInformationType& InViewCreateInformation)
		{
			switch (InViewCreateInformation.Context.GetDevice().GetDeviceType())
#ifdef ETERNAL_ENABLE_D3D12
			{
			case DeviceType::D3D12:
				return new D3D12View(InViewCreateInformation);
#endif

			case DeviceType::VULKAN:
				return new VulkanView(InViewCreateInformation);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}

		template<typename ViewCreateInformationType>
		static MultiBuffered<View> CreateMultiBufferedBufferView(_In_ const ViewCreateInformationType& InViewCreateInformation, _In_ MultiBuffered<Resource>& InResources)
		{
			uint32_t ResourceIndex = 0;
			return MultiBuffered<View>(InViewCreateInformation.Context, [&InViewCreateInformation, &ResourceIndex, &InResources]() {
				const_cast<ViewCreateInformationType&>(InViewCreateInformation).GraphicsResource = InResources[ResourceIndex++];
				return CreateView(InViewCreateInformation);
			});
		}

		View* CreateConstantBufferView(_In_ const ConstantBufferViewCreateInformation& InConstantBufferViewCreateInformation)
		{
			return CreateView(InConstantBufferViewCreateInformation);
		}

		View* CreateShaderResourceView(_In_ const ShaderResourceViewCreateInformation& InShaderResourceViewCreateInformation)
		{
			return CreateView(InShaderResourceViewCreateInformation);
		}

		MultiBuffered<View> CreateMultiBufferedConstantBufferView(_In_ MultiBuffered<Resource>& InResources, _In_ const ConstantBufferViewCreateInformation& InConstantBufferViewCreateInformation)
		{
			return CreateMultiBufferedBufferView(InConstantBufferViewCreateInformation, InResources);
		}
	}
}
