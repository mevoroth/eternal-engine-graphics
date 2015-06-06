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
			ID3D11SamplerState* GetD3D11SamplerState();

		private:
			ID3D11SamplerState* _SamplerState = nullptr;
		};
	}
}

#endif
