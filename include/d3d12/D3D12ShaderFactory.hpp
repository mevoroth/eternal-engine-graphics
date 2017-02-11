#ifndef _D3D12_SHADER_FACTORY_HPP_
#define _D3D12_SHADER_FACTORY_HPP_

#include <vector>
#include "Graphics/ShaderFactory.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class D3D12ShaderFactory : public ShaderFactory
		{
		public:
			virtual void RegisterShaderPath(const string& Path) override;
			virtual Shader* CreateVertexShader(_In_ const string& Name, _In_ const string& Src, _In_ const InputLayout::VertexDataType DataType[], _In_ uint32_t Size, _In_ const vector<string>& Defines = vector<string>()) override;
			virtual Shader* CreateVertexShader(_In_ const string& Name, _In_ const string& Src, _In_ const vector<string>& Defines = vector<string>()) override;
			virtual Shader* CreateGeometryShader(_In_ const string& Name, _In_ const string& Src, _In_ const vector<string>& Defines = vector<string>()) override;
			virtual Shader* CreatePixelShader(_In_ const string& Name, _In_ const string& Src, _In_ const vector<string>& Defines = vector<string>()) override;
			virtual vector<string>& GetShaderPaths() override { *((long*)2) = 2; return vector<string>(); }
			virtual void Recompile() override { *((long*)2) = 2; }

		private:
			vector<Shader*> _Shaders;

			Shader* _Find(_In_ const string& Key);
			Shader* _FindOrCreate(_In_ const string& Key, _In_ const string& Src, _In_ const string& EntryPoint, _In_ const string& ShaderModel);


		};
	}
}

#endif
