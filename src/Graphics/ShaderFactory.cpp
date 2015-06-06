#include "Graphics/ShaderFactory.hpp"

#include <cassert>

using namespace Eternal::Graphics;

ShaderFactory* ShaderFactory::_Inst = nullptr;

ShaderFactory::ShaderFactory()
{
	assert(_Inst);
	_Inst = this;
}
