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
			
			static constexpr D3D12_ROOT_PARAMETER1 DefaultRootParameter = {};
			static constexpr D3D12_DESCRIPTOR_RANGE1 DefaultDescriptorRange = {};

			const vector<RootSignatureConstants>& InConstants			= InRootSignatureCreateInformation.Constants;
			const vector<RootSignatureParameter>& InParameters			= InRootSignatureCreateInformation.Parameters;
			const vector<RootSignatureStaticSampler>& InStaticSamplers	= InRootSignatureCreateInformation.StaticSamplers;

			std::array<
				std::array<uint32_t, static_cast<int32_t>(D3D12RegisterType::D3D12_REGISTER_TYPE_COUNT)>,
				static_cast<int32_t>(ShaderType::SHADER_TYPE_COUNT)
			> RegisterIndices;
			for (uint32_t ShaderTypeIndex = 0; ShaderTypeIndex < RegisterIndices.size(); ++ShaderTypeIndex)
			{
				for (uint32_t RegisterIndex = 0; RegisterIndex < RegisterIndices[ShaderTypeIndex].size(); ++RegisterIndex)
					RegisterIndices[ShaderTypeIndex].fill(0);
			}
			
#ifdef ETERNAL_DEBUG
			std::array<uint32_t, static_cast<int32_t>(ShaderType::SHADER_TYPE_COUNT)> DebugRegisterCount;
			DebugRegisterCount.fill(0);
