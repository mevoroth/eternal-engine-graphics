#ifndef _D3D12_SAMPLER_HPP_
#define _D3D12_SAMPLER_HPP_

#include "Graphics/Sampler.hpp"

struct D3D12_STATIC_SAMPLER_DESC;

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class DescriptorHeap;

		class D3D12Sampler : public Sampler
		{
		public:
			/**
			 * Point/Linear filtering
			 */
			D3D12Sampler(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W);
			/**
			 * Anisotropic filtering
			 */
			D3D12Sampler(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W);
			/**
			 * Point/Linear filtering
			 * Static sampler
			 */
			D3D12Sampler(_In_ Device& DeviceObj, _In_ uint32_t Register, _In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W);

			void GetD3D12StaticSampler(_Out_ D3D12_STATIC_SAMPLER_DESC& StaticSamplerDesc) const;

		private:
			uint32_t _Slot	= 0xFFFFFFFF;
		};
	}
}

#endif
