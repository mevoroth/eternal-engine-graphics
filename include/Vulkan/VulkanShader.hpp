#ifndef _VULKAN_SHADER_HPP_
#define _VULKAN_SHADER_HPP_

#include <string>
#include <vector>
#include "Graphics/Shader.hpp"

struct VkShaderModule_T;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		enum ShaderType;

		class VulkanShader : public Shader
		{
		public:
			VulkanShader(_In_ Device& DeviceObj, _In_ const string& Name, _In_ const string& Source, _In_ const ShaderType& Type, _In_ const vector<string>& Defines = vector<string>());
			VkShaderModule_T* GetVulkanShader();

		private:
			void _CompileFile(_In_ Device& DeviceObj, _In_ const string& Src, _In_ const ShaderType& Type, _In_ const vector<string>& Defines);

			//std::vector<char> _ShaderCode;
			VkShaderModule_T* _ShaderModule = nullptr;
		};
	}
}

#endif
