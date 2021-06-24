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
			ShaderCreateInformation(const ShaderType& Stage, const char* Name, const char* FileName, const vector<string>& Defines = vector<string>())
				: Stage(Stage)
				, Name(Name)
				, FileName(FileName)
				, Defines(Defines)
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
			Shader(_In_ const string& Name);
			virtual ~Shader() {}
			const string& GetName() const;

		private:
			string _Name;
		};
	}
}
