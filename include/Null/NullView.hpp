#pragma once

#include "Graphics/View.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class NullView : public View
		{
		public:
			NullView(_In_ const ViewCreateInformation& InViewCreateInformation)
				: View(InViewCreateInformation)
			{
			}
		};
	}
}
