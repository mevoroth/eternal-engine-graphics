#ifndef _D3D11_INPUT_LAYOUT_HPP_
#define _D3D11_INPUT_LAYOUT_HPP_

#include <d3d11.h>
#include <vector>

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11InputLayout
		{
			friend class D3D11VertexShader;
		public:
			enum VertexDataType
			{
				POSITION_T	= 0x1,
				COLOR_T		= 0x2,
				NORMAL_T	= 0x4,
				TANGENT_T	= 0x8,
				TEXCOORD_T	= 0x10,

				VERTEX_DATA_TYPE_COUNT = 5
			};
		private:
			ID3D11ClassLinkage* _classLinkage;
			ID3D11InputLayout* _inputLayout;
			vector<D3D11_INPUT_ELEMENT_DESC> _input;
			void _AddInputDesc(_In_ const char* semantic, _In_ const DXGI_FORMAT& format);
		public:
			D3D11InputLayout(_In_ const VertexDataType& dataType);
			ID3D11InputLayout* GetD3D11InputLayout() const;
		};
	}
}

#endif
