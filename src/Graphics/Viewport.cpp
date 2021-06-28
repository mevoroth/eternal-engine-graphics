#include "Graphics/Viewport.hpp"

using namespace Eternal::Graphics;

Viewport::Viewport(_In_ int32_t InX, _In_ int32_t InY, _In_ int32_t InWidth, _In_ int32_t InHeight)
	: _X(InX)
	, _Y(InY)
	, _Width(InWidth)
	, _Height(InHeight)
{
}
