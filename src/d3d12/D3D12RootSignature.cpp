#include "d3d12/D3D12RootSignature.hpp"

#include <d3d12.h>
#include "Macros/Macros.hpp"
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12RootSignature.hpp"

using namespace Eternal::Graphics;

D3D12RootSignature::D3D12RootSignature(_In_ Device& DeviceObj)
{
	D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc;

	RootSignatureDesc.NumParameters		= 0;
	RootSignatureDesc.pParameters		= nullptr;
	RootSignatureDesc.NumStaticSamplers	= 0;
	RootSignatureDesc.pStaticSamplers	= nullptr;
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
