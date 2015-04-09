#include "Graphics/Texture.hpp"

#include <cassert>

using namespace Eternal::Graphics;

Texture::Texture(int Width, int Height)
	: _Width(Width)
	, _Height(Height)
{
	assert(Width > 0);
	assert(Height > 0);
}

inline int Texture::GetWidth() const
{
	return _Width;
}
inline int Texture::GetHeight() const
{
	return _Height;
}

void Texture::SetWidth(_In_ int Width)
{
	_Width = Width;
}
void Texture::SetHeight(_In_ int Height)
{
	_Height = Height;
}
