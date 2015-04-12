#include "d3d11/D3D11Material.hpp"

#include <cassert>
#include <numeric>

#include "d3d11/D3D11Renderer.hpp"
#include "d3d11/D3D11Shader.hpp"
#include "d3d11/D3D11VertexShader.hpp"
#include "d3d11/D3D11Texture.hpp"

using namespace std;
using namespace Eternal::Graphics;

D3D11Material::D3D11Material()
	: _DynamicParams(0)
	, _Vertex(0)
	, _Geometry(0)
	, _Pixel(0)
{
	dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateClassLinkage(&_DynamicParams);
}

void D3D11Material::SetMaterialDesc(_In_ const MaterialProperty& MatProperty)
{
	assert(MatProperty.Name.size() > 0);
	if (MatProperty.Type == MaterialProperty::TEXTURE_T)
	{
		_TexturesInput.push_back(MatProperty.Name);
	}
	else
	{
		_MatInput.push_back(MatProperty);
	}
}

void D3D11Material::AttachInputLayout(_In_ D3D11InputLayout* InputLayout)
{
	_InputLayout = InputLayout;
}

void D3D11Material::Apply()
{
	if (_Applied)
	{
		return;
	}

	ID3D11DeviceContext* Ctx = dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDeviceContext();
	Ctx->IASetInputLayout(_InputLayout->GetD3D11InputLayout());
	Ctx->VSSetShader(_Vertex, 0, 0);
	if (_Geometry)
	{
		Ctx->GSSetShader(_Geometry, 0, 0);
	}
	Ctx->PSSetShader(_Pixel, 0, 0);
	
	D3D11_BUFFER_DESC BufferDesc;
	uint32_t Size = 0;
	for (uint32_t InputIndice = 0; InputIndice < _MatInput.size(); ++InputIndice)
	{
		Size += MaterialProperty::GetSize(_MatInput[InputIndice].Type);
	}
	BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;

	//D3D11_SUBRESOURCE_DATA SubResData;
	//SubResData.pSysMem = 
	ID3D11Buffer* ConstantBuffer;

	dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDevice()->CreateBuffer(&BufferDesc, 0, &ConstantBuffer);

	uint32_t TextureCount = _TexturesInput.size();
	Ctx->VSSetShaderResources(0, TextureCount, 0);
	if (_Geometry)
	{
		Ctx->GSSetShaderResources(0, TextureCount, 0);
	}
	Ctx->PSSetShaderResources(0, TextureCount, 0);

	_Applied = true;
}

void D3D11Material::AttachVertexShader(_Inout_ Shader* Shader)
{
	if (_Vertex)
	{
		_Vertex->Release();
		_Vertex = 0;
	}
	((D3D11Shader*)Shader)->InstantiateShader(_DynamicParams, (void**)&_Vertex);
}
void D3D11Material::AttachGeometryShader(_Inout_ Shader* Shader)
{
	if (_Geometry)
	{
		_Geometry->Release();
		_Geometry = 0;
	}
	((D3D11Shader*)Shader)->InstantiateShader(_DynamicParams, (void**)&_Geometry);
}
void D3D11Material::AttachPixelShader(_Inout_ Shader* Shader)
{
	if (_Pixel)
	{
		_Pixel->Release();
		_Pixel = 0;
	}
	((D3D11Shader*)Shader)->InstantiateShader(_DynamicParams, (void**)&_Pixel);
}
void D3D11Material::Unbind()
{
	_Applied = false;
}

void Eternal::Graphics::D3D11Material::SetFloat(_In_ const string& Name, _In_ float Value)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Eternal::Graphics::D3D11Material::SetVector4(_In_ const string& Name, _In_ const Vector4& Value)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void Eternal::Graphics::D3D11Material::SetTexture(_In_ const string& Name, _In_ Texture* Value)
{
	ID3D11DeviceContext* Ctx = dynamic_cast<D3D11Renderer*>(Renderer::Get())->GetDeviceContext();

	ID3D11ShaderResourceView* ShaderResourceView = static_cast<D3D11Texture*>(Value)->CreateShaderResourceView();

	for (uint32_t TextureIndex = 0, TextureCount = _TexturesInput.size(); TextureIndex < TextureCount; ++TextureIndex)
	{
		if (Name == _TexturesInput[TextureIndex])
		{
			Ctx->VSSetShaderResources(TextureIndex, 1, 0);
			if (_Geometry)
			{
				Ctx->GSSetShaderResources(0, TextureCount, 0);
			}
			Ctx->PSSetShaderResources(0, TextureCount, 0);
		}
	}
}
