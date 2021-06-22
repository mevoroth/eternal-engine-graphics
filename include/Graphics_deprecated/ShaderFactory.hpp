#ifndef _SHADER_FACTORY_HPP_
#define _SHADER_FACTORY_HPP_

#include <string>

#include "Graphics_deprecated/InputLayout.hpp"

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class Shader;

		class ShaderFactory
		{
		public:
			static ShaderFactory* Get();

			ShaderFactory();

			virtual void RegisterShaderPath(const string& Path) = 0;
			virtual vector<string>& GetShaderPaths() = 0;
			virtual Shader* CreateVertexShader(_In_ const string& Name, _In_ const string& Src, _In_ const InputLayout::VertexDataType DataType[], _In_ uint32_t Size, _In_ const vector<string>& Defines = vector<string>()) = 0;
			virtual Shader* CreateVertexShader(_In_ const string& Name, _In_ const string& Src, _In_ const vector<string>& Defines = vector<string>()) = 0;
			virtual Shader* CreateGeometryShader(_In_ const string& Name, _In_ const string& Src, _In_ const vector<string>& Defines = vector<string>()) = 0;
			virtual Shader* CreatePixelShader(_In_ const string& Name, _In_ const string& Src, _In_ const vector<string>& Defines = vector<string>()) = 0;
			virtual void Recompile() = 0;

		private:
			static ShaderFactory* _Instance;
		};
	}
}

#endif
