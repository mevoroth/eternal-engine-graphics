#ifndef _D3D12_DESCRIPTOR_TABLE_HPP_
#define _D3D12_DESCRIPTOR_TABLE_HPP_

#include "Graphics/DescriptorTable.h"
#include "d3d12_deprecated/D3D12View.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D12DescriptorTable : public DescriptorTable
		{
		public:
			D3D12DescriptorTable(_In_ DescriptorHeap& DescriptorHeapObj);

			DescriptorHeapHandle& GetHandle() { return _Handle; }
			//virtual void SetResource(_In_ uint32_t Slot, _In_ View* ViewObj) override;

		private:
			DescriptorHeapHandle	_Handle;
		};
	}
}

#endif
