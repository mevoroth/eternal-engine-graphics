#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class DeviceType;
		class GraphicsContext;
		struct WindowsGraphicsContextCreateInformation;
		struct BakeGraphicsContextCreateInformation;

		GraphicsContext* CreateGraphicsContext(_In_ const DeviceType& InDeviceType);
		GraphicsContext* CreateGraphicsContext(_In_ const WindowsGraphicsContextCreateInformation& InWindowsGraphicsContextCreateInformation);
		GraphicsContext* CreateGraphicsContext(_In_ const BakeGraphicsContextCreateInformation& InBakeGraphicsContextCreateInformation);
		void DestroyGraphicsContext(_Inout_ GraphicsContext*& Context);
	}
}

#if ETERNAL_USE_PRIVATE
#include "Graphics/GraphicsContextFactoryPrivate.hpp"
#endif
