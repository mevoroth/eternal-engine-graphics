#pragma once

#include "Graphics/ShaderTable.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class NullShaderTable : public ShaderTable
		{
		public:

			NullShaderTable(_In_ Pipeline& InPipeline)
				: ShaderTable(InPipeline)
			{
			}

		};
	}
}
