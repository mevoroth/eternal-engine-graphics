#include "Graphics/View.hpp"

#include "Graphics/Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		std::unordered_set<View*> View::ViewHistory;

		View::View(_In_ const ViewCreateInformation& InViewCreateInformation)
			: _ViewCreateInformation(InViewCreateInformation)
		{
			ETERNAL_ASSERT(InViewCreateInformation.ResourceViewType != ViewType::VIEW_UNKNOWN);
			ViewHistory.insert(this);
		}

		View::~View()
		{
			ViewHistory.erase(this);
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
