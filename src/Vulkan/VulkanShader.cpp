#include "Vulkan/VulkanShader.hpp"

#include "Macros/Macros.hpp"
#include <cstdint>
#include <fstream>
#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"
#include "File/FilePath.hpp"

using namespace Eternal::Graphics;
using namespace Eternal::File;

VulkanShader::VulkanShader(_In_ VulkanDevice& Device, _In_ const string& Name, _In_ const string& Src)
{
	string FullPathSrc = FilePath::Find(Src, FilePath::SHADERS);

	ifstream ShaderFile(FullPathSrc.c_str(), ios::in | ios::binary | ios::ate);

	if (!ShaderFile)
	{
		ETERNAL_ASSERT(false);
	}

	uint32_t FileSize = (uint32_t)ShaderFile.tellg();
	_ShaderCode.resize(FileSize);
	ShaderFile.seekg(0, ios::beg);
	ShaderFile.read(_ShaderCode.data(), FileSize);
	ShaderFile.close();

	VkShaderModuleCreateInfo ShaderModuleInfo;
	ShaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	ShaderModuleInfo.pNext = nullptr;
	ShaderModuleInfo.pCode = (const uint32_t*)_ShaderCode.data();
	ShaderModuleInfo.codeSize = _ShaderCode.size();
	ShaderModuleInfo.flags = 0;

	VkResult Result = vkCreateShaderModule(Device.GetDevice(), &ShaderModuleInfo, nullptr, &_ShaderModule);
	ETERNAL_ASSERT(!Result);
}
