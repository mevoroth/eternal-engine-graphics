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
					(void**)&_CBV_SRV_UAV_DescriptorHeap
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
	}
}
