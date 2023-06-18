#include "Graphics/ShaderFactory.hpp"

#include "Graphics/ShaderType.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "d3d12/D3D12Shader.hpp"
#include "Vulkan/VulkanShader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		ShaderFactory::ShaderFactory()
		{
		}

		ShaderFactory::~ShaderFactory()
		{
			for (uint32_t ShaderIndex = 0; ShaderIndex < _Shaders.size(); ++ShaderIndex)
			{
				delete _Shaders[ShaderIndex];
				_Shaders[ShaderIndex] = nullptr;
			}
			_Shaders.clear();
		}

		Shader* ShaderFactory::GetShader(_In_ GraphicsContext& InContext, _In_ const ShaderCreateInformation& InCreateInformation)
		{
			Shader* ShaderBinary = _Find(InCreateInformation.Name);
			if (ShaderBinary)
				return ShaderBinary;

			ShaderBinary = _Create(InContext, InCreateInformation);
			ETERNAL_ASSERT(ShaderBinary);
			_Shaders.push_back(ShaderBinary);
			return ShaderBinary;
		}

		Shader* ShaderFactory::_Find(_In_ const string& Name)
		{
			for (uint32_t ShaderIndex = 0; ShaderIndex < _Shaders.size(); ++ShaderIndex)
			{
				if (_Shaders[ShaderIndex]->GetName() == Name)
					return _Shaders[ShaderIndex];
			}
			return nullptr;
		}

		void ShaderFactory::Create(_In_ GraphicsContext& InContext, _In_ const ShaderCreateInformation& InCreateInformation, _In_ Shader* InShader)
		{
			_Create(InContext, InCreateInformation, InShader);
		}

		Shader* ShaderFactory::_Create(_In_ GraphicsContext& InContext, _In_ const ShaderCreateInformation& InCreateInformation, _In_ Shader* InShader)
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				if (InShader)
					return new (InShader) D3D12Shader(InContext, InCreateInformation);
				return new D3D12Shader(InContext, InCreateInformation);
#endif
#ifdef ETERNAL_ENABLE_VULKAN
			case DeviceType::VULKAN:
				if (InShader)
					return new (InShader) VulkanShader(InContext, InCreateInformation);
				return new VulkanShader(InContext, InCreateInformation);
#endif
			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
