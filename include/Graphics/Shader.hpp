#pragma once

#include <string>
#include "Graphics/ShaderType.hpp"

namespace Eternal
{
	namespace FileSystem
	{
		class File;
	}
	namespace Graphics
	{
		using namespace std;
		using namespace Eternal::FileSystem;

		enum class ShaderType;
		class GraphicsContext;

		enum class D3D12ShaderCompilerType
		{
			D3D12_SHADER_COMPILER_TYPE_FXC = 0,
			D3D12_SHADER_COMPILER_TYPE_DXC
		};

		struct ShaderCreateInformation
		{
			ShaderCreateInformation(_In_ ShaderType InStage, _In_ const char* InName, _In_ const char* InFileName, _In_ const vector<string>& InDefines = vector<string>());

			ShaderType Stage								= ShaderType::SHADER_TYPE_COUNT;
			D3D12ShaderCompilerType D3D12CompilerPreference	= D3D12ShaderCompilerType::D3D12_SHADER_COMPILER_TYPE_FXC;
			string Name;
			string FileName;
			vector<string> Defines;

		private:

			ShaderCreateInformation() {}

			friend class Shader;
		};

		class Shader
		{
		public:
			virtual ~Shader() {}
			virtual bool IsShaderCompiled() const = 0;
			virtual void SerializeShader(_Inout_ File* InOutFile);

			const string& GetName() const;
			const string& GetFileName() const;
			ShaderCreateInformation& GetShaderCreateInformation();
			const ShaderCreateInformation& GetShaderCreateInformation() const;

		protected:
			Shader(_Inout_ GraphicsContext& InOutContext, _In_ const ShaderCreateInformation& InShaderCreateInformation);
			Shader();

		private:
			ShaderCreateInformation _ShaderCreateInformation;
		};
	}
}
