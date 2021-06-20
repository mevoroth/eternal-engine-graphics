#include "Graphics_deprecated/Sampler.hpp"

using namespace Eternal::Graphics;

Sampler::Sampler(_In_ bool MINLinear, _In_ bool MAGLinear, _In_ bool MIPLinear, _In_ const AddressMode& U, _In_ const AddressMode& V, _In_ const AddressMode& W/*, _In_ uint32_t Register /* = 0xFFFFFFFF */)
	: /*_Register(Register)
	,*/ _MINLinear(MINLinear)
	, _MAGLinear(MAGLinear)
	, _MIPLinear(MIPLinear)
	, _U(U)
	, _V(V)
	, _W(W)
{
}
