#if ETERNAL_PLATFORM_ANDROID

#include "Graphics/GraphicsContext.hpp"

#include "Android/AndroidOutputDevice.hpp"

struct ANativeWindow;

namespace Eternal
{
	namespace Graphics
	{
		struct AndroidGraphicsContextCreateInformation : public GraphicsContextCreateInformation
		{
			AndroidGraphicsContextCreateInformation(_In_ const RenderSettings& InSettings)
				: GraphicsContextCreateInformation(InSettings)
			{
			}

			ANativeWindow* NativeWindow = nullptr;
		};

		class AndroidGraphicsContext
		{
		public:

			AndroidGraphicsContext(_In_ const AndroidGraphicsContextCreateInformation& InGraphicsContextCreateInformation);

			ANativeWindow* GetAndroidNativeWindow();

		protected:

			AndroidOutputDeviceCreateInformation	_AndroidOutputDeviceCreateInformation;
			AndroidOutputDevice						_AndroidOutputDevice;

		};
	}
}

#endif
