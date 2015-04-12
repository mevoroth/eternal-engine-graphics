#ifndef _D3D11_MATERIAL_HPP_
#define _D3D11_MATERIAL_HPP_

#include "Graphics/Material.hpp"
#include "d3d11/D3D11InputLayout.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Material : public Material
		{
		public:
			D3D11Material();
			virtual void AttachVertexShader(_Inout_ Shader* Shader) override;
			virtual void AttachGeometryShader(_Inout_ Shader* Shader) override;
			virtual void AttachPixelShader(_Inout_ Shader* Shader) override;
			virtual void SetMaterialDesc(_In_ const MaterialProperty& MatProperty) override;
			void AttachInputLayout(_In_ D3D11InputLayout* InputLayout);
			virtual void Apply() override;
			void Unbind();

			virtual void SetFloat(_In_ const string& Name, _In_ float Value) override;
			virtual void SetVector4(_In_ const string& Name, _In_ const Vector4& Value) override;
			virtual void SetTexture(_In_ const string& Name, _In_ Texture* Value) override;

		private:
			static ID3D11ShaderResourceView* SHADER_RESOURCE_VIEWS[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
			vector<MaterialProperty> _MatInput;
			vector<string> _TexturesInput;
			D3D11InputLayout* _InputLayout = 0;
			ID3D11ClassLinkage* _DynamicParams = 0;
			ID3D11VertexShader* _Vertex = 0;
			ID3D11GeometryShader* _Geometry = 0;
			ID3D11PixelShader* _Pixel = 0;
			bool _Applied = false;
			uint8_t _Buffer[1024];
			//ID3DBlob* _vertexSrc;
			//ID3DBlob* _geometrySrc;
			//ID3DBlob* _PixelSrc;

			//inline DXGI_FORMAT _GetD3DParam(_In_ const ParamType& paramType) const;
		};
	}
}

#endif
