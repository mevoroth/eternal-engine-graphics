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

		Resource& View::GetResource()
		{
			ETERNAL_ASSERT(_ViewCreateInformation.GraphicsResource);
			return *_ViewCreateInformation.GraphicsResource;
		}

		const Resource& View::GetResource() const
		{
			ETERNAL_ASSERT(_ViewCreateInformation.GraphicsResource);
			return *_ViewCreateInformation.GraphicsResource;
		}

		const TransitionState& View::GetResourceTransition() const
		{
			return GetResource().GetResourceState();
		}

		ResourceType View::GetResourceType() const
		{
			return GetResource().GetResourceType();
		}
	}
}
