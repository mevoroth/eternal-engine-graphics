#ifndef _D3D12_INPUT_LAYOUT_HPP_
#define _D3D12_INPUT_LAYOUT_HPP_

#include <cstdint>
#include <vector>
#include <d3d12.h>

#include "Graphics/InputLayout.hpp"

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class D3D12InputLayout : public InputLayout
		{
		public:
			D3D12InputLayout(_In_ const VertexDataType DataType[], _In_ uint32_t Size);

			const vector<D3D12_INPUT_ELEMENT_DESC>& GetInputElements() const { return _Input; }

		private:
			void _AddInputDesc(_In_ const char* Semantic, _In_ const DXGI_FORMAT& Format);

			vector<D3D12_INPUT_ELEMENT_DESC> _Input;
		};
	}
}

#endif