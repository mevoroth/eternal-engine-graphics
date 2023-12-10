#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class OutputDevice;
		class GraphicsContext;
		enum class DeviceType;

		void PreInitializeDevice(_Inout_ GraphicsContext& InOutContext, _In_ const DeviceType& InDeviceType);
		Device* CreateDevice(_Inout_ GraphicsContext& InOutContext, _In_ const DeviceType& InDeviceType);
		void DestroyDevice(_Inout_ Device*& InOutDevice);
		void PreDestroyDevice(_In_ const DeviceType& InDeviceType);
	}
}
