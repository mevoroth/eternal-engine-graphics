#include "Graphics_deprecated/Texture.hpp"

#include "Graphics_deprecated/Format.hpp"

using namespace Eternal::Graphics;

Texture::Texture(_In_ const Format& FormatObj, _In_ uint32_t Width /*= 1*/, _In_ uint32_t Height /*= 1*/)
	: _Format(FormatObj)
	, _Width(Width)
	, _Height(Height)
{
	ETERNAL_ASSERT(Width > 0);
	ETERNAL_ASSERT(Height > 0);
}
