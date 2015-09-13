#ifndef _D3D11_SAMPLER_HPP_
#define _D3D11_SAMPLER_HPP_

#include "Graphics/Sampler.hpp"

struct ID3D11SamplerState;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Sampler : public Sampler
		{
		public:
			/**
			 * Point/Linear filtering
			 */
			D3D11Sampler(_In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W);
			/**
			 * Anisotropic filtering
			 */
			D3D11Sampler(_In_ bool Comparison, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W);
			~D3D11Sampler();
			ID3D11SamplerState* GetD3D11SamplerState();

		private:
			ID3D11SamplerState* _SamplerState = nullptr;
		};
	}
}

#endif
