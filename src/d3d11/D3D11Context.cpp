#include "d3d11/D3D11Context.hpp"

#include <d3d11.h>

#include "d3d11/D3D11RenderTarget.hpp"
#include "d3d11/D3D11Shader.hpp"
#include "d3d11/D3D11VertexShader.hpp"
#include "d3d11/D3D11Constant.hpp"
#include "d3d11/ID3D11ShaderResource.hpp"
#include "d3d11/D3D11VertexBuffer.hpp"
#include "d3d11/D3D11IndexBuffer.hpp"
#include "d3d11/D3D11DepthStencil.hpp"
#include "d3d11/D3D11DepthStencilBuffer.hpp"
#include "d3d11/D3D11Sampler.hpp"
#include "d3d11/D3D11Viewport.hpp"
#include "d3d11/D3D11BlendState.hpp"

using namespace Eternal::Graphics;

static const D3D11_PRIMITIVE_TOPOLOGY D3D11_PRIMITIVE_TYPE[] = {
	D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
	D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
};

D3D11Context::D3D11Context(_In_ ID3D11DeviceContext* D3D11ContextObj, _In_ bool IsDeferred)
	: Context(IsDeferred)
	, _DeviceContext(D3D11ContextObj)
{
	_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (int RenderTargetIndex = 0; RenderTargetIndex < D3D11_MAX_RENDERTARGETS; ++RenderTargetIndex)
	{
		_RenderTargets[RenderTargetIndex] = nullptr;
	}
}

D3D11Context::~D3D11Context()
{
	_DeviceContext->Release();
	_DeviceContext = nullptr;
}

void D3D11Context::SetTopology(_In_ const Topology& TopologyObj)
{
	ETERNAL_ASSERT(TopologyObj < TOPOLOGY_COUNT);
	_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TYPE[TopologyObj]);
}

void D3D11Context::DrawIndexed(_In_ VertexBuffer* VerticesBuffer, _In_ IndexBuffer* IndicesBuffer)
{
	//_CommitRenderState();

	uint32_t Stride = (uint32_t)VerticesBuffer->GetSize();
	uint32_t Offset = 0;

	ID3D11Buffer* D3D11Indices = (ID3D11Buffer*)IndicesBuffer->GetNative();
	DXGI_FORMAT D3D11IndicesFormat = (DXGI_FORMAT)IndicesBuffer->GetNativeFormat();
	ID3D11Buffer* D3D11Vertices = (ID3D11Buffer*)VerticesBuffer->GetNative();

	_DeviceContext->IASetVertexBuffers(0, 1, &D3D11Vertices, &Stride, &Offset);
	_DeviceContext->IASetIndexBuffer(D3D11Indices, D3D11IndicesFormat, 0);

	_DeviceContext->DrawIndexed(IndicesBuffer->GetCount(), 0, 0);
}

void D3D11Context::DrawIndexedInstanced(_In_ VertexBuffer* VerticesBuffer, _In_ IndexBuffer* IndicesBuffer, _In_ int InstanceCount)
{
	uint32_t Stride = (uint32_t)VerticesBuffer->GetSize();
	uint32_t Offset = 0;

	ID3D11Buffer* D3D11Indices = (ID3D11Buffer*)IndicesBuffer->GetNative();
	DXGI_FORMAT D3D11IndicesFormat = (DXGI_FORMAT)IndicesBuffer->GetNativeFormat();
	ID3D11Buffer* D3D11Vertices = (ID3D11Buffer*)VerticesBuffer->GetNative();

	_DeviceContext->IASetVertexBuffers(0, 1, &D3D11Vertices, &Stride, &Offset);
	_DeviceContext->IASetIndexBuffer(D3D11Indices, D3D11IndicesFormat, 0);

	_DeviceContext->DrawIndexedInstanced(IndicesBuffer->GetCount(), InstanceCount, 0, 0, 0);
}

void D3D11Context::DrawDirect(_In_ VertexBuffer* VerticesBuffer)
{
	//_CommitRenderState();

	uint32_t Stride = (uint32_t)VerticesBuffer->GetSize();
	uint32_t Offset = 0;
	ID3D11Buffer* D3D11Vertices = (ID3D11Buffer*)VerticesBuffer->GetNative();

	_DeviceContext->IASetVertexBuffers(0, 1, &D3D11Vertices, &Stride, &Offset);

	_DeviceContext->Draw((UINT)VerticesBuffer->GetVerticesCount(), 0);
}

