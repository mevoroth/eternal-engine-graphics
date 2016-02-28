#ifndef _D3D12_STATE_HPP_
#define _D3D12_STATE_HPP_

class Context;
struct ID3D12PipelineState;

namespace Eternal
{
	namespace Graphics
	{
		class D3D12State
		{
		public:
			D3D12State();

		private:
			ID3D12PipelineState* _PipelineState = nullptr;
		};
	}
}

#endif
