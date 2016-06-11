#ifndef _D3D11_SHADER_FACTORY_HPP_
#define _D3D11_SHADER_FACTORY_HPP_

#include <vector>

#include "Graphics/ShaderFactory.hpp"
#include "d3d11/D3D11VertexShader.hpp"
#include "d3d11/D3D11GeometryShader.hpp"
#include "d3d11/D3D11PixelShader.hpp"

struct ID3D11ClassLinkage;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11ShaderFactory : public ShaderFactory
		{
		public:
			D3D11ShaderFactory();
			~D3D11ShaderFactory();

			virtual void RegisterShaderPath(const string& Path) override;
			virtual Shader* CreateVertexShader(_In_ const string& Name, _In_ const string& Src, _In_ const InputLayout::VertexDataType DataType[], _In_ uint32_t Size) override;
			virtual Shader* CreateVertexShader(_In_ const string& Name, _In_ const string& Src) override;
			virtual Shader* CreateGeometryShader(_In_ const string& Name, _In_ const string& Src) override;
			virtual Shader* CreatePixelShader(_In_ const string& Name, _In_ const string& Src) override;

		private:
			ID3D11ClassLinkage* _ClassLinkage = nullptr;

			vector<D3D11VertexShader*> _VertexShaders;
			vector<D3D11GeometryShader*> _GeometryShaders;
			vector<D3D11PixelShader*> _PixelShaders;

			template<class ShaderT>
			ShaderT* _Find(const vector<ShaderT*>& Container, const string& Name) const
			{
				for (uint32_t ShaderIndex = 0; ShaderIndex < Container.size(); ++ShaderIndex)
				{
					if (Container[ShaderIndex]->GetName() == Name)
					{
						return Container[ShaderIndex];
					}
				}
				return nullptr;
			}

			template<class ShaderT>
			void _Clear(vector<ShaderT*>& Container)
			{
				for (uint32_t ShaderIndex = 0; ShaderIndex < Container.size(); ++ShaderIndex)
				{
					delete Container[ShaderIndex];
				}
				Container.clear();
			}
		};
	}
}

#endif
