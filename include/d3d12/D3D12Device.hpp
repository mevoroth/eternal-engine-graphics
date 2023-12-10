#pragma once

#if ETERNAL_ENABLE_D3D12

#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"

#if ETERNAL_USE_NVIDIA_AFTERMATH
#include "NVIDIANsightAftermath.hpp"
#endif

struct IDXGIFactory4;
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
using LPCSTR = const char*;
#endif

namespace Eternal
{
	namespace Graphics
	{
		class OutputDevice;

		class D3D12Device final : public Device
		{
		public:
			static constexpr bool UseDRED = false;

			static void Initialize();
			static void Destroy();
			static IDXGIFactory4* GetDXGIFactory();

			D3D12Device(_In_ uint32_t DeviceIndex);
			~D3D12Device();

			virtual uint32_t GetDeviceMask() const override final;
			virtual DeviceType GetDeviceType() const override final { return DeviceType::DEVICE_TYPE_D3D12; }

			inline ID3D12Device* GetD3D12Device() { return _Device; }
			inline ID3D12Device5* GetD3D12Device5() { return _Device5; }
#if ETERNAL_USE_NVIDIA_AFTERMATH
			inline NVIDIA::NVIDIANsightAftermath& GetNVIDIANsightAftermath() { return _NVIDIANsightAftermath; }
#endif
#if ETERNAL_USE_DEBUG_LAYER
			//inline ID3D12DeviceRemovedExtendedData* GetD3D12DeviceRemovedExtendedData() { return _D3D12DeviceRemovedExtendedData; }
#endif

		private:
#if ETERNAL_USE_DEBUG_LAYER
			static ID3D12Debug3*								_Debug3;
			static IDXGIInfoQueue*								_DXGIInfoQueue;
			static IDXGIDebug*									_DXGIDebug;
			static ID3D12DeviceRemovedExtendedDataSettings*		_D3D12DeviceRemovedExtendedDataSettings;
			static ID3D12DeviceRemovedExtendedDataSettings1*	_D3D12DeviceRemovedExtendedDataSettings1;
			ID3D12InfoQueue*									_D3D12InfoQueue					= nullptr;
			ID3D12InfoQueue1*									_D3D12InfoQueue1				= nullptr;
			//ID3D12DeviceRemovedExtendedData*					_D3D12DeviceRemovedExtendedData	= nullptr;
#endif
			static bool											_IsInitialized;
			static IDXGIFactory4*								_DXGIFactory;

#if ETERNAL_USE_NVIDIA_AFTERMATH
			NVIDIA::NVIDIANsightAftermath						_NVIDIANsightAftermath;
#endif
			IDXGIAdapter1*										_DXGIAdapter					= nullptr;
			ID3D12Device*										_Device							= nullptr;
			ID3D12Device5*										_Device5						= nullptr;

			uint32_t											_DeviceMask						= 0xFFFFFFFF;

		};
	}
}

#endif
