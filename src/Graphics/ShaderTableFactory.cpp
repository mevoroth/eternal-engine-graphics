#include "Graphics/ShaderTableFactory.hpp"

#include "Graphics/Device.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "d3d12/D3D12ShaderTable.hpp"

namespace Eternal
{
	namespace Graphics
	{
		ShaderTable* CreateShaderTable(_In_ Device& InDevice, _In_ Pipeline& InPipeline)
		{
			switch (InDevice.GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12ShaderTable(InDevice, InPipeline);
#endif
#ifdef ETERNAL_ENABLE_VULKAN
			case DeviceType::VULKAN:
#endif
			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
