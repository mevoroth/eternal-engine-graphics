#pragma once

namespace Eternal
{
	namespace Graphics
	{
		struct OutputDeviceCreateInformation
		{
			int Width			= 640;
			int Height			= 480;
			bool VSync			= false;
			bool Windowed		= true;

		protected:
			OutputDeviceCreateInformation(
				_In_ int InWidth,
				_In_ int InHeight,
				_In_ bool InVSync = false,
				_In_ bool InWindowed = true
			)
				: Width(InWidth)
				, Height(InHeight)
				, VSync(InVSync)
				, Windowed(InWindowed)
			{
			}
		};

		class OutputDevice
		{
		public:
			inline bool GetVSync() const { return _OutputDeviceCreateInformation.VSync; }
			inline int GetWidth() const { return _OutputDeviceCreateInformation.Width; }
			inline int GetHeight() const { return _OutputDeviceCreateInformation.Height; }
			virtual const char* GetApplicationName() const { return ""; }

		protected:
			OutputDevice(_In_ const OutputDeviceCreateInformation& InOutputDeviceCreateInformation);
			virtual ~OutputDevice() {}

			OutputDeviceCreateInformation _OutputDeviceCreateInformation;
		};
	}
}
