#pragma once

#include <string>
#include "Graphics/ShaderType.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

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
			const vector<string> Defines;
		};

		class Shader
		{
		public:
			virtual ~Shader() {}
			virtual bool IsShaderCompiled() const = 0;
			const string& GetName() const;
			const string& GetFileName() const;
			ShaderCreateInformation& GetShaderCreateInformation();
			const ShaderCreateInformation& GetShaderCreateInformation() const;

		protected:
			Shader(_Inout_ GraphicsContext& InOutContext, _In_ const ShaderCreateInformation& InShaderCreateInformation);

		private:
			ShaderCreateInformation _ShaderCreateInformation;
		};
	}
}
