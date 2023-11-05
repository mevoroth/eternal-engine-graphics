#pragma once

#if ETERNAL_ENABLE_D3D12

#include "Graphics/Sampler.hpp"
#include "d3d12/D3D12Utils.hpp"
#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class GraphicsContext;

		class D3D12Sampler final : public Sampler
		{
		public:
			D3D12Sampler(_In_ GraphicsContext& InContext, _In_ const SamplerCreateInformation& InSamplerCreateInformation);

			const D3D12_GPU_DESCRIPTOR_HANDLE& GetD3D12GPUDescriptorHandle() const { return _D3D12Handle.D3D12GPUDescriptorHandle; }
			void GetD3D12StaticSampler(_Out_ D3D12_STATIC_SAMPLER_DESC& StaticSamplerDesc) const;

		private:
			D3D12_FILTER GetD3D12Filering() const;
			D3D12_TEXTURE_ADDRESS_MODE GetD3D12U() const;
			D3D12_TEXTURE_ADDRESS_MODE GetD3D12V() const;
			D3D12_TEXTURE_ADDRESS_MODE GetD3D12W() const;

			D3D12Handle _D3D12Handle;
		};
	}
}

#endif
