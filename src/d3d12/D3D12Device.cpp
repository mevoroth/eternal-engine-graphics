#if ETERNAL_ENABLE_D3D12

#include "d3d12/D3D12Device.hpp"

namespace Eternal
{
	namespace Graphics
	{
		D3D12Device::D3D12Device(_In_ uint32_t InDeviceIndex)
			: _DeviceMask(1 << InDeviceIndex)
		{
		}

		D3D12Device::~D3D12Device()
		{
		}

		uint32_t D3D12Device::GetDeviceMask() const
		{
			ETERNAL_ASSERT(_DeviceMask != 0xFFFFFFFF);
			return _DeviceMask;
		}
	}
}

#endif
