#include "d3d12/D3D12RootSignature.hpp"

#include <string>
#include <vector>
#include <d3d12.h>
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12Utils.hpp"
#include "d3d12/D3D12Sampler.hpp"
#include "Math/Math.hpp"
#include "Log/Log.hpp"

namespace Eternal
{
	namespace Graphics
	{

		//const D3D12_ROOT_PARAMETER_TYPE D3D12_PARAMETER_TYPES[] =
		//{
		//	(D3D12_ROOT_PARAMETER_TYPE)-1,
		//	D3D12_ROOT_PARAMETER_TYPE_SRV,
		//	D3D12_ROOT_PARAMETER_TYPE_UAV,
		//	D3D12_ROOT_PARAMETER_TYPE_SRV,
		//	D3D12_ROOT_PARAMETER_TYPE_UAV,
		//	D3D12_ROOT_PARAMETER_TYPE_SRV,
		//	D3D12_ROOT_PARAMETER_TYPE_UAV,
		//	D3D12_ROOT_PARAMETER_TYPE_CBV,
		//	D3D12_ROOT_PARAMETER_TYPE_UAV,
		//	D3D12_ROOT_PARAMETER_TYPE_SRV,
		//	D3D12_ROOT_PARAMETER_TYPE_SRV,
		//	//D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE
		//};

		//ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_PARAMETER_TYPES) == int(RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_COUNT), "D3D12 Parameters Types declaration not complete");

		//const D3D12_DESCRIPTOR_RANGE_TYPE D3D12_PARAMETER_RANGE_TYPES[] =
		//{
		//	D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER,
		//	D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
		//	D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
		//	D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
		//	D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
		//	D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
		//	D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
		//	D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
		//	D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
		//	D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
		//	D3D12_DESCRIPTOR_RANGE_TYPE_SRV
		//};

		//ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_PARAMETER_RANGE_TYPES) == int(RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_COUNT), "D3D12 Parameter Range Types declaration not complete");

		//ROOT_SIGNATURE_PARAMETER_SAMPLER = 0,
		//	ROOT_SIGNATURE_PARAMETER_TEXTURE,
		//	ROOT_SIGNATURE_PARAMETER_RW_TEXTURE,
		//	ROOT_SIGNATURE_PARAMETER_STRUCTURED_BUFFER,
		//	ROOT_SIGNATURE_PARAMETER_RW_STRUCTURED_BUFFER,
		//	ROOT_SIGNATURE_PARAMETER_CONSTANT_BUFFER,
		//	ROOT_SIGNATURE_PARAMETER_BUFFER,
		//	ROOT_SIGNATURE_PARAMETER_RW_BUFFER,
		//	ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE,

					//D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE = 0,
			//	D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS = (D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE + 1),
			//	D3D12_ROOT_PARAMETER_TYPE_CBV = (D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS + 1),
			//	D3D12_ROOT_PARAMETER_TYPE_SRV = (D3D12_ROOT_PARAMETER_TYPE_CBV + 1),
			//	D3D12_ROOT_PARAMETER_TYPE_UAV = (D3D12_ROOT_PARAMETER_TYPE_SRV + 1)
		
		void AllowStage(D3D12_ROOT_SIGNATURE_FLAGS& Flags, const RootSignatureAccess& InAccess)
		{
			Flags &= ~static_cast<D3D12_ROOT_SIGNATURE_FLAGS>(1 << (static_cast<uint32_t>(InAccess) + 1u));
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

			D3D12_ROOT_SIGNATURE_FLAGS Flags = static_cast<D3D12_ROOT_SIGNATURE_FLAGS>(
				D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
				D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS
			);

			D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc;
			
			vector<D3D12_ROOT_PARAMETER> Parameters;
			Parameters.resize(InRootSignatureCreateInformation.Parameters.size());
			vector<D3D12_STATIC_SAMPLER_DESC> StaticSamplers;
			StaticSamplers.resize(InRootSignatureCreateInformation.StaticSamplers.size());

			uint32_t RegisterOffset = 0;

			const vector<RootSignatureParameter>& InParameters = InRootSignatureCreateInformation.Parameters;
			for (uint32_t ParameterIndex = 0; ParameterIndex < InParameters.size(); ++ParameterIndex)
			{
				AllowStage(Flags, InParameters[ParameterIndex].Access);

				Parameters[ParameterIndex].ParameterType	= ConvertRootSignatureParameterTypeToD3D12RootParameterType(InParameters[ParameterIndex].Parameter);

				switch (InParameters[ParameterIndex].Parameter)
				{
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_SAMPLER:
				{
					D3D12_DESCRIPTOR_RANGE DescriptorRange;
					DescriptorRange.RangeType							= D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
					DescriptorRange.BaseShaderRegister					= ParameterIndex + RegisterOffset;
					DescriptorRange.RegisterSpace						= 0;
					DescriptorRange.OffsetInDescriptorsFromTableStart	= D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

					const RootSignatureParameter& CurrentSamplerParameter = InParameters[ParameterIndex];

					uint32_t NextParameterIndex = ParameterIndex + 1;
					while (NextParameterIndex < InParameters.size()
						&& InParameters[NextParameterIndex] == CurrentSamplerParameter)
					{
						++NextParameterIndex;
					}

					DescriptorRange.NumDescriptors						= NextParameterIndex - ParameterIndex;
					ParameterIndex = NextParameterIndex - 1;
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
						CurrentDescriptorRange.BaseShaderRegister					= ParameterIndex + RegisterOffset;
						CurrentDescriptorRange.RegisterSpace						= 0;
						CurrentDescriptorRange.OffsetInDescriptorsFromTableStart	= D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

						RegisterOffset += DescriptorTableLayout[DescriptorTableIndex].DescriptorCount - 1;
					}
				} break;

				default:
				{
					Parameters[ParameterIndex].Descriptor.RegisterSpace		= 0;
					Parameters[ParameterIndex].Descriptor.ShaderRegister	= ParameterIndex + RegisterOffset;
				} break;
				}

				Parameters[ParameterIndex].ShaderVisibility	= ConvertRootSignatureAccessToD3D12ShaderVisibility(InParameters[ParameterIndex].Access);
			}

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
