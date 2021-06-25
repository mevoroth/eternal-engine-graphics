#include "d3d12/D3D12Sampler.hpp"

#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12GraphicsContext.hpp"

namespace Eternal
{
	namespace Graphics
	{
		D3D12_TEXTURE_ADDRESS_MODE ConvertAdressModeToD3D12TextureAdressMode(const AddressMode& InAddressMode)
		{
			return static_cast<D3D12_TEXTURE_ADDRESS_MODE>(static_cast<int32_t>(InAddressMode) + 1);
		}

		D3D12Sampler::D3D12Sampler(_In_ GraphicsContext& InContext, _In_ const SamplerCreateInformation& InSamplerCreateInformation)
			: Sampler(InSamplerCreateInformation)
		{
			if (!InSamplerCreateInformation.IsStatic)
			{
				D3D12_SAMPLER_DESC SamplerDesc;

				SamplerDesc.Filter			= GetD3D12Filering();
				SamplerDesc.AddressU		= GetD3D12U();
				SamplerDesc.AddressV		= GetD3D12V();
				SamplerDesc.AddressW		= GetD3D12W();
				SamplerDesc.MipLODBias		= GetMipLODBias();
				SamplerDesc.MaxAnisotropy	= GetMaxAnisotropy();
				SamplerDesc.ComparisonFunc	= D3D12::ConvertComparisonFunctionToD3D12ComparisonFunction(GetComparisonFunction());
				D3D12::ConvertBorderColorToFloats(GetBorderColor(), SamplerDesc.BorderColor);
				SamplerDesc.MinLOD			= GetMinLOD();
				SamplerDesc.MaxLOD			= GetMaxLOD();

				_D3D12Handle.D3D12CPUDescriptorHandle = static_cast<D3D12GraphicsContext&>(InContext).AllocateSamplerDescriptor(_D3D12Handle.DescriptorHandle);
				static_cast<D3D12Device&>(InContext.GetDevice()).GetD3D12Device()->CreateSampler(
					&SamplerDesc,
					_D3D12Handle.D3D12CPUDescriptorHandle
				);
			}
		}

		void D3D12Sampler::GetD3D12StaticSampler(_Out_ D3D12_STATIC_SAMPLER_DESC& StaticSamplerDesc) const
		{
			ETERNAL_ASSERT(IsStatic());

			StaticSamplerDesc.Filter			= GetD3D12Filering();
			StaticSamplerDesc.AddressU			= GetD3D12U();
			StaticSamplerDesc.AddressV			= GetD3D12V();
			StaticSamplerDesc.AddressW			= GetD3D12W();
			StaticSamplerDesc.MipLODBias		= GetMipLODBias();
			StaticSamplerDesc.MaxAnisotropy		= GetMaxAnisotropy();
			StaticSamplerDesc.ComparisonFunc	= D3D12::ConvertComparisonFunctionToD3D12ComparisonFunction(GetComparisonFunction());
			StaticSamplerDesc.BorderColor		= D3D12::ConvertBorderColorToD3D12StaticBorderColor(GetBorderColor());
			StaticSamplerDesc.MinLOD			= GetMinLOD();
			StaticSamplerDesc.MaxLOD			= GetMaxLOD();
		}

		D3D12_FILTER D3D12Sampler::GetD3D12Filering() const
		{
			// We're not supporting min max filtering yet due to Vulkan

			D3D12_FILTER Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;

			Filter = static_cast<D3D12_FILTER>(Filter | (IsComparisonEnabled() ? D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT : D3D12_FILTER_MIN_MAG_MIP_POINT));
			Filter = static_cast<D3D12_FILTER>(Filter | (IsAnisotropic() ? D3D12_FILTER_ANISOTROPIC : D3D12_FILTER_MIN_MAG_MIP_POINT));
			Filter = static_cast<D3D12_FILTER>(Filter | (GetMINLinear() ? D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT : D3D12_FILTER_MIN_MAG_MIP_POINT));
			Filter = static_cast<D3D12_FILTER>(Filter | (GetMAGLinear() ? D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT : D3D12_FILTER_MIN_MAG_MIP_POINT));
			Filter = static_cast<D3D12_FILTER>(Filter | (GetMIPLinear() ? D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR : D3D12_FILTER_MIN_MAG_MIP_POINT));

			return Filter;
		}

		D3D12_TEXTURE_ADDRESS_MODE D3D12Sampler::GetD3D12U() const
		{
			return ConvertAdressModeToD3D12TextureAdressMode(GetU());
		}

		D3D12_TEXTURE_ADDRESS_MODE D3D12Sampler::GetD3D12V() const
		{
			return ConvertAdressModeToD3D12TextureAdressMode(GetV());
		}

		D3D12_TEXTURE_ADDRESS_MODE D3D12Sampler::GetD3D12W() const
		{
			return ConvertAdressModeToD3D12TextureAdressMode(GetW());
		}
	}
}

