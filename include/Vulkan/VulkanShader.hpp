#pragma once

#include <string>
#include <vulkan/vulkan.hpp>
#include "Graphics/Shader.hpp"

struct VkShaderModule_T;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class GraphicsContext;
		enum class ShaderType;

		class VulkanShader : public Shader
		{
		public:
			VulkanShader(_In_ Device& InDevice, _In_ const string& Name, _In_ const string& Source, _In_ const ShaderType& Type, _In_ const vector<string>& Defines = vector<string>());
			VulkanShader(_In_ GraphicsContext& Context, const ShaderCreateInformation& CreateInformation);
			vk::ShaderModule& GetVulkanShader();

		private:
			void _CompileFile(_In_ Device& InDevice, _In_ const string& Src, _In_ const ShaderType& Type, _In_ const vector<string>& Defines);

			vk::ShaderModule _ShaderModule;
		};
	}
}