#endif

			D3D12_ROOT_SIGNATURE_FLAGS Flags = static_cast<D3D12_ROOT_SIGNATURE_FLAGS>(
				D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS
			);

			Flags |= InRootSignatureCreateInformation.HasInputAssembler ? D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT : D3D12_ROOT_SIGNATURE_FLAG_NONE;

			vector<D3D12_ROOT_PARAMETER1> Parameters;
			Parameters.reserve(InRootSignatureCreateInformation.Parameters.size() + InRootSignatureCreateInformation.Constants.size());
			vector<D3D12_STATIC_SAMPLER_DESC> StaticSamplers;
			StaticSamplers.resize(InRootSignatureCreateInformation.StaticSamplers.size());

			for (uint32_t ConstantsIndex = 0; ConstantsIndex < InConstants.size(); ++ConstantsIndex)
			{
				const RootSignatureAccess& CurrentAccess = InConstants[ConstantsIndex].Access;

				AllowStage(Flags, CurrentAccess);

				Parameters.push_back(DefaultRootParameter);
				D3D12_ROOT_PARAMETER1& CurrentRootParameter = Parameters.back();

				CurrentRootParameter.ParameterType				= D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
				CurrentRootParameter.Constants.ShaderRegister	= RegisterIndices[static_cast<int32_t>(CurrentAccess)][static_cast<int32_t>(D3D12RegisterType::D3D12_REGISTER_TYPE_CBV)]++;
				CurrentRootParameter.Constants.RegisterSpace	= 0;
				CurrentRootParameter.Constants.Num32BitValues	= InConstants[ConstantsIndex].Count;
				CurrentRootParameter.ShaderVisibility			= ConvertRootSignatureAccessToD3D12ShaderVisibility(CurrentAccess);

#ifdef ETERNAL_DEBUG
				++DebugRegisterCount[static_cast<int32_t>(CurrentAccess)];
#endif
			}

			vector<D3D12_DESCRIPTOR_RANGE1> DescriptorRanges;
			DescriptorRanges.reserve(DescriptorRangesMaxCount);


			for (uint32_t ParameterIndex = 0; ParameterIndex < InParameters.size(); ++ParameterIndex)
			{
				const RootSignatureAccess& CurrentAccess = InParameters[ParameterIndex].Access;

				AllowStage(Flags, CurrentAccess);

				Parameters.push_back(DefaultRootParameter);
				D3D12_ROOT_PARAMETER1& CurrentRootParameter = Parameters.back();

				std::array<uint32_t, static_cast<int32_t>(D3D12RegisterType::D3D12_REGISTER_TYPE_COUNT)>& RegisterIndexPerType = RegisterIndices[static_cast<int32_t>(CurrentAccess)];
#ifdef ETERNAL_DEBUG
				uint32_t& DebugRegisterCountType = DebugRegisterCount[static_cast<int32_t>(CurrentAccess)];
#endif
				uint32_t RegisterTypeUint = ConvertRootSignatureParameterTypeToD3D12RegisterTypeUInt(InParameters[ParameterIndex].Parameter);

				CurrentRootParameter.ParameterType	= ConvertRootSignatureParameterTypeToD3D12RootParameterType(InParameters[ParameterIndex].Parameter);
				switch (InParameters[ParameterIndex].Parameter)
				{
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_SAMPLER:
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_TEXTURE:
				{
					DescriptorRanges.push_back(DefaultDescriptorRange);
					D3D12_DESCRIPTOR_RANGE1& DescriptorRange = DescriptorRanges.back();
					DescriptorRange.RangeType							= ConvertRootSignatureParameterTypeToD3D12DescriptorRangeType(InParameters[ParameterIndex].Parameter);
					DescriptorRange.BaseShaderRegister					= RegisterIndexPerType[RegisterTypeUint];
					DescriptorRange.RegisterSpace						= 0;
					DescriptorRange.OffsetInDescriptorsFromTableStart	= D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
					DescriptorRange.Flags								= D3D12_DESCRIPTOR_RANGE_FLAG_NONE;

					const RootSignatureParameter& CurrentParameterGroup = InParameters[ParameterIndex];

					uint32_t NextParameterIndex = ParameterIndex + 1;
					while (NextParameterIndex < InParameters.size()
						&& InParameters[NextParameterIndex] == CurrentParameterGroup)
					{
						++NextParameterIndex;
					}

					const uint32_t NumDescriptors = NextParameterIndex - ParameterIndex;

					DescriptorRange.NumDescriptors						= NumDescriptors;
					
					CurrentRootParameter.DescriptorTable.NumDescriptorRanges	= 1;
					CurrentRootParameter.DescriptorTable.pDescriptorRanges		= &DescriptorRange;

					RegisterIndexPerType[RegisterTypeUint]	+= NumDescriptors;
#ifdef ETERNAL_DEBUG
					DebugRegisterCountType += NumDescriptors;
#endif
					ParameterIndex	= NextParameterIndex - 1;
				} break;

				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE:
				{
					const vector<RootSignatureDescriptorTableParameter>& DescriptorTableLayout = InParameters[ParameterIndex].DescriptorTable.Parameters;

					uint32_t DescriptorRangeFirst = static_cast<uint32_t>(DescriptorRanges.size());

					for (uint32_t DescriptorTableIndex = 0; DescriptorTableIndex < DescriptorTableLayout.size(); ++DescriptorTableIndex)
					{
						const RootSignatureParameterType& CurrentParameter = DescriptorTableLayout[DescriptorTableIndex].Parameter;
						uint32_t RegisterType = ConvertRootSignatureParameterTypeToD3D12RegisterTypeUInt(CurrentParameter);
						ETERNAL_ASSERT(CurrentParameter != RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE);

						DescriptorRanges.push_back(DefaultDescriptorRange);

						D3D12_DESCRIPTOR_RANGE1& CurrentDescriptorRange				= DescriptorRanges.back();
						CurrentDescriptorRange.RangeType							= ConvertRootSignatureParameterTypeToD3D12DescriptorRangeType(CurrentParameter);
						CurrentDescriptorRange.NumDescriptors						= CurrentParameter != RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_SAMPLER
																						? DescriptorTableLayout[DescriptorTableIndex].DescriptorsCount
																						: DescriptorTableLayout[DescriptorTableIndex].Samplers.size();
						CurrentDescriptorRange.BaseShaderRegister					= RegisterIndexPerType[RegisterType];
						CurrentDescriptorRange.RegisterSpace						= 0;
						CurrentDescriptorRange.Flags								= D3D12_DESCRIPTOR_RANGE_FLAG_NONE;
						CurrentDescriptorRange.OffsetInDescriptorsFromTableStart	= D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

						RegisterIndexPerType[RegisterType] += DescriptorTableLayout[DescriptorTableIndex].DescriptorsCount;
#ifdef ETERNAL_DEBUG
						DebugRegisterCountType += DescriptorTableLayout[DescriptorTableIndex].DescriptorsCount;
#endif
					}

					CurrentRootParameter.DescriptorTable.NumDescriptorRanges	= static_cast<UINT>(DescriptorRanges.size() - DescriptorRangeFirst);
					CurrentRootParameter.DescriptorTable.pDescriptorRanges		= &DescriptorRanges[DescriptorRangeFirst];
				} break;

				default:
				{
					CurrentRootParameter.Descriptor.ShaderRegister	= RegisterIndexPerType[ConvertRootSignatureParameterTypeToD3D12RegisterTypeUInt(InParameters[ParameterIndex].Parameter)]++;
					CurrentRootParameter.Descriptor.RegisterSpace	= 0;
#ifdef ETERNAL_DEBUG
					++DebugRegisterCountType;
#endif
				} break;
				}

				CurrentRootParameter.ShaderVisibility	= ConvertRootSignatureAccessToD3D12ShaderVisibility(CurrentAccess);
			}

			for (uint32_t StaticSamplerIndex = 0; StaticSamplerIndex < InStaticSamplers.size(); ++StaticSamplerIndex)
			{
				const RootSignatureAccess& CurrentAccess = InStaticSamplers[StaticSamplerIndex].Access;

				AllowStage(Flags, CurrentAccess);

				static_cast<D3D12Sampler*>(InStaticSamplers[StaticSamplerIndex].StaticSampler)->GetD3D12StaticSampler(StaticSamplers[StaticSamplerIndex]);
				StaticSamplers[StaticSamplerIndex].ShaderRegister	= RegisterIndices[static_cast<int32_t>(CurrentAccess)][static_cast<int32_t>(D3D12RegisterType::D3D12_REGISTER_TYPE_SAMPLER)]++;
				StaticSamplers[StaticSamplerIndex].RegisterSpace	= 0;
				StaticSamplers[StaticSamplerIndex].ShaderVisibility	= ConvertRootSignatureAccessToD3D12ShaderVisibility(CurrentAccess);

#if ETERNAL_DEBUG
				++DebugRegisterCount[static_cast<int32_t>(CurrentAccess)];
#endif
			}
			
			D3D12_VERSIONED_ROOT_SIGNATURE_DESC RootVersionedSignatureDesc;
			RootVersionedSignatureDesc.Version						= D3D_ROOT_SIGNATURE_VERSION_1_1;
			RootVersionedSignatureDesc.Desc_1_1.NumParameters		= static_cast<UINT>(Parameters.size());
			RootVersionedSignatureDesc.Desc_1_1.pParameters			= Parameters.data();
			RootVersionedSignatureDesc.Desc_1_1.NumStaticSamplers	= static_cast<UINT>(StaticSamplers.size());
			RootVersionedSignatureDesc.Desc_1_1.pStaticSamplers		= StaticSamplers.data();
			RootVersionedSignatureDesc.Desc_1_1.Flags				= Flags;

