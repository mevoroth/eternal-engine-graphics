#include "Tools/PipelineDependency.hpp"

namespace Eternal
{
	namespace Graphics
	{
		void PipelineDependency::RegisterShaderDependency(_In_ Shader* InShader, _In_ const string& InSource)
		{
			auto ShadersIterator = _SourceToShaders.find(InSource);
			if (ShadersIterator == _SourceToShaders.end())
				ShadersIterator = _SourceToShaders.emplace(InSource, unordered_set<Shader*>()).first;

			if (ShadersIterator->second.find(InShader) == ShadersIterator->second.end())
				ShadersIterator->second.emplace(InShader);
		}

		void PipelineDependency::RegisterPipelineDependency(_In_ Pipeline* InPipeline, _In_ Shader* InShader)
		{
			ETERNAL_ASSERT(InShader);
			auto PipelinesIterator = _ShaderToPipelines.find(InShader);
			if (PipelinesIterator == _ShaderToPipelines.end())
				PipelinesIterator = _ShaderToPipelines.emplace(InShader, vector<Pipeline*>()).first;

			if (std::find(PipelinesIterator->second.begin(), PipelinesIterator->second.end(), InPipeline) == PipelinesIterator->second.end())
				PipelinesIterator->second.push_back(InPipeline);
		}

		ResolvedPipelineDependency PipelineDependency::ResolveSource(_In_ const string& InSource)
		{
			auto ShadersIterator = _SourceToShaders.find(InSource);
			if (ShadersIterator == _SourceToShaders.end())
				return ResolvedPipelineDependency();

			unordered_set<Shader*>& Shaders = ShadersIterator->second;

			vector<Pipeline*> Pipelines;
			Pipelines.reserve(Shaders.size());

			for (auto ShaderIterator = Shaders.begin(); ShaderIterator != Shaders.end(); ++ShaderIterator)
			{
				auto FoundPipeline = _ShaderToPipelines.find(*ShaderIterator);
				if (FoundPipeline != _ShaderToPipelines.end())
					Pipelines.insert(Pipelines.end(), FoundPipeline->second.begin(), FoundPipeline->second.end());
			}

			return ResolvedPipelineDependency(
				Shaders,
				Pipelines
			);
		}

		//////////////////////////////////////////////////////////////////////////
		// ResolvedPipelineDependency
		unordered_set<Shader*> ResolvedPipelineDependency::EmptyShaders;

		ResolvedPipelineDependency::ResolvedPipelineDependency()
			: Shaders(EmptyShaders)
		{
		}

		ResolvedPipelineDependency::ResolvedPipelineDependency(_In_ unordered_set<Shader*>& InShaders, _In_ vector<Pipeline*>& InPipelines)
			: Shaders(InShaders)
			, Pipelines(InPipelines)
		{
		}

		bool ResolvedPipelineDependency::IsShaderSourceResolved() const
		{
			return &Shaders != &EmptyShaders;
		}

		ResolvedPipelineDependency& ResolvedPipelineDependency::operator=(_In_ const ResolvedPipelineDependency& InOutPipelineDependency)
		{
			if (this != &InOutPipelineDependency)
			{
				Shaders		= InOutPipelineDependency.Shaders;
				Pipelines	= InOutPipelineDependency.Pipelines;
			}
			return *this;
		}
	}
}
