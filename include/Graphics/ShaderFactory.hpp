#ifndef _SHADER_FACTORY_HPP_
#define _SHADER_FACTORY_HPP_

#include <cstdint>
#include <vector>

#include "Graphics/InputLayout.hpp"

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

			virtual Shader* CreateVertexShader(_In_ const string& Name, _In_ const string& Src, _In_ const InputLayout::VertexDataType DataType[], _In_ uint32_t Size) = 0;
			virtual Shader* CreateGeometryShader(_In_ const string& Name, _In_ const string& Src) = 0;
			virtual Shader* CreatePixelShader(_In_ const string& Name, _In_ const string& Src) = 0;

		private:
			static ShaderFactory* _Inst;
		};
	}
}

#endif
