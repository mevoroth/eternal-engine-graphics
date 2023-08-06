#include "Graphics/AccelerationStructureFactory.hpp"

#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Device.hpp"
#include "d3d12/D3D12AccelerationStructure.hpp"

namespace Eternal
{
	namespace Graphics
	{
		AccelerationStructure* CreateBottomLevelAccelerationStructure(_In_ GraphicsContext& InContext, _In_ const BottomLevelAccelerationStructureCreateInformation& InBottomLevelAccelerationStructureCreateInformation)
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12BottomLevelAccelerationStructure(InContext, InBottomLevelAccelerationStructureCreateInformation);
#endif
#ifdef ETERNAL_ENABLE_VULKAN
			case DeviceType::VULKAN:
				
#endif
			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}

		AccelerationStructure* CreateTopLevelAccelerationStructure(_In_ GraphicsContext& InContext, _In_ const TopLevelAccelerationStructureCreateInformation& InTopLevelAccelerationStructureCreateInformation)
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12TopLevelAccelerationStructure(InTopLevelAccelerationStructureCreateInformation);
#endif
#ifdef ETERNAL_ENABLE_VULKAN
			case DeviceType::VULKAN:

#endif
			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}

		void DestroyAccelerationStructure(_Inout_ AccelerationStructure*& InOutAccelerationStructure)
		{
			delete InOutAccelerationStructure;
			InOutAccelerationStructure = nullptr;
		}
	}
}