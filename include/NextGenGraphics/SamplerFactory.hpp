#ifndef _SAMPLER_FACTORY_HPP_
#define _SAMPLER_FACTORY_HPP_

#include "Graphics/Sampler.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class DescriptorHeap;

		Sampler* CreateSampler(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ bool Comparison, _In_ const Sampler::AddressMode& U, _In_ const Sampler::AddressMode& V, _In_ const Sampler::AddressMode& W);
	}
}

#endif
