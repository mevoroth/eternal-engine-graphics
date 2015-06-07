#ifndef _D3D11_UINT32_INDEX_BUFFER_HPP_
#define _D3D11_UINT32_INDEX_BUFFER_HPP_

#include "d3d11/D3D11IndexBuffer.hpp"

#include <vector>

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11UInt32IndexBuffer : public D3D11IndexBuffer
		{
		public:
			D3D11UInt32IndexBuffer(vector<uint32_t>& Indices);

			virtual ID3D11ShaderResourceView* GetD3D11ShaderResourceView()
			{
				return nullptr;
			}

			virtual ID3D11Buffer* GetD3D11Buffer()
			{
				return nullptr;
			}

			virtual uint32_t GetCount() const override;

		private:
			vector<uint32_t>& _Indices;
		};
	}
}

#endif
