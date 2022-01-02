#pragma once

#include <string>

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		enum class ShaderType;

		struct ShaderCreateInformation
		{
			ShaderCreateInformation(_In_ const ShaderType& InStage, _In_ const char* InName, _In_ const char* InFileName, _In_ const vector<string>& InDefines = vector<string>())
				: Stage(InStage)
				, Name(InName)
				, FileName(InFileName)
				, Defines(InDefines)
			{
			}

			const ShaderType& Stage;
			const char* Name = nullptr;
			const char* FileName = nullptr;
			const vector<string> Defines;
		};

		class Shader
		{
		public:
			virtual ~Shader() {}
			const string& GetName() const;

		protected:
			Shader(_In_ const string& InName);

		private:
			string _Name;
		};
	}
}
