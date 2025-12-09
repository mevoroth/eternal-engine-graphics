#if ETERNAL_PLATFORM_ANDROID

#include "Android/AndroidGraphicsContext.hpp"

namespace Eternal
{
	namespace Graphics
	{
		void DestroyGraphicsContextCreateInformation(_Inout_ GraphicsContextCreateInformation*& InOutGraphicsContextCreateInformation)
		{
			delete InOutGraphicsContextCreateInformation;
			InOutGraphicsContextCreateInformation = nullptr;
		}
	}
}

#endif
