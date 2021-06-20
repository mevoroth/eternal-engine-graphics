#ifndef _D3D11_STRUCTURED_BUFFER_HPP_
#define _D3D11_STRUCTURED_BUFFER_HPP_

#include "Graphics_deprecated/StructuredBuffer.hpp"
#include "d3d11/D3D11Buffer.hpp"
#include "d3d11/ID3D11ShaderResource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11StructuredBuffer
			: public StructuredBuffer
			, public D3D11Resource
			, public ID3D11ShaderResource
		{

		public:
			D3D11StructuredBuffer(_In_ const Usage& UsageObj, _In_ const CPUAccess& CPUMode, _In_ size_t Stride, _In_ uint32_t ElementsCount, _In_ const void* Data = nullptr);

			virtual Resource* GetAsResource() override;
			inline virtual ID3D11ShaderResourceView* GetD3D11ShaderResourceView() override
			{
				return _ShaderResourceView;
			}

		private:
			ID3D11ShaderResourceView* _ShaderResourceView = nullptr;
		};
	}
}

#endif
