#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class Resource;
		class Pipeline;

		class ShaderTable
		{
		protected:
			ShaderTable(_In_ Pipeline& InPipeline)
				: _Pipeline(InPipeline)
			{
			}
			
			Pipeline&	_Pipeline;
			Resource*	_ShaderTable	= nullptr;
		};
	}
}
