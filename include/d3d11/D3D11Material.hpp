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
			void AttachVertexShader(_Inout_ Shader* Shader);
			void AttachGeometryShader(_Inout_ Shader* Shader);
			void AttachPixelShader(_Inout_ Shader* Shader);
			void SetMaterialDesc(_In_ const MaterialProperty& MatProperty);
			void AttachInputLayout(_In_ D3D11InputLayout* InputLayout);
			void Apply();
			void Unbind();

		private:
			vector<MaterialProperty> _MatInput;
			D3D11InputLayout* _InputLayout = 0;
			ID3D11ClassLinkage* _DynamicParams = 0;
			ID3D11VertexShader* _Vertex = 0;
			ID3D11GeometryShader* _Geometry = 0;
			ID3D11PixelShader* _Pixel = 0;
			bool _Applied = false;
			//ID3DBlob* _vertexSrc;
			//ID3DBlob* _geometrySrc;
			//ID3DBlob* _PixelSrc;

			//inline DXGI_FORMAT _GetD3DParam(_In_ const ParamType& paramType) const;
		};
	}
}

#endif
