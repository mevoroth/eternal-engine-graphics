#include "Graphics/DescriptorHeap.hpp"

using namespace Eternal::Graphics;

DescriptorHeap::DescriptorHeap(_In_ uint32_t Space, _In_ uint32_t Register)
	: _Space(Space)
	, _Register(Register)
{
}
