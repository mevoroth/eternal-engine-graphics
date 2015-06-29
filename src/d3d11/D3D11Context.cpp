#include "d3d11/D3D11Context.hpp"

#include "d3d11/D3D11RenderTarget.hpp"
#include "d3d11/D3D11Shader.hpp"
#include "d3d11/D3D11Constant.hpp"
#include "Graphics/Viewport.hpp"
#include "Graphics/BlendState.hpp"

using namespace Eternal::Graphics;

D3D11Context::D3D11Context(ID3D11DeviceContext* D3D11ContextObj)
	: _DeviceContext(D3D11ContextObj)
{
}

void D3D11Context::DrawIndexed(_In_ VertexBuffer* VerticesBuffer, _In_ IndexBuffer* IndicesBuffer)
{
	uint32_t Stride = VerticesBuffer->GetSize();
	uint32_t Offset = 0;

	D3D11IndexBuffer* Indices = static_cast<D3D11IndexBuffer*>(IndicesBuffer);

	ID3D11Buffer* D3D11Buffer = static_cast<D3D11VertexBuffer*>(VerticesBuffer)->GetD3D11Buffer();
	_DeviceContext->IASetVertexBuffers(0, 1, &D3D11Buffer, &Stride, &Offset);
	_DeviceContext->IASetIndexBuffer(Indices->GetD3D11Buffer(), Indices->GetD3D11Format(), 0);

	_DeviceContext->DrawIndexed(IndicesBuffer->GetCount(), 0, 0);
}

void D3D11Context::SetRenderTargets(_In_ RenderTarget** RenderTargets, _In_ int RenderTargetsCount)
{
	ID3D11RenderTargetView* RenderTargetViews[D3D11_MAX_RENDERTARGETS];
	for (int RenderTargetIndex = 0; RenderTargetIndex < RenderTargetsCount; ++RenderTargetIndex)
	{
		if (RenderTargets[RenderTargetIndex])
		{
			RenderTargetViews[RenderTargetIndex] = static_cast<D3D11RenderTarget*>(RenderTargets[RenderTargetIndex])->GetD3D11RenderTarget();
		}
		else
		{
			RenderTargetViews[RenderTargetIndex] = nullptr;
		}
	}
	_DeviceContext->OMSetRenderTargets(RenderTargetsCount, RenderTargetViews, nullptr);
}

void D3D11Context::SetViewport(_In_ Viewport* ViewportObj)
{
	_Viewport = ViewportObj;
	_Viewport->Apply(this);
}
void D3D11Context::SetBlendMode(_In_ BlendState* BlendStateObj)
{
	_BlendState = BlendStateObj;
	_BlendState->Apply(this);
}

