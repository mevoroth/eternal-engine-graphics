#include "Graphics/InputLayoutFactory.hpp"

#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "d3d12/D3D12InputLayout.hpp"
#include "Vulkan/VulkanInputLayout.hpp"

namespace Eternal
{
	namespace Graphics
	{
		static InputLayout* CreateInputLayout(_In_ Device& InDevice,  _In_ const InputLayout::VertexDataType DataType[], _In_ uint32_t DataTypeCount)
		{
			switch (InDevice.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12InputLayout(DataType, DataTypeCount);
#endif

			case DeviceType::VULKAN:
				return new VulkanInputLayout(DataType, DataTypeCount);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}

		InputLayout* CreateInputLayout(_In_ GraphicsContext& Context, _In_ const InputLayout::VertexDataType DataType[], _In_ uint32_t DataTypeCount)
		{
			return CreateInputLayout(Context.GetDevice(), DataType, DataTypeCount);
		}
	}
}