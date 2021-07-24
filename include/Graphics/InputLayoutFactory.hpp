#pragma once

#include "Graphics/InputLayout.hpp"

namespace Eternal
{
	namespace Graphics
	{
		enum class VertexDataType;
		class GraphicsContext;
		class Device;

		InputLayout* CreateInputLayout(_In_ GraphicsContext& InContext, _In_ const vector<VertexStreamBase>& InVertexStreams = vector<VertexStreamBase>());
		void DestroyInputLayout(_Inout_ InputLayout*& InOutInputLayout);
	}
}
