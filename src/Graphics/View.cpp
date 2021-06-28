#include "Graphics/View.hpp"

#include "Graphics/Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		View::View(_In_ const ViewCreateInformation& InViewCreateInformation)
			: _ViewCreateInformation(InViewCreateInformation)
		{
			ETERNAL_ASSERT(InViewCreateInformation.ResourceViewType != ViewType::VIEW_UNKNOWN);
		}

		const TransitionState& View::GetResourceTransition() const
		{
			return GetViewCreateInformation().GraphicsResource.GetResourceState();
		}

		ResourceType View::GetResourceType() const
		{
			return GetViewCreateInformation().GraphicsResource.GetResourceType();
		}
	}
}
