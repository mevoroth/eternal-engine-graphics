#pragma once

#if ETERNAL_ENABLE_D3D12

#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"

#if ETERNAL_USE_NVIDIA_AFTERMATH
#include "NVIDIANsightAftermath.hpp"
#endif

struct IDXGIFactory4;
struct IDXGIAdapter;
struct IDXGIAdapter1;
struct ID3D12Device;
struct ID3D12Device5;

#if ETERNAL_USE_DEBUG_LAYER
struct ID3D12Debug3;
struct IDXGIInfoQueue;
struct ID3D12InfoQueue;
struct ID3D12InfoQueue1;
struct IDXGIDebug;
struct ID3D12DeviceRemovedExtendedDataSettings;
struct ID3D12DeviceRemovedExtendedDataSettings1;
struct ID3D12DeviceRemovedExtendedData;
enum D3D12_MESSAGE_CATEGORY;
enum D3D12_MESSAGE_SEVERITY;
enum D3D12_MESSAGE_ID;
#endif

namespace Eternal
{
	namespace Graphics
	{
		class OutputDevice;

		class D3D12Device : public Device
		{
		public:
			static constexpr bool UseDRED = true;

			~D3D12Device();

			virtual uint32_t GetDeviceMask() const override final;
			virtual DeviceType GetDeviceType() const override final { return DeviceType::DEVICE_TYPE_D3D12; }

			inline ID3D12Device* GetD3D12Device() { return _Device; }
			inline ID3D12Device5* GetD3D12Device5() { return _Device5; }

		protected:

			D3D12Device(_In_ uint32_t InDeviceIndex);

			ID3D12Device*	_Device			= nullptr;
			ID3D12Device5*	_Device5		= nullptr;
			IDXGIAdapter*	_DXGIAdapter	= nullptr;
			IDXGIAdapter1*	_DXGIAdapter1	= nullptr;

			uint32_t		_DeviceMask		= 0xFFFFFFFF;

		};
	}
}

#endif
