#pragma once

#include "Graphics/InputLayout.hpp"

namespace Eternal
{
	namespace Graphics
	{
		enum class VertexDataType;
		class GraphicsContext;
		class Device;

		InputLayout* CreateInputLayout(_In_ GraphicsContext& Context, _In_ const VertexDataType DataType[] = nullptr, _In_ uint32_t DataTypeCount = 0);
	}
}
