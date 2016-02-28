#ifndef _D3D12_RENDERER_HPP_
#define _D3D12_RENDERER_HPP_

#include <cstdint>

struct IDXGIFactory4;
struct IDXGIAdapter1;
struct ID3D12Device;
struct ID3D12Debug;

namespace Eternal
{
	namespace Graphics
	{
		class D3D12Renderer
		{
		public:
			const uint32_t MAX_DXGI_ADAPTER = 10;
			D3D12Renderer();

			inline ID3D12Device* GetDevice()
			{
				return _Device;
			}

		private:
			IDXGIFactory4* _DXGIFactory = nullptr;
			IDXGIAdapter1* _DXGIAdapter = nullptr;
			ID3D12Device* _Device = nullptr;
#ifdef ETERNAL_DEBUG
			ID3D12Debug* _Debug = nullptr;
#endif
		};
	}
}

#endif
