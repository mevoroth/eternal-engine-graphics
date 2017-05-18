#ifndef _SAMPLER_FACTORY_HPP_
#define _SAMPLER_FACTORY_HPP_

#include "Graphics/Sampler.hpp"

namespace Eternal
{
	namespace Graphics
	{
		enum AddressMode;
		class Sampler;

		/**
		 * Point/Linear filtering
		 */
		Sampler* CreateSampler(_In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W);
		/**
		 * Anisotropic filtering
		 */
		Sampler* CreateSampler(_In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W);
	}
}

#endif
