#include "Graphics/Shader.hpp"

using namespace Eternal::Graphics;

Shader::Shader(_In_ const string& Name)
	: _Name(Name)
{
}

const string& Shader::GetName() const
{
	return _Name;
}
