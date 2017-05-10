#ifndef _D3D12_DEVICE_HPP_
#define _D3D12_DEVICE_HPP_

#include <cstdint>
#include "NextGenGraphics/Device.hpp"

struct IDXGIFactory4;
struct IDXGIAdapter1;
struct ID3D12Device;

#ifdef ETERNAL_DEBUG
struct ID3D12Debug;
#endif

namespace Eternal
{
	namespace Graphics
	{
		class Window;

		class D3D12Device : public Device
		{
		public:
			static void Initialize();
			static IDXGIFactory4* GetDXGIFactory();

			D3D12Device(_In_ uint32_t DeviceIndex);

			inline ID3D12Device* GetD3D12Device() { return _Device; }
			virtual uint32_t GetDeviceMask() const override;
			virtual DeviceType GetDeviceType() const override { return D3D12; }

#ifdef ETERNAL_DEBUG
			static ID3D12Debug*		_Debug;
#endif
			static IDXGIFactory4*	_DXGIFactory;

			IDXGIAdapter1*			_DXGIAdapter	= nullptr;
			ID3D12Device*			_Device			= nullptr;

			uint32_t				_DeviceMask		= 0xFFFFFFFF;
		};
	}
}

#endif
