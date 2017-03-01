#ifndef _VULKAN_SHADER_HPP_
#define _VULKAN_SHADER_HPP_

#include <string>
#include <vector>

struct VkShaderModule_T;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class VulkanDevice;

		class VulkanShader
		{
		public:
			VulkanShader(_In_ VulkanDevice& Device, _In_ const string& Name, _In_ const string& Src);

		private:
			std::vector<char> _ShaderCode;
			VkShaderModule_T* _ShaderModule = nullptr;
		};
	}
}

#endif
