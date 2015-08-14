#include "Graphics/Shader.hpp"

using namespace Eternal::Graphics;

Shader::Shader(const string& Name)
	: _Name(Name)
{
}

const string& Shader::GetName() const
{
	return _Name;
}
