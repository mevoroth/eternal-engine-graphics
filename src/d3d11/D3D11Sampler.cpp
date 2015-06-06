#include "d3d11/D3D11Sampler.hpp"

using namespace Eternal::Graphics;

ID3D11SamplerState* D3D11Sampler::GetD3D11SamplerState()
{
	return _SamplerState;
}
