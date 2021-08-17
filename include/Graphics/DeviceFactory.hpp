#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class Window;
		class GraphicsContext;
		enum class DeviceType;

		Device* CreateDevice(_In_ const DeviceType& InDeviceType, _In_ Window& InWindow);
		Device* CreateDevice(_Inout_ GraphicsContext& InContext, _In_ const DeviceType& InDeviceType);
		void DestroyDevice(_Inout_ Device*& InOutDevice);
	}
}
