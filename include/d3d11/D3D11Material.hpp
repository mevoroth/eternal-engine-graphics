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
			void AttachVertexShader(_Inout_ Shader* shader);
			void AttachGeometryShader(_Inout_ Shader* shader);
			void AttachPixelShader(_Inout_ Shader* shader);
			void SetMaterialDesc(_In_ const MaterialProperty& matProperty);
			void AttachInputLayout(_In_ D3D11InputLayout* inputLayout);
			void Apply();

		private:
			vector<MaterialProperty> _matInput;
			//Shader* _vertex;
			//Shader* _geometry;
			//Shader* _pixel;
			D3D11InputLayout* _inputLayout;
			ID3D11ClassLinkage* _dynamicParams;
			ID3D11VertexShader* _vertex;
			ID3D11GeometryShader* _geometry;
			ID3D11PixelShader* _pixel;
			//ID3DBlob* _vertexSrc;
			//ID3DBlob* _geometrySrc;
			//ID3DBlob* _PixelSrc;

			//inline DXGI_FORMAT _GetD3DParam(_In_ const ParamType& paramType) const;
		};
	}
}

#endif
