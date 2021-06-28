#pragma once

#include <d3d12.h>
#include <vector>
#include "Graphics/InputLayout.hpp"

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		struct D3D12Format;

		class D3D12InputLayout : public InputLayout
		{
		public:
			D3D12InputLayout(_In_ const VertexDataType DataType[], _In_ uint32_t DataTypeCount);

			const vector<D3D12_INPUT_ELEMENT_DESC>& GetInputElements() const { return _Input; }

		private:
			vector<D3D12_INPUT_ELEMENT_DESC> _Input;
		};

		const D3D12Format& ConvertVertexDataTypeToD3D12Format(const VertexDataType& InVertexDataType);
	}
}
