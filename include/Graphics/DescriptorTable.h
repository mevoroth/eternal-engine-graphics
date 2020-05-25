#ifndef _DESCRIPTOR_TABLE_HPP_
#define _DESCRIPTOR_TABLE_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		class View;
		class Sampler;
		class Device;

		class DescriptorTable
		{
		public:
			virtual ~DescriptorTable() {}

			virtual void SetResource(_In_ uint32_t Slot, _In_ View* ViewObj) {}
			virtual void SetResource(_In_ uint32_t Slot, _In_ Sampler* SamplerObj) {}
			virtual void Update(_In_ Device& DeviceObj) {}
		};
	}
}

#endif
