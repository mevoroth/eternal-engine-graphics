#include "d3d12_deprecated/D3D12ShaderFactory.hpp"

#include "File/FilePath.hpp"
#include "d3d12_deprecated/D3D12Shader.hpp"

using namespace Eternal::Graphics;
using namespace Eternal::File;

void D3D12ShaderFactory::RegisterShaderPath(const string& Path)
{
	FilePath::Register(Path, FileType::SHADERS);
}

Shader* D3D12ShaderFactory::CreateVertexShader(_In_ const string& Name, _In_ const string& Src, _In_ const InputLayout::VertexDataType DataType[], _In_ uint32_t Size, _In_ const vector<string>& Defines /*= vector<string>()*/)
{
	ETERNAL_BREAK();
	return nullptr;
}

Shader* D3D12ShaderFactory::CreateVertexShader(_In_ const string& Name, _In_ const string& Src, _In_ const vector<string>& Defines /*= vector<string>()*/)
{
	return _FindOrCreate(Name + "VS", Src, "VS", "vs_5_0");
}

Shader* D3D12ShaderFactory::CreatePixelShader(_In_ const string& Name, _In_ const string& Src, _In_ const vector<string>& Defines /*= vector<string>()*/)
{
	return _FindOrCreate(Name + "PS", Src, "PS", "ps_5_0");
}

Shader* D3D12ShaderFactory::CreateGeometryShader(_In_ const string& Name, _In_ const string& Src, _In_ const vector<string>& Defines /*= vector<string>()*/)
{
	return _FindOrCreate(Name + "GS", Src, "GS", "gs_5_0");
}

Shader* D3D12ShaderFactory::_FindOrCreate(_In_ const string& Key, _In_ const string& Src, _In_ const string& EntryPoint, _In_ const string& ShaderModel)
{
	Shader* ShaderObj = _Find(Key);

	if (ShaderObj)
		return ShaderObj;

	ShaderObj = new D3D12Shader(Key, Src, EntryPoint, ShaderModel);
	_Shaders.push_back(ShaderObj);
	return ShaderObj;
}

Shader* D3D12ShaderFactory::_Find(const string& Key)
{
	for (int ShaderIndex = 0; ShaderIndex < _Shaders.size(); ++ShaderIndex)
	{
		if (_Shaders[ShaderIndex]->GetName() == Key)
			return _Shaders[ShaderIndex];
	}

	return nullptr;
}
