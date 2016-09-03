#ifndef _SAMPLER_FACTORY_HPP_
#define _SAMPLER_FACTORY_HPP_

#include "Graphics/Sampler.hpp"

namespace Eternal
{
	namespace Graphics
	{
		/**
		 * Point/Linear filtering
		 */
		Sampler* CreateSampler(_In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ bool Comparison, _In_ const Sampler::AddressMode& U, _In_ const Sampler::AddressMode& V, _In_ const Sampler::AddressMode& W);
		/**
		 * Anisotropic filtering
		 */
		Sampler* CreateSampler(_In_ bool Comparison, _In_ const Sampler::AddressMode& U, _In_ const Sampler::AddressMode& V, _In_ const Sampler::AddressMode& W);
	}
}

#endif
