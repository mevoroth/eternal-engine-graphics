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

		Shader* ShaderFactory::GetShader(_In_ GraphicsContext& Context, _In_ const ShaderCreateInformation& CreateInformation)
		{
			Shader* ShaderBinary = _Find(CreateInformation.Name);
			if (ShaderBinary)
				return ShaderBinary;

			ShaderBinary = _Create(Context, CreateInformation);
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

		Shader* ShaderFactory::_Create(_In_ GraphicsContext& Context, _In_ const ShaderCreateInformation& CreateInformation)
		{
			switch (Context.GetDevice().GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12Shader(Context, CreateInformation);
#endif
#ifdef ETERNAL_ENABLE_VULKAN
			case DeviceType::VULKAN:
				return new VulkanShader(Context, CreateInformation);
#endif
			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}
	}
}
