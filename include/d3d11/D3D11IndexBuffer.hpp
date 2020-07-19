#ifndef _D3D11_INDEX_BUFFER_HPP_
#define _D3D11_INDEX_BUFFER_HPP_

#include <d3d11.h>
#include "Graphics/IndexBuffer.hpp"
#include "d3d11/D3D11Buffer.hpp"

enum DXGI_FORMAT;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		template<typename IndexT>
		class D3D11IndexBuffer
			: public D3D11Buffer
			, public IndexBuffer
		{
		public:
			D3D11IndexBuffer(_In_ vector<IndexT>& Indices)
				: _Indices(Indices)
				, D3D11Buffer(Indices.size() * sizeof(IndexT), DYNAMIC, WRITE, INDEX_BUFFER, &Indices[0])
			{
			}
			DXGI_FORMAT GetD3D11Format() const
			{
				return DXGI_FORMAT_R32_UINT; // DIRTY: DXGI_FORMAT_R16_UINT is better
			}

			virtual uint32_t GetCount() const override
			{
				return _Indices.size();
			}

			virtual void* GetNative() override
			{
				return GetD3D11Buffer();
			}

			virtual int GetNativeFormat() override
			{
				return GetD3D11Format();
			}

		private:
			D3D11IndexBuffer(size_t BufferSize, void* Data)
				: D3D11Buffer(BufferSize, DYNAMIC, WRITE, INDEX_BUFFER, Data)
			{
			}

			vector<IndexT>& _Indices;
		};
	}
}

#endif
