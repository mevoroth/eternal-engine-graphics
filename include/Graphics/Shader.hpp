#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include <string>

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		enum class ShaderType;

		struct ShaderCreateInformation
		{
			ShaderCreateInformation(const ShaderType& Stage, const char* Name, const char* FileName)
				: Stage(Stage)
				, Name(Name)
				, FileName(FileName)
			{
			}

			const ShaderType& Stage;
			const char* Name = nullptr;
			const char* FileName = nullptr;
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

#endif
