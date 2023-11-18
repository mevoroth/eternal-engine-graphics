#pragma once

#include "Graphics/InputLayout.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class NullInputLayout : public InputLayout
		{
		public:

			NullInputLayout(_In_ const vector<VertexStreamBase>& InVertexStreams)
				: InputLayout(InVertexStreams)
			{
			}

		};
	}
}
