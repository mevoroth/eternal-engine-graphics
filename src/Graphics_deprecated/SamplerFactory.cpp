#include "Graphics/SamplerFactory.hpp"

#include "d3d11/D3D11Sampler.hpp"

namespace Eternal
{
	namespace Graphics
	{
		/**
		 * Point/Linear filtering
		 */
		Sampler* CreateSampler(_In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ bool Comparison, _In_ const Sampler::AddressMode& U, _In_ const Sampler::AddressMode& V, _In_ const Sampler::AddressMode& W)
		{
			return new D3D11Sampler(MINLinear, MAGLinear, MIPLinear, Comparison, U, V, W);
		}
		/**
		 * Anisotropic filtering
		 */
		Sampler* CreateSampler(_In_ bool Comparison, _In_ const Sampler::AddressMode& U, _In_ const Sampler::AddressMode& V, _In_ const Sampler::AddressMode& W)
		{
			return new D3D11Sampler(Comparison, U, V, W);
		}
	}
}
