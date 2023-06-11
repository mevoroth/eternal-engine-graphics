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

		class VulkanShader final : public Shader
		{
		public:
			VulkanShader(_Inout_ GraphicsContext& InOutContext, const ShaderCreateInformation& InCreateInformation);
			vk::ShaderModule& GetVulkanShader();

		private:
			VulkanShader(_Inout_ GraphicsContext& Context, _In_ const string& InName, _In_ const string& InSource, _In_ const ShaderType& InStage, _In_ const vector<string>& InDefines = vector<string>());

			void _CompileFile(_In_ Device& InDevice, _In_ const string& InSource, _In_ const ShaderType& InStage, _In_ const vector<string>& InDefines);

			vk::ShaderModule _ShaderModule;
		};
	}
}
