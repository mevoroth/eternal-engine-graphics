#ifndef _D3D11_RESOURCE_HPP_
#define _D3D11_RESOURCE_HPP_

#include "Graphics/Resource.hpp"

struct ID3D11ShaderResourceView;
struct ID3D11Buffer;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Resource : public Resource
		{
		public:
			virtual ID3D11ShaderResourceView* GetD3D11ShaderResourceView() = 0;
			virtual ID3D11Buffer* GetD3D11Buffer() = 0;
#pragma region Resource
			virtual void* Lock(const LockMode& LockingMode) override;
			virtual void Unlock() override;
#pragma endregion Resource
		};
	}
}

#endif
