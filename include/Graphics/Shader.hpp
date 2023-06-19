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

		struct ShaderCreateInformation
		{
			ShaderCreateInformation(_In_ ShaderType InStage, _In_ const char* InName, _In_ const char* InFileName, _In_ const vector<string>& InDefines = vector<string>())
				: Stage(InStage)
				, Name(InName)
				, FileName(InFileName)
				, Defines(InDefines)
			{
			}

			ShaderType Stage = ShaderType::SHADER_TYPE_COUNT;
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

		protected:
			Shader(_Inout_ GraphicsContext& InOutContext, _In_ const ShaderCreateInformation& InShaderCreateInformation);

		private:
			ShaderCreateInformation _ShaderCreateInformation;
		};
	}
}
