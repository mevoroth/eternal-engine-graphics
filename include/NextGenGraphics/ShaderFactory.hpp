#pragma once

#include <string>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		enum class ShaderType;

		class Device;
		class Shader;

		class ShaderFactory
		{
		public:
			static ShaderFactory* Get();

			ShaderFactory();
			virtual ~ShaderFactory();

			Shader* GetShader(_In_ Device& DeviceObj, _In_ const string& Name, _In_ const string& Source, _In_ const ShaderType& Type, _In_ const vector<string>& Defines = vector<string>());

		private:
			static ShaderFactory* _Inst;

			Shader* _Find(_In_ const string& Name);
			Shader* _Create(_In_ Device& DeviceObj, _In_ const string& Name, _In_ const string& Source, _In_ const ShaderType& Type, _In_ const vector<string>& Defines);

			vector<Shader*>	_Shaders;
		};
	}
}
