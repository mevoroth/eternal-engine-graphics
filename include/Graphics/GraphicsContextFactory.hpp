#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;
		struct WindowsGraphicsContextCreateInformation;

		GraphicsContext* CreateGraphicsContext(_In_ const WindowsGraphicsContextCreateInformation& InWindowsGraphicsContextCreateInformation);
		void DestroyGraphicsContext(_Inout_ GraphicsContext*& Context);
	}
}

#if ETERNAL_USE_PRIVATE
#include "Graphics/GraphicsContextFactoryPrivate.hpp"
#endif
