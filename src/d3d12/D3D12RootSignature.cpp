#include "d3d12/D3D12RootSignature.hpp"

#include <string>
#include <vector>
#include <array>
#include <d3d12.h>
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12Utils.hpp"
#include "d3d12/D3D12Sampler.hpp"
#include "Graphics/ShaderType.hpp"
#include "Math/Math.hpp"
#include "Log/Log.hpp"

namespace Eternal
{
	namespace Graphics
	{
		void AllowStage(D3D12_ROOT_SIGNATURE_FLAGS& Flags, const RootSignatureAccess& InAccess)
		{
			if (InAccess == RootSignatureAccess::ROOT_SIGNATURE_ACCESS_CS)
				return;

			ETERNAL_ASSERT(InAccess != RootSignatureAccess::ROOT_SIGNATURE_ACCESS_INVALID);
			Flags &= ~static_cast<D3D12_ROOT_SIGNATURE_FLAGS>(1 << (static_cast<uint32_t>(InAccess) + 1));
		}

		D3D12RootSignature::D3D12RootSignature(_In_ Device& InDevice)
		{
			using namespace Eternal::Graphics::D3D12;

			D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc;

			RootSignatureDesc.NumParameters		= 0;
			RootSignatureDesc.pParameters		= nullptr;
			RootSignatureDesc.NumStaticSamplers	= 0;
			RootSignatureDesc.pStaticSamplers	= nullptr;
			RootSignatureDesc.Flags				= D3D12_ROOT_SIGNATURE_FLAG_NONE;

			ID3DBlob* RootSignatureBlob	= nullptr;
			ID3DBlob* ErrorBlob			= nullptr;

			HRESULT hr;
			VerifySuccess(
				hr = D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &RootSignatureBlob, &ErrorBlob)
			);

			if (hr != S_OK)
			{
				std::string ErrorMessage(static_cast<char*>(ErrorBlob->GetBufferPointer()), ErrorBlob->GetBufferSize());
				LogWrite(LogError, LogGraphics, ErrorMessage);
				ETERNAL_BREAK();
			}

			{
				std::string RootSignatureContent(static_cast<char*>(RootSignatureBlob->GetBufferPointer()), RootSignatureBlob->GetBufferSize());
				LogWrite(LogInfo, LogGraphics, RootSignatureContent);
			}

			VerifySuccess(
				static_cast<D3D12Device&>(InDevice).GetD3D12Device()->CreateRootSignature(
					InDevice.GetDeviceMask(),
					RootSignatureBlob->GetBufferPointer(),
					RootSignatureBlob->GetBufferSize(),
					__uuidof(ID3D12RootSignature),
					(void**)&_RootSignature
				)
			);

			RootSignatureBlob->Release();
			ErrorBlob->Release();
		}

