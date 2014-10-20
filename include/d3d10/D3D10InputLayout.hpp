#ifndef _D3D10_INPUT_LAYOUT_HPP_
#define _D3D10_INPUT_LAYOUT_HPP_

#include <d3d10.h>
#include <vector>

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class D3D10InputLayout
		{
			friend class D3D10VertexShader;
		public:
			enum VertexDataType
			{
				POSITION_T = 0x1,
				COLOR_T = 0x2,
				NORMAL_T = 0x4,
				TANGENT_T = 0x8,
				TEXCOORD_T = 0x10,

				VERTEX_DATA_TYPE_COUNT = 5
			};
		private:
			ID3D10InputLayout* _inputLayout;
			vector<D3D10_INPUT_ELEMENT_DESC> _input;
			void _AddInputDesc(_In_ const char* semantic, _In_ const DXGI_FORMAT& format);
		public:
			D3D10InputLayout(_In_ const VertexDataType& dataType);
			ID3D10InputLayout* GetD3D10InputLayout() const;
		};
	}
}

#endif
