#include "Graphics/InputLayoutFactory.hpp"

#include "Macros/Macros.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12/D3D12InputLayout.hpp"
#include "Vulkan/VulkanInputLayout.hpp"

namespace Eternal
{
	namespace Graphics
	{
		InputLayout* CreateInputLayout(_In_ Device& DeviceObj,  _In_ const InputLayout::VertexDataType DataType[], _In_ uint32_t DataTypeCount)
		{
			switch (DeviceObj.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case D3D12:
				return new D3D12InputLayout(DataType, DataTypeCount);
#endif

			case VULKAN:
				return new VulkanInputLayout(DataType, DataTypeCount);

			default:
				ETERNAL_ASSERT(false);
				return nullptr;
			}
		}
	}
}
