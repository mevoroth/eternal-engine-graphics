#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <unordered_set>

namespace Eternal
{
	namespace Graphics
	{
		class Pipeline;
		class Shader;

		using namespace std;

		struct ResolvedPipelineDependency
		{
			ResolvedPipelineDependency();

			bool IsShaderSourceResolved() const;

			unordered_set<Shader*>&	Shaders;
			vector<Pipeline*>		Pipelines;

			ResolvedPipelineDependency& operator=(_In_ const ResolvedPipelineDependency& InOutPipelineDependency);

		private:

			static unordered_set<Shader*> EmptyShaders;

			ResolvedPipelineDependency(_In_ unordered_set<Shader*>& InShaders, _In_ vector<Pipeline*>& InPipelines);

			friend class PipelineDependency;
		};

		class PipelineDependency
		{
		public:

			void RegisterShaderDependency(_In_ Shader* InShader, _In_ const string& InSource);
			void RegisterPipelineDependency(_In_ Pipeline* InPipeline, _In_ Shader* InShader);
			ResolvedPipelineDependency ResolveSource(_In_ const string& InSource);
			const unordered_set<Pipeline*>& GetPipelines() const { return _Pipelines; }

		private:

			unordered_set<Pipeline*>						_Pipelines;
			unordered_map<Shader*, vector<Pipeline*>>		_ShaderToPipelines;
			unordered_map<string, unordered_set<Shader*>>	_SourceToShaders;
		};
	}
}
