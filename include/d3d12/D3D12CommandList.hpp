#ifndef _D3D12_COMMAND_LIST_HPP_
#define _D3D12_COMMAND_LIST_HPP_

struct ID3D12CommandList;

namespace Eternal
{
	namespace Graphics
	{
		class D3D12Context;
		class D3D12CommandList
		{
		public:
			D3D12CommandList(D3D12Context* ContextObj);
		private:
			ID3D12CommandList* _CommandList = nullptr;
		};
	}
}

#endif
