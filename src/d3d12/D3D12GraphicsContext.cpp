#if ETERNAL_ENABLE_D3D12

#include "d3d12/D3D12GraphicsContext.hpp"

#include <d3d12.h>
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12Utils.hpp"
#include "Graphics/Fence.hpp"

#if ETERNAL_USE_DEBUG_VERBOSE
#include "Log/Log.hpp"
#include <sstream>
#include <iomanip>
#endif

namespace Eternal
{
	namespace Graphics
	{
		void LogDescriptorHeap(_In_ const string& InDescriptorHeapName, _In_ ID3D12DescriptorHeap* InDescriptorHeap, _In_ uint32_t InDescriptorCount, _In_ uint32_t InDescriptorSize)
		{
			(void)InDescriptorHeapName;
			(void)InDescriptorHeap;
			(void)InDescriptorCount;
			(void)InDescriptorSize;
#if ETERNAL_USE_DEBUG_VERBOSE
			using namespace std;
			using namespace Eternal::LogSystem;

			SIZE_T CPUDescriptorStart	= InDescriptorHeap->GetCPUDescriptorHandleForHeapStart().ptr;
			SIZE_T GPUDescriptorStart	= InDescriptorHeap->GetGPUDescriptorHandleForHeapStart().ptr;

			stringstream LogDescriptorHeapStream;
			LogDescriptorHeapStream << "DescriptorHeap: { Name = " << InDescriptorHeapName << ", "
									<< "CPUPtr = { 0x" << hex << setfill('0') << setw(16) << CPUDescriptorStart << "-0x" << setfill('0') << setw(16) << CPUDescriptorStart + InDescriptorSize * InDescriptorCount << " }, "
									<< "GPUVA = { 0x" << setfill('0') << setw(16) << GPUDescriptorStart << "-0x" << setw(16) << GPUDescriptorStart + InDescriptorSize * InDescriptorCount << " } }";
			LogWrite(LogInfo, LogGraphics, LogDescriptorHeapStream.str());
#endif
		}

		D3D12GraphicsContext::D3D12GraphicsContext(_In_ const WindowsGraphicsContextCreateInformation& InWindowsGraphicsContextCreateInformation)
			: WindowsGraphicsContext(InWindowsGraphicsContextCreateInformation)
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
					reinterpret_cast<void**>(&_CBV_SRV_UAV_DescriptorHeap)
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
					reinterpret_cast<void**>(&_SamplerDescriptorHeap)
				)
			);
			VerifySuccess(
				_SamplerDescriptorHeap->SetName(L"Sampler_DescriptorHeap")
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
					reinterpret_cast<void**>(&_RenderTargetViewDescriptorHeap)
				)
			);
			VerifySuccess(
				_RenderTargetViewDescriptorHeap->SetName(L"RenderTargetView_DescriptorHeap")
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
					reinterpret_cast<void**>(&_DepthStencilViewDescriptorHeap)
				)
			);
			VerifySuccess(
				_DepthStencilViewDescriptorHeap->SetName(L"DepthStencilView_DescriptorHeap")
			);

			_CBV_SRV_UAV_DescriptorHandleIncrementSize		= InD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			_SamplerDescriptorHandleIncrementSize			= InD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);
			_RenderTargetViewDescriptorHandleIncrementSize	= InD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			_DepthStencilViewDescriptorHandleIncrementSize	= InD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

			LogDescriptorHeap("CBV_SRV_UAV_DescriptorHeap",			_CBV_SRV_UAV_DescriptorHeap,		MaxShaderResourceViewCount + MaxUnorderedAccessViewCount + MaxConstantBufferViewCount,	_CBV_SRV_UAV_DescriptorHandleIncrementSize);
			LogDescriptorHeap("Sampler_DescriptorHeap",				_SamplerDescriptorHeap,				MaxSamplerCount,																		_SamplerDescriptorHandleIncrementSize);
			LogDescriptorHeap("RenderTargetView_DescriptorHeap",	_RenderTargetViewDescriptorHeap,	MaxRenderTargetViewCount,																_RenderTargetViewDescriptorHandleIncrementSize);
			LogDescriptorHeap("DepthStencilView_DescriptorHeap",	_DepthStencilViewDescriptorHeap,	MaxDepthStencilViewCount,																_DepthStencilViewDescriptorHandleIncrementSize);
		}

		D3D12GraphicsContext::~D3D12GraphicsContext()
		{
			WaitForAllFences();

			_DepthStencilViewDescriptorHeap->Release();
			_DepthStencilViewDescriptorHeap = nullptr;

			_RenderTargetViewDescriptorHeap->Release();
			_RenderTargetViewDescriptorHeap = nullptr;

			_SamplerDescriptorHeap->Release();
			_SamplerDescriptorHeap = nullptr;

			_CBV_SRV_UAV_DescriptorHeap->Release();
			_CBV_SRV_UAV_DescriptorHeap = nullptr;
		}

		void D3D12GraphicsContext::ResetFrameStates()
		{
			GraphicsContext::ResetFrameStates();

#if ETERNAL_USE_NVIDIA_AFTERMATH
			static_cast<D3D12Device&>(GetDevice()).GetNVIDIANsightAftermath().ResetMarkers();
#endif
		}

		template<bool IsShaderVisible = true, typename BitFieldHandles>
		static D3D12Handle AllocateDescriptor(_In_ BitFieldHandles& Handles, _In_ ID3D12DescriptorHeap* DescriptorHeap, _In_ uint32_t IncrementSize, _In_ uint32_t Offset = 0)
		{
			D3D12Handle Handle;

			Handle.DescriptorHandle					= Handles.Pop();
			Handle.D3D12CPUDescriptorHandle			= DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			Handle.D3D12CPUDescriptorHandle.ptr		+= IncrementSize * (Offset + Handle.DescriptorHandle);
			if (IsShaderVisible)
			{
				Handle.D3D12GPUDescriptorHandle		= DescriptorHeap->GetGPUDescriptorHandleForHeapStart();
				Handle.D3D12GPUDescriptorHandle.ptr	+= IncrementSize * (Offset + Handle.DescriptorHandle);
			}

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
			return AllocateDescriptor</* IsShaderVisible = */ false>(
				_RenderTargetViewHandles,
				_RenderTargetViewDescriptorHeap,
				_RenderTargetViewDescriptorHandleIncrementSize
			);
		}

		D3D12Handle D3D12GraphicsContext::AllocateDepthStencilViewDescriptor()
		{
			return AllocateDescriptor</* IsShaderVisible = */ false>(
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

#endif