		D3D12RootSignature::D3D12RootSignature(_In_ Device& InDevice, _In_ const RootSignatureCreateInformation& InRootSignatureCreateInformation)
			: RootSignature(InRootSignatureCreateInformation)
		{
			using namespace Eternal::Graphics::D3D12;
			
			const vector<RootSignatureConstants>& InConstants			= InRootSignatureCreateInformation.Constants;
			const vector<RootSignatureParameter>& InParameters			= InRootSignatureCreateInformation.Parameters;
			const vector<RootSignatureStaticSampler>& InStaticSamplers	= InRootSignatureCreateInformation.StaticSamplers;

			std::array<uint32_t, static_cast<int32_t>(ShaderType::SHADER_TYPE_COUNT)> RegisterIndices;
			RegisterIndices.fill(0);

			D3D12_ROOT_SIGNATURE_FLAGS Flags = static_cast<D3D12_ROOT_SIGNATURE_FLAGS>(
				D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS
			);

			Flags |= InRootSignatureCreateInformation.HasInputAssembler ? D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT : D3D12_ROOT_SIGNATURE_FLAG_NONE;

			D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc;
			
			vector<D3D12_ROOT_PARAMETER> Parameters;
			Parameters.resize(InRootSignatureCreateInformation.Parameters.size() + InRootSignatureCreateInformation.Constants.size());
			vector<D3D12_STATIC_SAMPLER_DESC> StaticSamplers;
			StaticSamplers.resize(InRootSignatureCreateInformation.StaticSamplers.size());

			for (uint32_t ConstantsIndex = 0; ConstantsIndex < InConstants.size(); ++ConstantsIndex)
			{
				const RootSignatureAccess& CurrentAccess = InConstants[ConstantsIndex].Access;

				AllowStage(Flags, CurrentAccess);

				Parameters[ConstantsIndex].ParameterType				= D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
				Parameters[ConstantsIndex].Constants.ShaderRegister		= RegisterIndices[static_cast<int32_t>(CurrentAccess)]++;
				Parameters[ConstantsIndex].Constants.RegisterSpace		= 0;
				Parameters[ConstantsIndex].Constants.Num32BitValues		= InConstants[ConstantsIndex].Count;
				Parameters[ConstantsIndex].ShaderVisibility				= ConvertRootSignatureAccessToD3D12ShaderVisibility(CurrentAccess);
			}

			for (uint32_t ParameterIndex = 0; ParameterIndex < InParameters.size(); ++ParameterIndex)
			{
				const RootSignatureAccess& CurrentAccess = InParameters[ParameterIndex].Access;

				AllowStage(Flags, CurrentAccess);

				uint32_t& RegisterIndex = RegisterIndices[static_cast<int32_t>(CurrentAccess)];

				Parameters[ParameterIndex].ParameterType	= ConvertRootSignatureParameterTypeToD3D12RootParameterType(InParameters[ParameterIndex].Parameter);

				switch (InParameters[ParameterIndex].Parameter)
				{
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_SAMPLER:
				{
					D3D12_DESCRIPTOR_RANGE DescriptorRange;
					DescriptorRange.RangeType							= D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
					DescriptorRange.BaseShaderRegister					= RegisterIndex;
					DescriptorRange.RegisterSpace						= 0;
					DescriptorRange.OffsetInDescriptorsFromTableStart	= D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

					const RootSignatureParameter& CurrentSamplerParameter = InParameters[ParameterIndex];

					uint32_t NextParameterIndex = ParameterIndex + 1;
					while (NextParameterIndex < InParameters.size()
						&& InParameters[NextParameterIndex] == CurrentSamplerParameter)
					{
						++NextParameterIndex;
					}

					const uint32_t NumDescriptors = NextParameterIndex - ParameterIndex;

					DescriptorRange.NumDescriptors						= NumDescriptors;
					RegisterIndex	+= NumDescriptors;
					ParameterIndex	= NextParameterIndex - 1;
				} break;

				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE:
				{
					const vector<RootSignatureDescriptorTableParameter>& DescriptorTableLayout = InParameters[ParameterIndex].DescriptorTable.Parameters;

					vector<D3D12_DESCRIPTOR_RANGE> DescriptorRanges;
					DescriptorRanges.resize(DescriptorTableLayout.size());

					for (uint32_t DescriptorTableIndex = 0; DescriptorTableIndex < DescriptorTableLayout.size(); ++DescriptorTableIndex)
					{
						ETERNAL_ASSERT(DescriptorTableLayout[DescriptorTableIndex].Parameter != RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE);

						D3D12_DESCRIPTOR_RANGE& CurrentDescriptorRange				= DescriptorRanges[DescriptorTableIndex];
						CurrentDescriptorRange.RangeType							= ConvertRootSignatureParameterTypeToD3D12DescriptorRangeType(DescriptorTableLayout[DescriptorTableIndex].Parameter);
						CurrentDescriptorRange.NumDescriptors						= DescriptorTableLayout[DescriptorTableIndex].DescriptorCount;
						CurrentDescriptorRange.BaseShaderRegister					= RegisterIndex;
						CurrentDescriptorRange.RegisterSpace						= 0;
						CurrentDescriptorRange.OffsetInDescriptorsFromTableStart	= D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

						RegisterIndex += DescriptorTableLayout[DescriptorTableIndex].DescriptorCount;
					}

					Parameters[ParameterIndex].ParameterType						= D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
					Parameters[ParameterIndex].DescriptorTable.NumDescriptorRanges	= static_cast<UINT>(DescriptorRanges.size());
					Parameters[ParameterIndex].DescriptorTable.pDescriptorRanges	= DescriptorRanges.data();
				} break;

				default:
				{
					Parameters[ParameterIndex].Descriptor.ShaderRegister	= RegisterIndex++;
					Parameters[ParameterIndex].Descriptor.RegisterSpace		= 0;
				} break;
				}

				Parameters[ParameterIndex].ShaderVisibility	= ConvertRootSignatureAccessToD3D12ShaderVisibility(CurrentAccess);
			}

			for (uint32_t StaticSamplerIndex = 0; StaticSamplerIndex < InStaticSamplers.size(); ++StaticSamplerIndex)
			{
				const RootSignatureAccess& CurrentAccess = InStaticSamplers[StaticSamplerIndex].Access;

				AllowStage(Flags, CurrentAccess);

				static_cast<D3D12Sampler*>(InStaticSamplers[StaticSamplerIndex].StaticSampler)->GetD3D12StaticSampler(StaticSamplers[StaticSamplerIndex]);
				StaticSamplers[StaticSamplerIndex].ShaderRegister	= RegisterIndices[static_cast<int32_t>(CurrentAccess)]++;
				StaticSamplers[StaticSamplerIndex].RegisterSpace	= 0;
				StaticSamplers[StaticSamplerIndex].ShaderVisibility	= ConvertRootSignatureAccessToD3D12ShaderVisibility(CurrentAccess);
			}

			RootSignatureDesc.NumParameters		= static_cast<UINT>(Parameters.size());
			RootSignatureDesc.pParameters		= Parameters.data();
			RootSignatureDesc.NumStaticSamplers	= static_cast<UINT>(StaticSamplers.size());
			RootSignatureDesc.pStaticSamplers	= StaticSamplers.data();
			RootSignatureDesc.Flags				= Flags;

#ifdef ETERNAL_DEBUG
			// Check inconsistent root signature (graphics and compute at the same time)
			uint32_t GraphicsParameters = 0;
			uint32_t ComputeParameters = RegisterIndices[static_cast<int32_t>(ShaderType::CS)];
			for (uint32_t ShaderTypeIndex = static_cast<uint32_t>(ShaderType::VS); ShaderTypeIndex <= static_cast<uint32_t>(ShaderType::PS); ++ShaderTypeIndex)
			{
				GraphicsParameters += RegisterIndices[ShaderTypeIndex];
			}
			ETERNAL_ASSERT(
				(GraphicsParameters > 0 && ComputeParameters == 0) ||
				(GraphicsParameters == 0 && ComputeParameters > 0)
			);
#endif

			ID3DBlob* RootSignatureBlob = nullptr;
			ID3DBlob* ErrorBlob = nullptr;

			HRESULT hr;
			VerifySuccess(
				hr = D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &RootSignatureBlob, &ErrorBlob)
			);

			if (hr != S_OK)
			{
				std::string ErrorMessage(static_cast<char*>(ErrorBlob->GetBufferPointer()), ErrorBlob->GetBufferSize());
				LogWrite(LogError, LogGraphics, ErrorMessage);
				ETERNAL_BREAK();
			}

			{
				std::string RootSignatureContent(static_cast<char*>(RootSignatureBlob->GetBufferPointer()), RootSignatureBlob->GetBufferSize());
				LogWrite(LogInfo, LogGraphics, RootSignatureContent);
			}

			VerifySuccess(
				static_cast<D3D12Device&>(InDevice).GetD3D12Device()->CreateRootSignature(
					InDevice.GetDeviceMask(),
					RootSignatureBlob->GetBufferPointer(),
					RootSignatureBlob->GetBufferSize(),
					__uuidof(ID3D12RootSignature),
					(void**)&_RootSignature
				)
			);

			RootSignatureBlob->Release();
			ErrorBlob->Release();
		}

		D3D12RootSignature::~D3D12RootSignature()
		{
			_RootSignature->Release();
			_RootSignature = nullptr;
		}
	}
}
