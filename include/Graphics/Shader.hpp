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

			ShaderType Stage		= ShaderType::SHADER_TYPE_COUNT;
			const char* Name		= nullptr;
			const char* FileName	= nullptr;
			const vector<string> Defines;
		};

		class Shader
		{
		public:
			virtual ~Shader() {}
			const string& GetName() const;

		protected:
			Shader(_Inout_ GraphicsContext& InOutContext, _In_ const string& InName, _In_ const string& InFileName);

		private:
			string _Name;
			string _FileName;
		};
	}
}
