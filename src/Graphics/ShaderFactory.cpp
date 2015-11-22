#include "Graphics/ShaderFactory.hpp"

#include "Macros/Macros.hpp"

using namespace Eternal::Graphics;

ShaderFactory* ShaderFactory::_Inst = nullptr;

ShaderFactory::ShaderFactory()
{
	ETERNAL_ASSERT(!_Inst);
	_Inst = this;
}

ShaderFactory* ShaderFactory::Get()
{
	ETERNAL_ASSERT(_Inst);
	return _Inst;
}
