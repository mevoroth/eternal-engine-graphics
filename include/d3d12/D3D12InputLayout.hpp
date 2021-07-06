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

		class D3D12InputLayout final : public InputLayout
		{
		public:
			D3D12InputLayout(_In_ const vector<VertexStreamBase>& InVertexStreams);

			const vector<D3D12_INPUT_ELEMENT_DESC>& GetD3D12InputElements() const { return _InputElements; }

		private:
			vector<D3D12_INPUT_ELEMENT_DESC> _InputElements;
		};

		const D3D12Format& ConvertVertexDataTypeToD3D12Format(const VertexDataType& InVertexDataType);
	}
}
