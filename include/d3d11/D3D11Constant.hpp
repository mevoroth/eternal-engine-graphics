#ifndef _D3D11_CONSTANT_HPP_
#define _D3D11_CONSTANT_HPP_

#include "Graphics/Constant.hpp"
#include "d3d11/D3D11Buffer.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Constant
			: public Constant
			, public D3D11Buffer
		{
		public:
			D3D11Constant(_In_ size_t BufferSize, _In_ const Usage& UsageObj, _In_ const CPUAccess& CPUMode);
			D3D11Constant(_In_ size_t BufferSize, _In_ const Usage& UsageObj, _In_ const CPUAccess& CPUMode, _In_ void* Data);

			virtual Resource* GetAsResource() override;
		};
	}
}

#endif
