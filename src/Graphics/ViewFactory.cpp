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
		static MultiBuffered<View>* CreateMultiBufferedBufferView(_In_ const ViewCreateInformationType& InViewCreateInformation, _In_ MultiBuffered<Resource>& InResources)
		{
			uint32_t ResourceIndex = 0;
			return new MultiBuffered<View>(InViewCreateInformation.Context, [&InViewCreateInformation, &ResourceIndex, &InResources]() {
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

		View* CreateRenderTargetView(_In_ const RenderTargetViewCreateInformation& InRenderTargetViewCreateInformation)
		{
			return CreateView(InRenderTargetViewCreateInformation);
		}

		View* CreateUnorderedAccessView(_In_ const UnorderedAccessViewCreateInformation& InUnorderedAccessViewCreateInformation)
		{
			return CreateView(InUnorderedAccessViewCreateInformation);
		}

		View* CreateDepthStencilView(_In_ const DepthStencilViewCreateInformation& InDepthStencilViewCreateInformation)
		{
			return CreateView(InDepthStencilViewCreateInformation);
		}

		MultiBuffered<View>* CreateMultiBufferedConstantBufferView(_In_ MultiBuffered<Resource>& InResources, _In_ const ConstantBufferViewCreateInformation& InConstantBufferViewCreateInformation)
		{
			return CreateMultiBufferedBufferView(InConstantBufferViewCreateInformation, InResources);
		}

		void DestroyView(_Inout_ View*& InOutView)
		{
			delete InOutView;
			InOutView = nullptr;
		}

		void DestroyMultiBufferedView(_Inout_ MultiBuffered<View>*& InOutMultiBufferedView)
		{
			delete InOutMultiBufferedView;
			InOutMultiBufferedView = nullptr;
		}
	}
}