#ifdef ETERNAL_DEBUG
			// Check inconsistent root signature (graphics and compute at the same time)
			uint32_t GraphicsParameters = 0;
			uint32_t ComputeParameters = DebugRegisterCount[static_cast<int32_t>(ShaderType::CS)];
			for (uint32_t ShaderTypeIndex = static_cast<uint32_t>(ShaderType::VS); ShaderTypeIndex <= static_cast<uint32_t>(ShaderType::PS); ++ShaderTypeIndex)
			{
				GraphicsParameters += DebugRegisterCount[ShaderTypeIndex];
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
				hr = D3D12SerializeVersionedRootSignature(&RootVersionedSignatureDesc, &RootSignatureBlob, &ErrorBlob)
			);

			if (hr != S_OK)
			{
				std::string ErrorMessage(static_cast<char*>(ErrorBlob->GetBufferPointer()), ErrorBlob->GetBufferSize());
				LogWrite(LogError, LogGraphics, ErrorMessage);
				ETERNAL_BREAK();
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

			if (RootSignatureBlob)
			{
				RootSignatureBlob->Release();
				RootSignatureBlob = nullptr;
			}

			if (ErrorBlob)
			{
				ErrorBlob->Release();
				ErrorBlob = nullptr;
			}
		}

		D3D12RootSignature::~D3D12RootSignature()
		{
			_RootSignature->Release();
			_RootSignature = nullptr;
		}
	}
}
