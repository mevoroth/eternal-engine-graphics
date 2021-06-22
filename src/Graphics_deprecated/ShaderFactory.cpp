#include "Graphics_deprecated/ShaderFactory.hpp"

using namespace Eternal::Graphics;

ShaderFactory* ShaderFactory::_Instance = nullptr;

ShaderFactory::ShaderFactory()
{
	ETERNAL_ASSERT(!_Instance);
	_Instance = this;
}

ShaderFactory* ShaderFactory::Get()
{
	ETERNAL_ASSERT(_Instance);
	return _Instance;
}
