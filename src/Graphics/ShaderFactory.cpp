#include "Graphics/ShaderFactory.hpp"

#include "Graphics/ShaderType.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Null/NullShader.hpp"
#include "Proxy/ProxyShader.hpp"
#include "d3d12/D3D12Shader.hpp"
#include "Vulkan/VulkanShader.hpp"
#if (ETERNAL_USE_PRIVATE || ETERNAL_USE_PROXY)
#include "Graphics/ShaderFactoryPrivate.hpp"
#endif

namespace Eternal
{
	namespace Graphics
	{
		extern Shader* CreateShaderPrivate(_In_ GraphicsContext& InContext);

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

		Shader* ShaderFactory::GetShader(_In_ GraphicsContext& InContext)
		{
			Shader* ShaderBinary = nullptr;
			switch (InContext.GetDevice().GetDeviceType())
			{
			case DeviceType::DEVICE_TYPE_NULL:
				ShaderBinary = new NullShader();

			case DeviceType::DEVICE_TYPE_PROXY:
				ShaderBinary = new ProxyShader();

#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
				ShaderBinary = new D3D12Shader();
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				ShaderBinary = new VulkanShader();
#endif
			default:
#if (ETERNAL_USE_PRIVATE || ETERNAL_USE_PROXY)
			{
				ShaderBinary = CreateShaderPrivate(InContext);
			}
#endif
				break;
			}
			ETERNAL_ASSERT(ShaderBinary);
			_Shaders.push_back(ShaderBinary);
			return ShaderBinary;
		}

		Shader* ShaderFactory::GetShader(_In_ GraphicsContext& InContext, _In_ const ShaderCreateInformation& InCreateInformation)
		{
			return GetShader(InContext.GetDevice().GetDeviceType(), InContext, InCreateInformation);
		}

		Shader* ShaderFactory::GetShader(_In_ const DeviceType& InDeviceType, _In_ GraphicsContext& InContext, _In_ const ShaderCreateInformation& InCreateInformation)
		{
			Shader* ShaderBinary = _Find(InCreateInformation.Name);
			if (ShaderBinary)
				return ShaderBinary;

			ShaderBinary = _Create(InDeviceType, InContext, InCreateInformation);
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
			return _Create(InContext.GetDevice().GetDeviceType(), InContext, InCreateInformation, InShader);
		}

		Shader* ShaderFactory::_Create(_In_ const DeviceType& InDeviceType, _In_ GraphicsContext& InContext, _In_ const ShaderCreateInformation& InCreateInformation, _In_ Shader* InShader)
		{
			switch (InDeviceType)
			{
			case DeviceType::DEVICE_TYPE_NULL:
				if (InShader)
					return new (InShader) NullShader(InContext, InCreateInformation);
				return new NullShader(InContext, InCreateInformation);

			case DeviceType::DEVICE_TYPE_PROXY:
				if (InShader)
					return new (InShader) ProxyShader(InContext, InCreateInformation);
				return new ProxyShader(InContext, InCreateInformation);

#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
				if (InShader)
					return new (InShader) D3D12Shader(InContext, InCreateInformation);
				return new D3D12Shader(InContext, InCreateInformation);
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				if (InShader)
					return new (InShader) VulkanShader(InContext, InCreateInformation);
				return new VulkanShader(InContext, InCreateInformation);
#endif
			default:
#if (ETERNAL_USE_PRIVATE || ETERNAL_USE_PROXY)
				return CreateShaderPrivate(InDeviceType, InContext, InCreateInformation, InShader);
#endif
				break;
			}
			ETERNAL_BREAK();
			return nullptr;
		}
	}
}
