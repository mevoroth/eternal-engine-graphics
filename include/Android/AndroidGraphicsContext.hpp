#if ETERNAL_PLATFORM_ANDROID

#include "Graphics/GraphicsContext.hpp"

#include "Android/AndroidOutputDevice.hpp"

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
		};

		class AndroidGraphicsContext
		{
		public:

			AndroidGraphicsContext(_In_ const AndroidGraphicsContextCreateInformation& InGraphicsContextCreateInformation);

		protected:

			AndroidOutputDeviceCreateInformation	_AndroidOutputDeviceCreateInformation;
			AndroidOutputDevice						_AndroidOutputDevice;

		};
	}
}

#endif
