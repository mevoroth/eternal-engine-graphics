#include "d3d12_deprecated/D3D12RootSignature.hpp"

#include <d3d12.h>
#include "d3d12_deprecated/D3D12Device.hpp"
#include "d3d12_deprecated/D3D12RootSignature.hpp"
#include "d3d12_deprecated/D3D12Sampler.hpp"
#include "d3d12_deprecated/D3D12DescriptorHeap.hpp"

using namespace std;
using namespace Eternal::Graphics;

const D3D12_ROOT_PARAMETER_TYPE D3D12_PARAMETER_TYPES[] =
{
	(D3D12_ROOT_PARAMETER_TYPE)-1,
	D3D12_ROOT_PARAMETER_TYPE_SRV,
	D3D12_ROOT_PARAMETER_TYPE_UAV,
	D3D12_ROOT_PARAMETER_TYPE_SRV,
	D3D12_ROOT_PARAMETER_TYPE_UAV,
	D3D12_ROOT_PARAMETER_TYPE_SRV,
	D3D12_ROOT_PARAMETER_TYPE_UAV,
	D3D12_ROOT_PARAMETER_TYPE_CBV,
	D3D12_ROOT_PARAMETER_TYPE_UAV,
	D3D12_ROOT_PARAMETER_TYPE_SRV,
	D3D12_ROOT_PARAMETER_TYPE_SRV,
	//D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE
};

ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_PARAMETER_TYPES) == int(RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_COUNT), "D3D12 Parameters Types declaration not complete");

const D3D12_DESCRIPTOR_RANGE_TYPE D3D12_PARAMETER_RANGE_TYPES[] =
{
	D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER,
	D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
	D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
	D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
	D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
	D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
	D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
	D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
	D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
	D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
	D3D12_DESCRIPTOR_RANGE_TYPE_SRV
};

ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_PARAMETER_RANGE_TYPES) == int(RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_COUNT), "D3D12 Parameter Range Types declaration not complete");

