#pragma once

#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"

struct IDXGIFactory4;
struct IDXGIAdapter1;
struct ID3D12Device;
struct ID3D12Device5;

#if ETERNAL_USE_DEBUG_LAYER
struct ID3D12Debug;
struct IDXGIInfoQueue;
struct ID3D12InfoQueue;
struct IDXGIDebug;
#endif

namespace Eternal
{
	namespace Graphics
	{
		class Window;

		class D3D12Device final : public Device
		{
		public:
			static void Initialize();
			static void Destroy();
			static IDXGIFactory4* GetDXGIFactory();

			D3D12Device(_In_ uint32_t DeviceIndex);
			~D3D12Device();

			virtual uint32_t GetDeviceMask() const override final;
			virtual DeviceType GetDeviceType() const override final { return DeviceType::D3D12; }

			inline ID3D12Device* GetD3D12Device() { return _Device; }
			inline ID3D12Device5* GetD3D12Device5() { return _Device5; }

		private:
#if ETERNAL_USE_DEBUG_LAYER
			static ID3D12Debug*		_Debug;
			static IDXGIInfoQueue*	_DXGIInfoQueue;
			static IDXGIDebug*		_DXGIDebug;
			ID3D12InfoQueue*		_InfoQueue		= nullptr;
#endif
			static bool				_IsInitialized;
			static IDXGIFactory4*	_DXGIFactory;

			IDXGIAdapter1*			_DXGIAdapter	= nullptr;
			ID3D12Device*			_Device			= nullptr;
			ID3D12Device5*			_Device5		= nullptr;

			uint32_t				_DeviceMask		= 0xFFFFFFFF;
		};
	}
}