void D3D11Context::_BindVertexShader(_In_ Shader* ShaderObj)
{
	_DeviceContext->VSSetShader((ID3D11VertexShader*)static_cast<D3D11Shader*>(ShaderObj)->GetD3D11Shader(), nullptr, 0);
}
void D3D11Context::_BindGeometryShader(_In_ Shader* ShaderObj)
{
	_DeviceContext->GSSetShader((ID3D11GeometryShader*)static_cast<D3D11Shader*>(ShaderObj)->GetD3D11Shader(), nullptr, 0);
}
void D3D11Context::_BindPixelShader(_In_ Shader* ShaderObj)
{
	_DeviceContext->PSSetShader((ID3D11PixelShader*)static_cast<D3D11Shader*>(ShaderObj)->GetD3D11Shader(), nullptr, 0);
}
void Eternal::Graphics::D3D11Context::_UnbindVertexShader()
{
	_DeviceContext->VSSetShader(nullptr, nullptr, 0);
}
void Eternal::Graphics::D3D11Context::_UnbindGeometryShader()
{
	_DeviceContext->GSSetShader(nullptr, nullptr, 0);
}
void Eternal::Graphics::D3D11Context::_UnbindPixelShader()
{
	_DeviceContext->PSSetShader(nullptr, nullptr, 0);
}
void D3D11Context::_BindVSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj)
{
	ID3D11Buffer* buffer = static_cast<D3D11Constant*>(ConstantObj)->GetD3D11Buffer();
	_DeviceContext->VSSetConstantBuffers(Slot, 1, &buffer);
}
void D3D11Context::_BindGSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj)
{
	ID3D11Buffer* buffer = static_cast<D3D11Constant*>(ConstantObj)->GetD3D11Buffer();
	_DeviceContext->GSSetConstantBuffers(Slot, 1, &buffer);
}
void D3D11Context::_BindPSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj)
{
	ID3D11Buffer* buffer = static_cast<D3D11Constant*>(ConstantObj)->GetD3D11Buffer();
	_DeviceContext->PSSetConstantBuffers(Slot, 1, &buffer);
}
void Eternal::Graphics::D3D11Context::_UnbindVSConstant(_In_ uint32_t Slot)
{
	_DeviceContext->VSSetConstantBuffers(Slot, 1, nullptr);
}
void Eternal::Graphics::D3D11Context::_UnbindGSConstant(_In_ uint32_t Slot)
{
	_DeviceContext->GSSetConstantBuffers(Slot, 1, nullptr);
}
void Eternal::Graphics::D3D11Context::_UnbindPSConstant(_In_ uint32_t Slot)
{
	_DeviceContext->PSSetConstantBuffers(Slot, 1, nullptr);
}
void D3D11Context::_BindVSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj)
{
	ID3D11ShaderResourceView* ShaderResourceView = static_cast<D3D11Resource*>(BufferObj)->GetD3D11ShaderResourceView();
	_DeviceContext->VSSetShaderResources(Slot, 1, &ShaderResourceView);
}
void D3D11Context::_BindGSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj)
{
	ID3D11ShaderResourceView* ShaderResourceView = static_cast<D3D11Resource*>(BufferObj)->GetD3D11ShaderResourceView();
	_DeviceContext->GSSetShaderResources(Slot, 1, &ShaderResourceView);
}
void D3D11Context::_BindPSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj)
{
	ID3D11ShaderResourceView* ShaderResourceView = static_cast<D3D11Resource*>(BufferObj)->GetD3D11ShaderResourceView();
	_DeviceContext->PSSetShaderResources(Slot, 1, &ShaderResourceView);
}
void Eternal::Graphics::D3D11Context::_UnbindVSBuffer(_In_ uint32_t Slot)
{
	_DeviceContext->VSSetShaderResources(Slot, 1, nullptr);
}
void Eternal::Graphics::D3D11Context::_UnbindGSBuffer(_In_ uint32_t Slot)
{
	_DeviceContext->GSSetShaderResources(Slot, 1, nullptr);
}
void Eternal::Graphics::D3D11Context::_UnbindPSBuffer(_In_ uint32_t Slot)
{
	_DeviceContext->PSSetShaderResources(Slot, 1, nullptr);
}
void D3D11Context::_BindVSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj)
{
	ID3D11SamplerState* SamplerState = static_cast<D3D11Sampler*>(SamplerObj)->GetD3D11SamplerState();
	_DeviceContext->VSSetSamplers(Slot, 1, &SamplerState);
}
void D3D11Context::_BindGSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj)
{
	ID3D11SamplerState* SamplerState = static_cast<D3D11Sampler*>(SamplerObj)->GetD3D11SamplerState();
	_DeviceContext->GSSetSamplers(Slot, 1, &SamplerState);
}
void D3D11Context::_BindPSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj)
{
	ID3D11SamplerState* SamplerState = static_cast<D3D11Sampler*>(SamplerObj)->GetD3D11SamplerState();
	_DeviceContext->PSSetSamplers(Slot, 1, &SamplerState);
}
void Eternal::Graphics::D3D11Context::_UnbindVSSampler(_In_ uint32_t Slot)
{
	_DeviceContext->VSSetSamplers(Slot, 1, nullptr);
}
void Eternal::Graphics::D3D11Context::_UnbindGSSampler(_In_ uint32_t Slot)
{
	_DeviceContext->GSSetSamplers(Slot, 1, nullptr);
}
void Eternal::Graphics::D3D11Context::_UnbindPSSampler(_In_ uint32_t Slot)
{
	_DeviceContext->PSSetSamplers(Slot, 1, nullptr);
}