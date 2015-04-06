#include "Graphics/MaterialProperty.hpp"

#include "GraphicsSettings.hpp"
#include "Types/Types.hpp"

using namespace Eternal::Graphics;
using namespace Eternal::Types;

static const unsigned int PARAM_SIZE[MaterialProperty::ParamType::PARAMTYPE_COUNT] = {
	sizeof(void*),
	sizeof(float),
	sizeof(Vector2),
	sizeof(Vector3),
	sizeof(Vector4),
	sizeof(Vector4)
};

unsigned int MaterialProperty::GetSize(const ParamType& Type)
{
	return PARAM_SIZE[Type];
}

MaterialProperty::MaterialProperty(const string& name, const ParamType& type)
	: Name(name)
	, Type(type)
{
}
