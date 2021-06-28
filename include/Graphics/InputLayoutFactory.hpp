#pragma once

#include "Graphics/InputLayout.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;
		class Device;

		InputLayout* CreateInputLayout(_In_ GraphicsContext& Context, _In_ const InputLayout::VertexDataType DataType[] = nullptr, _In_ uint32_t DataTypeCount = 0);
	}
}