void D3D11Context::DrawPrimitive(_In_ uint32_t PrimitiveCount)
{
	ETERNAL_ASSERT(PrimitiveCount);
	ETERNAL_ASSERT(!(PrimitiveCount % 3));
	_CommitRenderState();
	
	_DeviceContext->Draw(PrimitiveCount, 0);
}

void D3D11Context::SetRenderTargets(_In_ RenderTarget** RenderTargets, _In_ int RenderTargetsCount)
{
	for (int RenderTargetIndex = 0; RenderTargetIndex < RenderTargetsCount; ++RenderTargetIndex)
	{
		if (_RenderTargets[RenderTargetIndex] != RenderTargets[RenderTargetIndex])
		{
			_RenderTargets[RenderTargetIndex] = RenderTargets[RenderTargetIndex];
			_MarkRenderStateAsDirty();
		}
	}

	_CommitRenderState();
}

void D3D11Context::SetDepthBuffer(_In_ RenderTarget* DepthBuffer)
{
	_DepthBuffer = DepthBuffer;
	_MarkRenderStateAsDirty();
	_CommitRenderState();
}

void D3D11Context::SetViewport(_In_ Viewport* ViewportObj)
{
	_Viewport = ViewportObj;
	static_cast<D3D11Viewport*>(_Viewport)->Apply(this);
}
void D3D11Context::SetBlendMode(_In_ BlendState* BlendStateObj)
{
	_BlendState = BlendStateObj;
	static_cast<D3D11BlendState*>(_BlendState)->Apply(this);
}

void D3D11Context::BindDepthStencilState(_In_ DepthStencil* DepthStencilState)
{
	_DeviceContext->OMSetDepthStencilState(static_cast<D3D11DepthStencil*>(DepthStencilState)->GetD3D11DepthStencilState(), 0x0);
}

void D3D11Context::UnbindDepthStencilState()
{
	_DeviceContext->OMSetDepthStencilState(nullptr, 0x0);
}

