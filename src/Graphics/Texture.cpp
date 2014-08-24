#include "Graphics/Texture.hpp"

using namespace Eternal::Graphics;

Texture::Texture(int width, int height)
	: _width(width)
	, _height(height)
{
}

inline int Texture::GetWidth() const
{
	return _width;
}
inline int Texture::GetHeight() const
{
	return _height;
}

void Texture::SetWidth(_In_ int width)
{
	_width = width;
}
void Texture::SetHeight(_In_ int height)
{
	_height = height;
}
