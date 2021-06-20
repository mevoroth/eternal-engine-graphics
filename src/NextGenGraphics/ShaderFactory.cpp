#include "NextGenGraphics/ShaderFactory.hpp"

#include "Graphics_deprecated/ShaderType.hpp"
#include "NextGenGraphics/Types/DeviceType.hpp"
#include "NextGenGraphics/Device.hpp"
#include "d3d12_deprecated/D3D12Shader.hpp"
#include "Vulkan_deprecated/VulkanShader.hpp"

using namespace Eternal::Graphics;

ShaderFactory* ShaderFactory::_Inst = nullptr;

ShaderFactory* ShaderFactory::Get()
{
	ETERNAL_ASSERT(_Inst);
	return _Inst;
}

ShaderFactory::ShaderFactory()
{
	ETERNAL_ASSERT(!_Inst);
	_Inst = this;
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

Shader* ShaderFactory::_Create(_In_ Device& DeviceObj, _In_ const string& Name, _In_ const string& Source, _In_ const ShaderType& Type, _In_ const vector<string>& Defines)
{
	switch (DeviceObj.GetDeviceType())
	{
#ifdef ETERNAL_ENABLE_D3D12
	case DeviceType::D3D12:
		return new D3D12Shader(Name, Source, Type, Defines);
#endif

	case DeviceType::VULKAN:
		return new VulkanShader(DeviceObj, Name, Source, Type, Defines);

	default:
		ETERNAL_BREAK();
		return nullptr;
	}
}
