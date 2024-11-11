#pragma once

#if ETERNAL_ENABLE_D3D12 && ETERNAL_PLATFORM_WINDOWS

#include "d3d12/D3D12Device.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D12WindowsDevice final : public D3D12Device
		{
		public:
		
			static void Initialize();
			static void Destroy();
			static IDXGIFactory4* GetDXGIFactory();

			D3D12WindowsDevice(_In_ uint32_t InDeviceIndex);
			~D3D12WindowsDevice();

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

		};
	}
}

#endif
