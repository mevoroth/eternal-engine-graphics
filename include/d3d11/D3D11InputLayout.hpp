#ifndef _D3D11_INPUT_LAYOUT_HPP_
#define _D3D11_INPUT_LAYOUT_HPP_

#include "Graphics/InputLayout.hpp"

#include <cstdint>
#include <d3d11.h>
#include <vector>

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11InputLayout : public InputLayout
		{
			friend class D3D11VertexShader;
		public:
			D3D11InputLayout(_In_ const VertexDataType DataType[], _In_ uint32_t Size);
			D3D11InputLayout();
			ID3D11InputLayout* GetD3D11InputLayout() const;

		private:
			ID3D11InputLayout* _InputLayout;
			vector<D3D11_INPUT_ELEMENT_DESC> _Input;
			void _AddInputDesc(_In_ const char* Semantic, _In_ const DXGI_FORMAT& Format);
		};
	}
}

#endif
