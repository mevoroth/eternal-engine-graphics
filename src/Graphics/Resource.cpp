#include "Graphics/Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Resource::Resource(_In_ const ResourceCreateInformation& InResourceCreateInformation, _In_ const ResourceSource& InResourceSource)
			: _ResourceCreateInformation(InResourceCreateInformation)
			, _ResourceSource(InResourceSource)
		{
		}
	}
}
