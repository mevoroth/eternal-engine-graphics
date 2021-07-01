#include "d3d12/D3D12GraphicsContext.hpp"

#include <d3d12.h>
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12Utils.hpp"

namespace Eternal
{
	namespace Graphics
	{
		D3D12GraphicsContext::D3D12GraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation)
			: GraphicsContext(CreateInformation)
		{
			using namespace Eternal::Graphics::D3D12;

			ID3D12Device* InD3DDevice = static_cast<D3D12Device&>(GetDevice()).GetD3D12Device();

			// CBV/SRV/UAV
			D3D12_DESCRIPTOR_HEAP_DESC CBV_SRV_UAV_DescriptorHeapDesc;
			CBV_SRV_UAV_DescriptorHeapDesc.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			CBV_SRV_UAV_DescriptorHeapDesc.NumDescriptors	= MaxShaderResourceViewCount + MaxUnorderedAccessViewCount + MaxConstantBufferViewCount;
			CBV_SRV_UAV_DescriptorHeapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			CBV_SRV_UAV_DescriptorHeapDesc.NodeMask			= GetDevice().GetDeviceMask();
			
			VerifySuccess(
				InD3DDevice->CreateDescriptorHeap(
					&CBV_SRV_UAV_DescriptorHeapDesc,
					__uuidof(ID3D12DescriptorHeap),
					(void**)&_CBV_SRV_UAV_DescriptorHeap
				)
			);
			VerifySuccess(
				_CBV_SRV_UAV_DescriptorHeap->SetName(L"CBV_SRV_UAV_DescriptorHeap")
			);

			// Sampler
			D3D12_DESCRIPTOR_HEAP_DESC SamplerDescriptorHeapDesc;
			SamplerDescriptorHeapDesc.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
			SamplerDescriptorHeapDesc.NumDescriptors	= MaxSamplerCount;
			SamplerDescriptorHeapDesc.Flags				= D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			SamplerDescriptorHeapDesc.NodeMask			= GetDevice().GetDeviceMask();

			VerifySuccess(
				InD3DDevice->CreateDescriptorHeap(
					&SamplerDescriptorHeapDesc,
					__uuidof(ID3D12DescriptorHeap),
					(void**)&_SamplerDescriptorHeap
				)
			);
			VerifySuccess(
				_CBV_SRV_UAV_DescriptorHeap->SetName(L"Sampler_DescriptorHeap")
			);

			// RTV
			D3D12_DESCRIPTOR_HEAP_DESC RenderTargetViewDescriptorHeapDesc;
			RenderTargetViewDescriptorHeapDesc.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			RenderTargetViewDescriptorHeapDesc.NumDescriptors	= MaxRenderTargetViewCount;
			RenderTargetViewDescriptorHeapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			RenderTargetViewDescriptorHeapDesc.NodeMask			= GetDevice().GetDeviceMask();

			VerifySuccess(
				InD3DDevice->CreateDescriptorHeap(
					&RenderTargetViewDescriptorHeapDesc,
					__uuidof(ID3D12DescriptorHeap),
					(void**)&_RenderTargetViewDescriptorHeap
				)
			);
			VerifySuccess(
				_CBV_SRV_UAV_DescriptorHeap->SetName(L"RenderTargetView_DescriptorHeap")
			);

			// DSV
			D3D12_DESCRIPTOR_HEAP_DESC DepthStencilViewDescriptorHeapDesc;
			DepthStencilViewDescriptorHeapDesc.Type				= D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			DepthStencilViewDescriptorHeapDesc.NumDescriptors	= MaxDepthStencilViewCount;
			DepthStencilViewDescriptorHeapDesc.Flags			= D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			DepthStencilViewDescriptorHeapDesc.NodeMask			= GetDevice().GetDeviceMask();

			VerifySuccess(
				InD3DDevice->CreateDescriptorHeap(
					&DepthStencilViewDescriptorHeapDesc,
					__uuidof(ID3D12DescriptorHeap),
					(void**)&_DepthStencilViewDescriptorHeap
				)
			);
			VerifySuccess(
				_DepthStencilViewDescriptorHeap->SetName(L"DepthStencilView_DescriptorHeap")
			);

			_CBV_SRV_UAV_DescriptorHandleIncrementSize		= InD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			_SamplerDescriptorHandleIncrementSize			= InD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
			_RenderTargetViewDescriptorHandleIncrementSize	= InD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			_DepthStencilViewDescriptorHandleIncrementSize	= InD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		}

