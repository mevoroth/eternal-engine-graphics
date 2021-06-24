#pragma once

#include "Graphics/Shader.hpp"
#include "Vulkan/VulkanHeader.hpp"
#include <string>

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
			VulkanShader(_In_ GraphicsContext& Context, const ShaderCreateInformation& CreateInformation);
			vk::ShaderModule& GetVulkanShader();

		private:
			VulkanShader(_In_ Device& InDevice, _In_ const string& Name, _In_ const string& Source, _In_ const ShaderType& Type, _In_ const vector<string>& Defines = vector<string>());

			void _CompileFile(_In_ Device& InDevice, _In_ const string& Src, _In_ const ShaderType& Type, _In_ const vector<string>& Defines);

			vk::ShaderModule _ShaderModule;
		};
	}
}
