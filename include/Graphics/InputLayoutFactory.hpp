#ifndef _INPUT_LAYOUT_FACTORY_HPP_
#define _INPUT_LAYOUT_FACTORY_HPP_

#include <cstdint>
#include "Graphics/InputLayout.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;
		class Device;

		InputLayout* CreateInputLayout(_In_ Device& DeviceObj, _In_ const InputLayout::VertexDataType DataType[] = nullptr, _In_ uint32_t DataTypeCount = 0);
		InputLayout* CreateInputLayout(_In_ GraphicsContext& Context, _In_ const InputLayout::VertexDataType DataType[] = nullptr, _In_ uint32_t DataTypeCount = 0);
	}
}

#endif
