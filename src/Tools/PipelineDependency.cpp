#include "Tools/PipelineDependency.hpp"

namespace Eternal
{
	namespace Graphics
	{
		void PipelineDependency::RegisterShaderDependency(_In_ Shader* InShader, _In_ const string& InSource)
		{
			auto ShadersIterator = _SourceToShaders.find(InSource);
			if (ShadersIterator == _SourceToShaders.end())
				ShadersIterator = _SourceToShaders.emplace(InSource, vector<Shader*>()).first;

			ETERNAL_ASSERT(std::find(ShadersIterator->second.begin(), ShadersIterator->second.end(), InShader) == ShadersIterator->second.end());
			ShadersIterator->second.push_back(InShader);
		}

		void PipelineDependency::RegisterPipelineDependency(_In_ Pipeline* InPipeline, _In_ Shader* InShader)
		{
			auto PipelinesIterator = _ShaderToPipelines.find(InShader);
			if (PipelinesIterator == _ShaderToPipelines.end())
				PipelinesIterator = _ShaderToPipelines.emplace(InShader, vector<Pipeline*>()).first;

			ETERNAL_ASSERT(std::find(PipelinesIterator->second.begin(), PipelinesIterator->second.end(), InPipeline) == PipelinesIterator->second.end());
			PipelinesIterator->second.push_back(InPipeline);
		}

		void PipelineDependency::RegisterIncludeDependency(_In_ const string& InInclude, _In_ const string& InSource)
		{
		}
	}
}
