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

		class Device;
		enum ShaderType;

		class VulkanShader
		{
		public:
			VulkanShader(_In_ Device& DeviceObj, _In_ const string& Name, _In_ const string& Src, const ShaderType& Type);
			VkShaderModule_T* GetVulkanShader();

		private:
			void _CompileFile(_In_ Device& DeviceObj, _In_ const string& Src, const ShaderType& Type);

			//std::vector<char> _ShaderCode;
			VkShaderModule_T* _ShaderModule = nullptr;
		};
	}
}

#endif
