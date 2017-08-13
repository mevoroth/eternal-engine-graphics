#include "Graphics/DescriptorHeap.hpp"

#include <cstdint>
#include <cstring>
#include "Macros/Macros.hpp"
#include "Graphics/RootSignature.hpp"

using namespace Eternal::Graphics;

DescriptorHeap::DescriptorHeap(_In_ const RootSignatureParameter Resources[], _In_ uint32_t ResourcesCount)
{
	ETERNAL_ASSERT(ResourcesCount > 0);
	ETERNAL_ASSERT(IsD3D12Compatible(Resources, ResourcesCount));
}

bool DescriptorHeap::IsD3D12Compatible(_In_ const RootSignatureParameter Resources[], _In_ uint32_t ResourcesCount)
{
	enum Type
	{
		SAMPLER,
		CBV_SRV_UAV,
		RTV,
		DSV,
		TYPE_COUNT
	};
	static const uint32_t RootSignatureParameterTypeToType[] =
	{
		SAMPLER,
		CBV_SRV_UAV,
		CBV_SRV_UAV,
		CBV_SRV_UAV,
		CBV_SRV_UAV,
		CBV_SRV_UAV,
		CBV_SRV_UAV,
		CBV_SRV_UAV,
		CBV_SRV_UAV,
		RTV,
		DSV
	};
	ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(RootSignatureParameterTypeToType) == ROOT_SIGNATURE_PARAMETER_COUNT, "Parameters Types declaration not complete");

	uint32_t TypeCount[TYPE_COUNT];
	memset(TypeCount, 0x0, sizeof(uint32_t) * TYPE_COUNT);

	for (uint32_t ResourceIndex = 0; ResourceIndex < ResourcesCount; ++ResourceIndex)
	{
		++TypeCount[RootSignatureParameterTypeToType[Resources[ResourceIndex].Parameter]];
	}
	
	uint32_t DifferentTypeCount = 0;
	for (uint32_t TypeIndex = 0; TypeIndex < TYPE_COUNT; ++TypeIndex)
	{
		if (TypeCount[TypeIndex])
			++DifferentTypeCount;
	}
	return DifferentTypeCount == 1;
}
