#include "Graphics/Texture.hpp"

using namespace Eternal::Graphics;

Texture::Texture(int Width, int Height)
	: _Width(Width)
	, _Height(Height)
{
	assert(Width > 0);
	assert(Height > 0);
}
