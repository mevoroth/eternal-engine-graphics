#include "Graphics/ShaderTableFactory.hpp"

#include "Graphics/Device.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Null/NullShaderTable.hpp"
#include "d3d12/D3D12ShaderTable.hpp"

namespace Eternal
{
	namespace Graphics
	{
		ShaderTable* CreateShaderTable(_In_ Device& InDevice, _In_ Pipeline& InPipeline)
		{
			switch (InDevice.GetDeviceType())
			{
			case DeviceType::DEVICE_TYPE_NULL:
			case DeviceType::DEVICE_TYPE_PROXY:
				return new NullShaderTable(InPipeline);

#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
				return new D3D12ShaderTable(InDevice, InPipeline);
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
#endif
			default:
				break;
			}
			ETERNAL_BREAK();
			return nullptr;
		}
	}
}
