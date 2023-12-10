#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

namespace Eternal
{
	namespace FileSystem
	{
		class File;
	}

	namespace Graphics
	{
		using namespace Eternal::FileSystem;
		using namespace std;

		class GraphicsContext;
		class Pipeline;
		class Shader;
		struct PipelineCreateInformation;

		enum class PipelineSerializationMode
		{
			PIPELINE_SERIALIZATION_MODE_READ,
			PIPELINE_SERIALIZATION_MODE_WRITE
		};

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

			friend class PipelineLibrary;
		};

		class PipelineLibrary
		{
		public:

			void RegisterShaderDependency(_In_ Shader* InShader, _In_ const string& InSource);
			void RegisterPipelineDependency(_In_ Pipeline* InPipeline, _In_ Shader* InShader);
			ResolvedPipelineDependency ResolveSource(_In_ const string& InSource);
			const unordered_set<Pipeline*>& GetPipelines() const { return _Pipelines; }
			void SerializePipelineLibrary(_Inout_ GraphicsContext& InOutContext, _Inout_ File* InOutFile);
			Pipeline* GetPipeline(_In_ const PipelineCreateInformation& InPipelineCreateInformation);

		private:

			unordered_set<Pipeline*>						_Pipelines;
			unordered_map<Shader*, vector<Pipeline*>>		_ShaderToPipelines;
			unordered_map<string, unordered_set<Shader*>>	_SourceToShaders;
		};
	}
}
