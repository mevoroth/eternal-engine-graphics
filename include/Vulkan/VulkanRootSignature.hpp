#pragma once

#include "Graphics/RootSignature.hpp"
#include "Graphics/ShaderType.hpp"
#include "Vulkan/VulkanHeader.hpp"
#include "Bit/BitField.hpp"
#include <vector>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		using namespace Eternal::Bit;

		class GraphicsContext;
		class Device;

		class VulkanRootSignature : public RootSignature
		{
		public:
			using RegisterIndicesContainer = std::array<uint32_t, static_cast<int32_t>(ShaderType::SHADER_TYPE_COUNT)>;

			VulkanRootSignature(_In_ Device& InDevice);
			VulkanRootSignature(_In_ GraphicsContext& InContext, _In_ const RootSignatureCreateInformation& InRootSignatureCreateInformation);
			virtual ~VulkanRootSignature();

			inline vk::PipelineLayout& GetPipelineLayout() { return _PipelineLayout; }

		private:
			void _CreateDescriptorSetLayout(_In_ const RootSignatureDescriptorTable& InDescriptorTable, _Inout_ RegisterIndicesContainer& InOutRegisterIndices);

			Device&							_Device;
			vk::PipelineLayout				_PipelineLayout;
			vector<vk::DescriptorSetLayout>	_DescriptorSetLayouts;
			vector<Handle>					_ConstantHandles;
		};
	}
}
