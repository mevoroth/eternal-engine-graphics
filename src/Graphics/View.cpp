#include "Graphics/View.hpp"

namespace Eternal
{
	namespace Graphics
	{
		View::View(_In_ const ViewCreateInformation& InViewCreateInformation)
			: _ViewCreateInformation(InViewCreateInformation)
		{
			ETERNAL_ASSERT(InViewCreateInformation.ResourceViewType != ViewType::VIEW_UNKNOWN);
		}
	}
}
