#include "d3d12/D3D12RootSignature.hpp"

#include <cstdint>
#include <vector>
#include <d3d12.h>
#include "Macros/Macros.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12RootSignature.hpp"
#include "d3d12/D3D12Sampler.hpp"
#include "d3d12/D3D12DescriptorHeap.hpp"

using namespace std;
using namespace Eternal::Graphics;

D3D12RootSignature::D3D12RootSignature(_In_ Device& DeviceObj, _In_ Sampler* StaticSamplers[], _In_ uint32_t StaticSamplersCount, _In_ DescriptorHeap* DescriptorHeaps[], _In_ uint32_t DescriptorHeapsCount, _In_ const RootSignatureAccess& RootSignatureAccessObj)
{
	vector<D3D12_STATIC_SAMPLER_DESC> D3D12StaticSamplers;
	D3D12StaticSamplers.resize(StaticSamplersCount);

	for (uint32_t StaticSamplerIndex = 0; StaticSamplerIndex < StaticSamplersCount; ++StaticSamplerIndex)
	{
		static_cast<D3D12Sampler*>(StaticSamplers[StaticSamplerIndex])->GetD3D12StaticSampler(D3D12StaticSamplers[StaticSamplerIndex]);
	}

	vector<D3D12_ROOT_PARAMETER> D3D12RootParameters;
	D3D12RootParameters.resize(DescriptorHeapsCount);

	for (uint32_t RootParameterIndex = 0; RootParameterIndex < DescriptorHeapsCount; ++RootParameterIndex)
	{
		D3D12RootParameters[RootParameterIndex].ParameterType				= static_cast<D3D12DescriptorHeap*>(DescriptorHeaps[RootParameterIndex])->GetD3D12RootParameterType();
		D3D12RootParameters[RootParameterIndex].Descriptor.RegisterSpace	= DescriptorHeaps[RootParameterIndex]->GetSpace();
		D3D12RootParameters[RootParameterIndex].Descriptor.ShaderRegister	= DescriptorHeaps[RootParameterIndex]->GetRegister();
		D3D12RootParameters[RootParameterIndex].ShaderVisibility			= D3D12_SHADER_VISIBILITY_ALL;
	}

	D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc;

	RootSignatureDesc.NumParameters		= D3D12RootParameters.size();
	RootSignatureDesc.pParameters		= D3D12RootParameters.data();
	RootSignatureDesc.NumStaticSamplers	= D3D12StaticSamplers.size();
	RootSignatureDesc.pStaticSamplers	= D3D12StaticSamplers.data();
	RootSignatureDesc.Flags				= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* RootSignatureBlob = nullptr;
	ID3DBlob* ErrorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &RootSignatureBlob, &ErrorBlob);
	ETERNAL_ASSERT(hr == S_OK);

	hr = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreateRootSignature(
		DeviceObj.GetDeviceMask(),
		RootSignatureBlob->GetBufferPointer(),
		RootSignatureBlob->GetBufferSize(),
		__uuidof(ID3D12RootSignature),
		(void**)&_RootSignature
	);
	ETERNAL_ASSERT(hr == S_OK);
}

D3D12RootSignature::~D3D12RootSignature()
{
	_RootSignature->Release();
	_RootSignature = nullptr;
}
