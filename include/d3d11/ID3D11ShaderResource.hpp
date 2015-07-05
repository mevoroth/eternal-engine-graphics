#ifndef _I_D3D11_SHADER_RESOURCE_HPP_
#define _I_D3D11_SHADER_RESOURCE_HPP_

struct ID3D11ShaderResourceView;

namespace Eternal
{
	namespace Graphics
	{
		class ID3D11ShaderResource
		{
		public:
			virtual ID3D11ShaderResourceView* GetD3D11ShaderResourceView() = 0;
		};
	}
}

#endif
