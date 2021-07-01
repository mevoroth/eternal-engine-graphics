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
		View* CreateConstantBufferView(_In_ const ConstantBufferViewCreateInformation& InConstantBufferViewCreateInformation)
		{
			switch (InConstantBufferViewCreateInformation.Context.GetDevice().GetDeviceType())
#ifdef ETERNAL_ENABLE_D3D12
			{
			case DeviceType::D3D12:
				return new D3D12View(InConstantBufferViewCreateInformation);
#endif

			case DeviceType::VULKAN:
				

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}

		View* CreateShaderResourceView(_In_ const ShaderResourceViewCreateInformation& InShaderResourceViewCreateInformation)
		{
			switch (InShaderResourceViewCreateInformation.Context.GetDevice().GetDeviceType())
#ifdef ETERNAL_ENABLE_D3D12
			{
			case DeviceType::D3D12:
				return new D3D12View(InShaderResourceViewCreateInformation);
#endif

			case DeviceType::VULKAN:


			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
