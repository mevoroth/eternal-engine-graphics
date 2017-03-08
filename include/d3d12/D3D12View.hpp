#ifndef _D3D12_VIEW_HPP_
#define _D3D12_VIEW_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class D3D12View
		{
		public:
			inline D3D12_CPU_DESCRIPTOR_HANDLE& GetCpuDescriptor() { return _CpuDescriptor; }

		private:
			D3D12_CPU_DESCRIPTOR_HANDLE _CpuDescriptor;
		};
	}
}

#endif