void D3D11Context::_BindVertexShader(_In_ Shader* ShaderObj)
{
	_DeviceContext->IASetInputLayout(static_cast<D3D11VertexShader*>(ShaderObj)->GetInputLayout()->GetD3D11InputLayout());
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
	ID3D11Buffer* NullConstant = nullptr;
	_DeviceContext->VSSetConstantBuffers(Slot, 1, &NullConstant);
}
void Eternal::Graphics::D3D11Context::_UnbindGSConstant(_In_ uint32_t Slot)
{
	ID3D11Buffer* NullConstant = nullptr;
	_DeviceContext->GSSetConstantBuffers(Slot, 1, &NullConstant);
}
void Eternal::Graphics::D3D11Context::_UnbindPSConstant(_In_ uint32_t Slot)
{
	ID3D11Buffer* NullConstant = nullptr;
	_DeviceContext->PSSetConstantBuffers(Slot, 1, &NullConstant);
}
void D3D11Context::_BindVSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj)
{
	ETERNAL_ASSERT(dynamic_cast<ID3D11ShaderResource*>(BufferObj));
	ID3D11ShaderResourceView* ShaderResourceView = dynamic_cast<ID3D11ShaderResource*>(BufferObj)->GetD3D11ShaderResourceView();
	_DeviceContext->VSSetShaderResources(Slot, 1, &ShaderResourceView);
}
void D3D11Context::_BindGSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj)
{
	ETERNAL_ASSERT(dynamic_cast<ID3D11ShaderResource*>(BufferObj));
	ID3D11ShaderResourceView* ShaderResourceView = dynamic_cast<ID3D11ShaderResource*>(BufferObj)->GetD3D11ShaderResourceView();
	_DeviceContext->GSSetShaderResources(Slot, 1, &ShaderResourceView);
}
void D3D11Context::_BindPSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj)
{
	ETERNAL_ASSERT(dynamic_cast<ID3D11ShaderResource*>(BufferObj));
	ID3D11ShaderResourceView* ShaderResourceView = dynamic_cast<ID3D11ShaderResource*>(BufferObj)->GetD3D11ShaderResourceView();
	_DeviceContext->PSSetShaderResources(Slot, 1, &ShaderResourceView);
}
void Eternal::Graphics::D3D11Context::_UnbindVSBuffer(_In_ uint32_t Slot)
{
	ID3D11ShaderResourceView* NullResource = nullptr;
	_DeviceContext->VSSetShaderResources(Slot, 1, &NullResource);
}
void Eternal::Graphics::D3D11Context::_UnbindGSBuffer(_In_ uint32_t Slot)
{
	ID3D11ShaderResourceView* NullResource = nullptr;
	_DeviceContext->GSSetShaderResources(Slot, 1, &NullResource);
}
void Eternal::Graphics::D3D11Context::_UnbindPSBuffer(_In_ uint32_t Slot)
{
	ID3D11ShaderResourceView* NullResource = nullptr;
	_DeviceContext->PSSetShaderResources(Slot, 1, &NullResource);
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
void D3D11Context::_UnbindVSSampler(_In_ uint32_t Slot)
{
	ID3D11SamplerState* SamplerState = nullptr;
	_DeviceContext->VSSetSamplers(Slot, 1, &SamplerState);
}
void D3D11Context::_UnbindGSSampler(_In_ uint32_t Slot)
{
	ID3D11SamplerState* SamplerState = nullptr;
	_DeviceContext->GSSetSamplers(Slot, 1, &SamplerState);
}
void D3D11Context::_UnbindPSSampler(_In_ uint32_t Slot)
{
	ID3D11SamplerState* SamplerState = nullptr;
	_DeviceContext->PSSetSamplers(Slot, 1, &SamplerState);
}

void D3D11Context::_CommitRenderState()
{
	if (_IsRenderStateDirty())
	{
		ID3D11RenderTargetView* RenderTargetViews[D3D11_MAX_RENDERTARGETS];
		for (int RenderTargetIndex = 0; RenderTargetIndex < D3D11_MAX_RENDERTARGETS; ++RenderTargetIndex)
		{
			if (_RenderTargets[RenderTargetIndex])
			{
				RenderTargetViews[RenderTargetIndex] = static_cast<D3D11RenderTarget*>(_RenderTargets[RenderTargetIndex])->GetD3D11RenderTarget();
			}
			else
			{
				RenderTargetViews[RenderTargetIndex] = nullptr;
			}
		}
		
		_DeviceContext->OMSetRenderTargets(D3D11_MAX_RENDERTARGETS, RenderTargetViews, (_DepthBuffer ? static_cast<D3D11DepthStencilBuffer*>(_DepthBuffer)->GetD3D11DepthStencilView() : nullptr));
	}
}

void D3D11Context::BeginCommandList()
{
}

void D3D11Context::EndCommandList()
{
	CommandListRecord Record;
	HRESULT hr = GetD3D11Context()->FinishCommandList(FALSE, &Record.CommandList);
	ETERNAL_ASSERT(hr == S_OK);
	Record.Time = Eternal::Time::Time::Get()->GetTimeMicroSeconds();
	_CommandLists.push_back(Record);
}

void D3D11Context::PrepareFlush(Context& ContextObj)
{
	ETERNAL_ASSERT(!ContextObj.IsDeferred());
	ETERNAL_ASSERT(IsDeferred());
	D3D11Context& D3D11ContextObj = (D3D11Context&)ContextObj;

	for (uint32_t CommandListIndex = 0; CommandListIndex < _CommandLists.size(); ++CommandListIndex)
	{
		CommandListRecord& Record = _CommandLists[CommandListIndex];
		vector<CommandListRecord>::iterator it = D3D11ContextObj._CommandLists.begin();
		vector<CommandListRecord>::iterator end = D3D11ContextObj._CommandLists.end();
		for (; it != end; ++it)
		{
			if ((*it).Time > Record.Time)
				break;
		}
		D3D11ContextObj._CommandLists.insert(it, Record);
	}

	_CommandLists.clear();
}

void D3D11Context::Flush()
{
	ETERNAL_ASSERT(!IsDeferred());
	for (int CommandListIndex = 0; CommandListIndex < _CommandLists.size(); ++CommandListIndex)
	{
#ifdef ETERNAL_DEBUG
		if (CommandListIndex + 1 < _CommandLists.size())
		{
			ETERNAL_ASSERT(_CommandLists[CommandListIndex].Time <= _CommandLists[CommandListIndex + 1].Time);
		}
#endif
		_DeviceContext->ExecuteCommandList(_CommandLists[CommandListIndex].CommandList, FALSE);
		_CommandLists[CommandListIndex].CommandList->Release();
	}
	_CommandLists.clear();
}
