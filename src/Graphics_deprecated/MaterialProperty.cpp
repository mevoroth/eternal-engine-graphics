#include "Graphics_deprecated/MaterialProperty.hpp"

#include "GraphicsSettings.hpp"
#include "Types/Types.hpp"

using namespace Eternal::Graphics;
using namespace Eternal::Types;

static const uint32_t PARAM_SIZE[MaterialProperty::ParamType::PARAMTYPE_COUNT] = {
	sizeof(void*),
	sizeof(float),
	sizeof(Vector4)
};

uint32_t MaterialProperty::GetSize(const ParamType& Type)
{
	return PARAM_SIZE[Type];
}

MaterialProperty::MaterialProperty(const string& name, const ParamType& type)
	: Name(name)
	, Type(type)
{
}
