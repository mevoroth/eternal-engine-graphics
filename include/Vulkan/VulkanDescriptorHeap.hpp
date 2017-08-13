#ifndef _VULKAN_DESCRIPTOR_HPP_
#define _VULKAN_DESCRIPTOR_HPP_

#include <cstdint>
#include <vector>
#include "Graphics/DescriptorHeap.hpp"
#include "Container/RingBuffer.hpp"

struct VkDescriptorPool_T;
struct VkDescriptorSetLayout_T;
struct VkDescriptorSet_T;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		using namespace Eternal::Container;

		class Device;
		class RootSignature;
		enum RootSignatureParameterType;
		enum RootSignatureAccess;

		class VulkanDescriptorHeap : public DescriptorHeap
		{
		public:
			VulkanDescriptorHeap(_In_ Device& DeviceObj, _In_ const RootSignatureParameter Resources[], _In_ uint32_t ResourcesCount);
			//VulkanDescriptorHeap(_In_ Device& DeviceObj, _In_ uint32_t Space, _In_ uint32_t Register, _In_ const RootSignatureParameterType& HeapTypeObj, _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj);
			~VulkanDescriptorHeap();

			virtual View* CreateView(_In_ Device& DeviceObj) override;
			VkDescriptorSetLayout_T*& GetDescriptorSetLayout() { return _DescriptorSetLayout; }
			VkDescriptorSet_T* Pop();
			void Push(_In_ VkDescriptorSet_T* Handle);

			VkDescriptorSet_T* Bind();
			void Unbind();

		private:
			VkDescriptorSet_T*					_DescriptorSet	= nullptr;
			//vector<VkDescriptorSet_T*>		_ResourcesPool;
			//RingBuffer<VkDescriptorSet_T*>	_BoundResources;

			Device& _Device;
			VkDescriptorPool_T*			_DescriptorPool			= nullptr;
			VkDescriptorSetLayout_T*	_DescriptorSetLayout	= nullptr;
		};
	}
}

#endif
