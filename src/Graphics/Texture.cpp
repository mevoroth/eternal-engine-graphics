#include "Graphics/Texture.hpp"

#include "Macros/Macros.hpp"

using namespace Eternal::Graphics;

Texture::Texture(int Width, int Height)
	: _Width(Width)
	, _Height(Height)
{
	ETERNAL_ASSERT(Width > 0);
	ETERNAL_ASSERT(Height > 0);
}