D3D12RootSignature::D3D12RootSignature(_In_ Device& DeviceObj, _In_ const vector<RootSignatureParameter> Resources[], _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj)
{
	struct DescriptorTableEntry
	{
		const D3D12_DESCRIPTOR_RANGE**	DescriptorRange;
		uint32_t						DescriptorRangeEntry;
	};

	vector<D3D12_ROOT_PARAMETER> D3D12RootParameters;
	D3D12RootParameters.resize(ResourcesCount);

	vector<D3D12_DESCRIPTOR_RANGE> DescriptorRanges;
	vector<DescriptorTableEntry> ForwardEntryRecords;

	for (uint32_t RootParameterIndex = 0; RootParameterIndex < ResourcesCount; ++RootParameterIndex)
	{
		const vector<RootSignatureParameter>& Parameter = Resources[RootParameterIndex];

		ETERNAL_ASSERT(Parameter.size() > 0);
		if (Parameter.size() == 1 &&
			Parameter[0].Parameter != RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_TEXTURE &&
			Parameter[0].Parameter != RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_SAMPLER)
		{
			const RootSignatureParameter& SingleParameter = Parameter[0];

			D3D12RootParameters[RootParameterIndex].ParameterType							= D3D12_PARAMETER_TYPES[int(SingleParameter.Parameter)];
			D3D12RootParameters[RootParameterIndex].Descriptor.RegisterSpace				= 0;
			D3D12RootParameters[RootParameterIndex].Descriptor.ShaderRegister				= SingleParameter.Register;
			D3D12RootParameters[RootParameterIndex].ShaderVisibility						= D3D12_SHADER_VISIBILITY_ALL;
		}
		else
		{
			uint32_t FirstElement = DescriptorRanges.size();

			for (uint32_t RangeIndex = 0; RangeIndex < Parameter.size(); ++RangeIndex)
			{
				const RootSignatureParameter& CurrentParameter = Parameter[RangeIndex];

				D3D12_DESCRIPTOR_RANGE DescriptorRange;
				DescriptorRange.RangeType							= D3D12_PARAMETER_RANGE_TYPES[int(CurrentParameter.Parameter)];
				DescriptorRange.NumDescriptors						= 1;
				DescriptorRange.BaseShaderRegister					= CurrentParameter.Register;
				DescriptorRange.RegisterSpace						= 0;
				DescriptorRange.OffsetInDescriptorsFromTableStart	= D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
				DescriptorRanges.push_back(DescriptorRange);
			}

			D3D12RootParameters[RootParameterIndex].ParameterType						= D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			D3D12RootParameters[RootParameterIndex].DescriptorTable.NumDescriptorRanges	= Parameter.size();
			//D3D12RootParameters[RootParameterIndex].DescriptorTable.pDescriptorRanges	= &DescriptorRanges[FirstElement];
			D3D12RootParameters[RootParameterIndex].ShaderVisibility					= D3D12_SHADER_VISIBILITY_ALL;

			DescriptorTableEntry NewForwardEntry = { &D3D12RootParameters[RootParameterIndex].DescriptorTable.pDescriptorRanges, FirstElement };
			ForwardEntryRecords.push_back(NewForwardEntry);
		}
	}

	for (uint32_t ForwardEntryIndex = 0; ForwardEntryIndex < ForwardEntryRecords.size(); ++ForwardEntryIndex)
	{
		const D3D12_DESCRIPTOR_RANGE**& DescriptorRangeToWrite	= ForwardEntryRecords[ForwardEntryIndex].DescriptorRange;
		uint32_t Entry											= ForwardEntryRecords[ForwardEntryIndex].DescriptorRangeEntry;
		*DescriptorRangeToWrite = &DescriptorRanges[Entry];
	}

	D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc;
	
	RootSignatureDesc.NumParameters		= D3D12RootParameters.size();
	RootSignatureDesc.pParameters		= D3D12RootParameters.data();
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

//D3D12RootSignature::D3D12RootSignature(_In_ Device& DeviceObj, _In_ Sampler* StaticSamplers[], _In_ uint32_t StaticSamplersCount, _In_ DescriptorHeap* DescriptorHeaps[], _In_ uint32_t DescriptorHeapsCount, _In_ const RootSignatureAccess& RootSignatureAccessObj)
//{
//	vector<D3D12_STATIC_SAMPLER_DESC> D3D12StaticSamplers;
//	D3D12StaticSamplers.resize(StaticSamplersCount);
//
//	for (uint32_t StaticSamplerIndex = 0; StaticSamplerIndex < StaticSamplersCount; ++StaticSamplerIndex)
//	{
//		static_cast<D3D12Sampler*>(StaticSamplers[StaticSamplerIndex])->GetD3D12StaticSampler(D3D12StaticSamplers[StaticSamplerIndex]);
//	}
//
//	vector<D3D12_ROOT_PARAMETER> D3D12RootParameters;
//	D3D12RootParameters.resize(DescriptorHeapsCount);
//
//	for (uint32_t RootParameterIndex = 0; RootParameterIndex < DescriptorHeapsCount; ++RootParameterIndex)
//	{
//		D3D12RootParameters[RootParameterIndex].ParameterType				= static_cast<D3D12DescriptorHeap*>(DescriptorHeaps[RootParameterIndex])->GetD3D12RootParameterType();
//		D3D12RootParameters[RootParameterIndex].Descriptor.RegisterSpace	= DescriptorHeaps[RootParameterIndex]->GetSpace();
//		D3D12RootParameters[RootParameterIndex].Descriptor.ShaderRegister	= DescriptorHeaps[RootParameterIndex]->GetRegister();
//		D3D12RootParameters[RootParameterIndex].ShaderVisibility			= D3D12_SHADER_VISIBILITY_ALL;
//	}
//
//	D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc;
//
//	RootSignatureDesc.NumParameters		= D3D12RootParameters.size();
//	RootSignatureDesc.pParameters		= D3D12RootParameters.data();
//	RootSignatureDesc.NumStaticSamplers	= D3D12StaticSamplers.size();
//	RootSignatureDesc.pStaticSamplers	= D3D12StaticSamplers.data();
//	RootSignatureDesc.Flags				= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
//
//	ID3DBlob* RootSignatureBlob = nullptr;
//	ID3DBlob* ErrorBlob = nullptr;
//	HRESULT hr = D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &RootSignatureBlob, &ErrorBlob);
//	ETERNAL_ASSERT(hr == S_OK);
//
//	hr = static_cast<D3D12Device&>(DeviceObj).GetD3D12Device()->CreateRootSignature(
//		DeviceObj.GetDeviceMask(),
//		RootSignatureBlob->GetBufferPointer(),
//		RootSignatureBlob->GetBufferSize(),
//		__uuidof(ID3D12RootSignature),
//		(void**)&_RootSignature
//	);
//	ETERNAL_ASSERT(hr == S_OK);
//}

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
