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
		static D3D12_CPU_DESCRIPTOR_HANDLE AllocateDescriptor(_In_ BitFieldHandles& Handles, ID3D12DescriptorHeap* DescriptorHeap, uint32_t IncrementSize, uint32_t Offset, _Out_ Handle& OutHandle)
		{
			OutHandle = Handles.Pop();
			
			D3D12_CPU_DESCRIPTOR_HANDLE OutCPUHandle = DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			OutCPUHandle.ptr += IncrementSize * (Offset + OutHandle);

			return OutCPUHandle;
		}

		D3D12_CPU_DESCRIPTOR_HANDLE D3D12GraphicsContext::AllocateConstantBufferViewDescriptor(_Out_ Handle& OutHandle)
		{
			return AllocateDescriptor(
				_ConstantBufferViewHandles,
				_CBV_SRV_UAV_DescriptorHeap,
				_CBV_SRV_UAV_DescriptorHandleIncrementSize,
				0,
				OutHandle
			);
		}

		D3D12_CPU_DESCRIPTOR_HANDLE D3D12GraphicsContext::AllocateShaderResourceViewDescriptor(_Out_ Handle& OutHandle)
		{
			return AllocateDescriptor(
				_ShaderResourceViewHandles,
				_CBV_SRV_UAV_DescriptorHeap,
				_CBV_SRV_UAV_DescriptorHandleIncrementSize,
				MaxConstantBufferViewCount,
				OutHandle
			);
		}

		D3D12_CPU_DESCRIPTOR_HANDLE D3D12GraphicsContext::AllocateUnorderedAccessViewDescriptor(_Out_ Handle& OutHandle)
		{
			return AllocateDescriptor(
				_UnorderedAccessViewHandles,
				_CBV_SRV_UAV_DescriptorHeap,
				_CBV_SRV_UAV_DescriptorHandleIncrementSize,
				MaxConstantBufferViewCount + MaxShaderResourceViewCount,
				OutHandle
			);
		}

		D3D12_CPU_DESCRIPTOR_HANDLE D3D12GraphicsContext::AllocateSamplerDescriptor(_Out_ Handle& OutHandle)
		{
			return AllocateDescriptor(
				_SamplerHandles,
				_SamplerDescriptorHeap,
				_SamplerDescriptorHandleIncrementSize,
				0,
				OutHandle
			);
		}

		D3D12_CPU_DESCRIPTOR_HANDLE D3D12GraphicsContext::AllocateRenderTargetViewDescriptor(_Out_ Handle& OutHandle)
		{
			return AllocateDescriptor(
				_RenderTargetViewHandles,
				_RenderTargetViewDescriptorHeap,
				_RenderTargetViewDescriptorHandleIncrementSize,
				0,
				OutHandle
			);
		}

		D3D12_CPU_DESCRIPTOR_HANDLE D3D12GraphicsContext::AllocateDepthStencilViewDescriptor(_Out_ Handle& OutHandle)
		{
			return AllocateDescriptor(
				_DepthStencilViewHandles,
				_DepthStencilViewDescriptorHeap,
				_DepthStencilViewDescriptorHandleIncrementSize,
				0,
				OutHandle
			);
		}

		void D3D12GraphicsContext::ReleaseConstantBufferViewDescriptor(_Inout_ Handle& InOutHandle)
		{
			_ConstantBufferViewHandles.Push(InOutHandle);
		}

		void D3D12GraphicsContext::ReleaseShaderResourceViewDescriptor(_Inout_ Handle& InOutHandle)
		{
			_ShaderResourceViewHandles.Push(InOutHandle);
		}

		void D3D12GraphicsContext::ReleaseUnorderedAccessViewDescriptor(_Inout_ Handle& InOutHandle)
		{
			_UnorderedAccessViewHandles.Push(InOutHandle);
		}

		void D3D12GraphicsContext::ReleaseSamplerDescriptor(_Inout_ Handle& InOutHandle)
		{
			_SamplerHandles.Push(InOutHandle);
		}

		void D3D12GraphicsContext::ReleaseRenderTargetViewDescriptor(_Inout_ Handle& InOutHandle)
		{
			_RenderTargetViewHandles.Push(InOutHandle);
		}

		void D3D12GraphicsContext::ReleaseDepthStencilViewDescriptor(_Inout_ Handle& InOutHandle)
		{
			_DepthStencilViewHandles.Push(InOutHandle);
		}
	}
}
