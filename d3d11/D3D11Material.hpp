#ifndef _D3D11_MATERIAL_HPP_
#define _D3D11_MATERIAL_HPP_

#include "Graphics\Material.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Material : public Material
		{
		public:
			D3D11Material();
			void SetVertexShader(_In_ const string& shader);
			void SetGeometryShader(_In_ const string& shader);
			void SetPixelShader(_In_ const string& shader);
			void SetMaterialDesc(_In_ const string& paramName, _In_ const ParamType& paramType);

		private:
			vector<D3D11_INPUT_ELEMENT_DESC> _matInput;
			ID3D11VertexShader* _vertex;
			ID3D11GeometryShader* _geometry;
			ID3D11PixelShader* _pixel;
			ID3D11ClassLinkage* _dynamicParams;
			
			inline DXGI_FORMAT _GetD3DParam(_In_ const ParamType& paramType) const;
		};
	}
}

#endif
