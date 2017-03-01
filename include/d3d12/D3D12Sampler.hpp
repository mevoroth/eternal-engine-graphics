#ifndef _D3D12_SAMPLER_HPP_
#define _D3D12_SAMPLER_HPP_

#include "Graphics/Sampler.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D12Device;
		class D3D12DescriptorHeap;

		class D3D12Sampler : public Sampler
		{
		public:
			/**
			 * Point/Linear filtering
			 */
			D3D12Sampler(_In_ D3D12Device& DeviceObj, _In_ D3D12DescriptorHeap& DescriptorHeapObj, _In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W);
			/**
			 * Anisotropic filtering
			 */
			D3D12Sampler(_In_ D3D12Device& DeviceObj, _In_ D3D12DescriptorHeap& DescriptorHeapObj, _In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W);
		};
	}
}

#endif
