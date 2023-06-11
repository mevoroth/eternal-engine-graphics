#pragma once

#include <map>
#include <vector>
#include <string>

namespace Eternal
{
	namespace Graphics
	{
		class Pipeline;
		class Shader;

		using namespace std;

		class PipelineDependency
		{
		public:

			void RegisterShaderDependency(_In_ Shader* InShader, _In_ const string& InSource);
			void RegisterPipelineDependency(_In_ Pipeline* InPipeline, _In_ Shader* InShader);
			void RegisterIncludeDependency(_In_ const string& InInclude, _In_ const string& InSource);

		private:

			map<Shader*, vector<Pipeline*>>	_ShaderToPipelines;
			map<string, vector<Shader*>>	_IncludeToShaders;
			map<string, vector<Shader*>>	_SourceToShaders;
			map<string, vector<string>>		_IncludeToFiles;
		};
	}
}
