#include "Graphics/ShaderFactory.hpp"

#include "Graphics/ShaderType.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "d3d12/D3D12Shader.hpp"
#include "Vulkan/VulkanShader.hpp"

using namespace Eternal::Graphics;

ShaderFactory* ShaderFactory::_Instance = nullptr;

ShaderFactory* ShaderFactory::Get()
{
	ETERNAL_ASSERT(_Instance);
	return _Instance;
}

ShaderFactory::ShaderFactory()
{
	ETERNAL_ASSERT(!_Instance);
	_Instance = this;
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

Shader* ShaderFactory::GetShader(_In_ Device& DeviceObj, _In_ const string& Name, _In_ const string& Source, _In_ const ShaderType& Type, _In_ const vector<string>& Defines /* = vector<string>() */)
{
	Shader* ShaderObj = _Find(Name);
	if (ShaderObj)
		return ShaderObj;

	ShaderObj = _Create(DeviceObj, Name, Source, Type, Defines);
	ETERNAL_ASSERT(ShaderObj);
	_Shaders.push_back(ShaderObj);
	return ShaderObj;
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

Shader* ShaderFactory::_Create(_In_ Device& InDevice, _In_ const string& Name, _In_ const string& Source, _In_ const ShaderType& Type, _In_ const vector<string>& Defines)
{
	switch (InDevice.GetDeviceType())
	{
#ifdef ETERNAL_ENABLE_D3D12
	case DeviceType::D3D12:
		return new D3D12Shader(Name, Source, Type, Defines);
#endif

	case DeviceType::VULKAN:
		return new VulkanShader(InDevice, Name, Source, Type, Defines);

	default:
		ETERNAL_BREAK();
		return nullptr;
	}
}