		D3D12GraphicsContext::~D3D12GraphicsContext()
		{
			_DepthStencilViewDescriptorHeap->Release();
			_DepthStencilViewDescriptorHeap = nullptr;

			_RenderTargetViewDescriptorHeap->Release();
			_RenderTargetViewDescriptorHeap = nullptr;

			_SamplerDescriptorHeap->Release();
			_SamplerDescriptorHeap = nullptr;

			_CBV_SRV_UAV_DescriptorHeap->Release();
			_CBV_SRV_UAV_DescriptorHeap = nullptr;
		}

		template<typename BitFieldHandles>
		static D3D12Handle AllocateDescriptor(_In_ BitFieldHandles& Handles, _In_ ID3D12DescriptorHeap* DescriptorHeap, _In_ uint32_t IncrementSize, _In_ uint32_t Offset = 0)
		{
			D3D12Handle Handle;

			Handle.DescriptorHandle				= Handles.Pop();
			Handle.D3D12CPUDescriptorHandle		= DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			Handle.D3D12GPUDescriptorHandle		= DescriptorHeap->GetGPUDescriptorHandleForHeapStart();
			Handle.D3D12CPUDescriptorHandle.ptr	+= IncrementSize * (Offset + Handle.DescriptorHandle);
			Handle.D3D12GPUDescriptorHandle.ptr	+= IncrementSize * (Offset + Handle.DescriptorHandle);

			return Handle;
		}

		D3D12Handle D3D12GraphicsContext::AllocateConstantBufferViewDescriptor()
		{
			return AllocateDescriptor(
				_ConstantBufferViewHandles,
				_CBV_SRV_UAV_DescriptorHeap,
				_CBV_SRV_UAV_DescriptorHandleIncrementSize
			);
		}

		D3D12Handle D3D12GraphicsContext::AllocateShaderResourceViewDescriptor()
		{
			return AllocateDescriptor(
				_ShaderResourceViewHandles,
				_CBV_SRV_UAV_DescriptorHeap,
				_CBV_SRV_UAV_DescriptorHandleIncrementSize,
				MaxConstantBufferViewCount
			);
		}

		D3D12Handle D3D12GraphicsContext::AllocateUnorderedAccessViewDescriptor()
		{
			return AllocateDescriptor(
				_UnorderedAccessViewHandles,
				_CBV_SRV_UAV_DescriptorHeap,
				_CBV_SRV_UAV_DescriptorHandleIncrementSize,
				MaxConstantBufferViewCount + MaxShaderResourceViewCount
			);
		}

		D3D12Handle D3D12GraphicsContext::AllocateSamplerDescriptor()
		{
			return AllocateDescriptor(
				_SamplerHandles,
				_SamplerDescriptorHeap,
				_SamplerDescriptorHandleIncrementSize
			);
		}

		D3D12Handle D3D12GraphicsContext::AllocateRenderTargetViewDescriptor()
		{
			return AllocateDescriptor(
				_RenderTargetViewHandles,
				_RenderTargetViewDescriptorHeap,
				_RenderTargetViewDescriptorHandleIncrementSize
			);
		}

		D3D12Handle D3D12GraphicsContext::AllocateDepthStencilViewDescriptor()
		{
			return AllocateDescriptor(
				_DepthStencilViewHandles,
				_DepthStencilViewDescriptorHeap,
				_DepthStencilViewDescriptorHandleIncrementSize
			);
		}

		void D3D12GraphicsContext::ReleaseConstantBufferViewDescriptor(_Inout_ D3D12Handle& InOutHandle)
		{
			_ConstantBufferViewHandles.Push(InOutHandle.DescriptorHandle);
		}

		void D3D12GraphicsContext::ReleaseShaderResourceViewDescriptor(_Inout_ D3D12Handle& InOutHandle)
		{
			_ShaderResourceViewHandles.Push(InOutHandle.DescriptorHandle);
		}

		void D3D12GraphicsContext::ReleaseUnorderedAccessViewDescriptor(_Inout_ D3D12Handle& InOutHandle)
		{
			_UnorderedAccessViewHandles.Push(InOutHandle.DescriptorHandle);
		}

		void D3D12GraphicsContext::ReleaseSamplerDescriptor(_Inout_ D3D12Handle& InOutHandle)
		{
			_SamplerHandles.Push(InOutHandle.DescriptorHandle);
		}

		void D3D12GraphicsContext::ReleaseRenderTargetViewDescriptor(_Inout_ D3D12Handle& InOutHandle)
		{
			_RenderTargetViewHandles.Push(InOutHandle.DescriptorHandle);
		}

		void D3D12GraphicsContext::ReleaseDepthStencilViewDescriptor(_Inout_ D3D12Handle& InOutHandle)
		{
			_DepthStencilViewHandles.Push(InOutHandle.DescriptorHandle);
		}
	}
}
