#pragma once

#include "Graphics/Shader.hpp"

#if ETERNAL_ENABLE_VULKAN

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
			VulkanShader();
			VulkanShader(_Inout_ GraphicsContext& InOutContext, const ShaderCreateInformation& InCreateInformation);

			virtual bool IsShaderCompiled() const override final;
			virtual void SerializeShader(_Inout_ File* InOutFile) override final;

			vk::ShaderModule& GetVulkanShader();

		private:

			void _CompileFile(_Inout_ GraphicsContext& InOutContext, _In_ const string& InSource, _In_ const ShaderType& InStage, _In_ const vector<string>& InDefines);

			vk::ShaderModule _ShaderModule;
		};
	}
}

#endif
