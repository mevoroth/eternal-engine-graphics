#ifndef _RESOURCE_FACTORY_HPP_
#define _RESOURCE_FACTORY_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		enum BufferType;
		enum TextureType;
		enum Format;
		enum ResourceDimension;
		enum TransitionState;

		class Device;
		class Heap;
		class Resource;

		Resource* CreateResource(_In_ Device& DeviceObj, _In_ Heap& HeapObj, _In_ uint64_t Stride, _In_ uint64_t Size, _In_ const BufferType& Type);
		Resource* CreateResource(_In_ Device& DeviceObj, _In_ Heap& HeapObj, _In_ const ResourceDimension& Dimension, _In_ const Format& FormatObj, const TextureType& Type, _In_ uint32_t Width, _In_ uint32_t Height, _In_ uint32_t Depth, _In_ uint32_t MipCount, _In_ const TransitionState& InitialState);
	}
}

#endif
