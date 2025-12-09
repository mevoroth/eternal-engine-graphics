#include "Graphics/GraphicsContext.hpp"

#if ETERNAL_PLATFORM_ANDROID

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

		//class AndroidGraphicsContext : public GraphicsContext
		//{
		//public:



		//};
	}
}

#endif
