#if ETERNAL_ENABLE_D3D12

#include "d3d12/D3D12ShaderTable.hpp"

#include "Graphics/Format.hpp"
#include "Graphics/ResourceFactory.hpp"
#include "d3d12/D3D12Pipeline.hpp"
#include "d3d12/D3D12Resource.hpp"
#include "d3d12/D3D12Shader.hpp"
#include "d3d12/D3D12Utils.hpp"
#include "Math/Math.hpp"
#include "Log/Log.hpp"
#include <sstream>
#include <iomanip>

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Graphics::D3D12;

		namespace D3D12
		{
			struct ShaderRecord
			{
				static constexpr uint32_t LocalRootArgumentsSize = 8;

				ShaderRecord(_In_ ID3D12StateObjectProperties* StateObjectProperties, _In_ Shader* InShader)
				{
					if (InShader)
						memcpy(ShaderIdentifier, StateObjectProperties->GetShaderIdentifier(static_cast<D3D12Shader*>(InShader)->GetD3D12StageEntryPoint()), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
				}

				ShaderRecord(_In_ ID3D12StateObjectProperties* StateObjectProperties)
				{
					memcpy(ShaderIdentifier, StateObjectProperties->GetShaderIdentifier(L"HitGroup"), D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
				}

				ShaderRecord() {}

				uint8_t ShaderIdentifier[D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES]	= {};
				uint8_t LocalRootArguments[LocalRootArgumentsSize]				= {};
			};

			D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE NullShaderTable = {};

			static const uint32_t RecordSize = []() -> uint32_t
			{
				return Math::Align<uint32_t>(sizeof(ShaderRecord), D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT);
			}();
		}

		D3D12ShaderTable::D3D12ShaderTable(_In_ Device& InDevice, _In_ Pipeline& InPipeline)
			: ShaderTable(InPipeline)
		{
			const PipelineCreateInformation& InPipelineCreateInformation = InPipeline.GetPipelineCreateInformation();

			ID3D12StateObjectProperties* StateObjectProperties = nullptr;
			VerifySuccess(
				static_cast<D3D12Pipeline&>(_Pipeline).GetD3D12StateObject()->QueryInterface(
					__uuidof(ID3D12StateObjectProperties),
					reinterpret_cast<void**>(&StateObjectProperties)
				)
			);

			std::string PipelineStateName	= "ShaderTable RayGeneration_" + string(InPipelineCreateInformation.ShaderRayTracingRayGeneration->GetName())
											+ (InPipelineCreateInformation.ShaderRayTracingClosestHit	? " ClosestHit_" + string(InPipelineCreateInformation.ShaderRayTracingClosestHit->GetName())	: string())
											+ (InPipelineCreateInformation.ShaderRayTracingMiss			? " Miss_" + string(InPipelineCreateInformation.ShaderRayTracingMiss->GetName())				: string())
											+ (InPipelineCreateInformation.ShaderRayTracingAnyHit		? " AnyHit_" + string(InPipelineCreateInformation.ShaderRayTracingAnyHit->GetName())			: string());

			ShaderRecord ShaderRecords[] =
			{
				ShaderRecord(StateObjectProperties, InPipelineCreateInformation.ShaderRayTracingRayGeneration),
				ShaderRecord(StateObjectProperties, InPipelineCreateInformation.ShaderRayTracingMiss),
				InPipelineCreateInformation.ShaderRayTracingClosestHit || InPipelineCreateInformation.ShaderRayTracingAnyHit ? ShaderRecord(StateObjectProperties) : ShaderRecord()
			};

			uint32_t TableSize	= RecordSize * ETERNAL_ARRAYSIZE(ShaderRecords);

			//for (uint32_t ShaderRecordIndex = 0; ShaderRecordIndex < ETERNAL_ARRAYSIZE(ShaderRecords); ++ShaderRecordIndex)
			//{
			//	stringstream LogShaderRecordStream;
			//	LogShaderRecordStream << "ShaderRecord : { ShaderIdentifier = 0x" << hex << setfill('0') << setw(16) << ShaderRecords[ShaderRecordIndex].ShaderIdentifier.Pointer << " }";
			//	LogWrite(LogError, LogGraphics, LogShaderRecordStream.str());
			//}

			_ShaderTable = CreateBuffer(
				BufferResourceCreateInformation(
					InDevice,
					PipelineStateName,
					BufferCreateInformation(
						Format::FORMAT_UNKNOWN,
						BufferResourceUsage::BUFFER_RESOURCE_USAGE_BUFFER,
						TableSize
					),
					ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_UPLOAD
				)
			);

			{
				MapRange ShaderTableMapRange(TableSize);
				MapScope<> ShaderTableMapScope(*_ShaderTable, ShaderTableMapRange);
				uint8_t* ShaderTablePointer = ShaderTableMapScope.GetDataPointer();
				for (uint32_t TableIndex = 0; TableIndex < ETERNAL_ARRAYSIZE(ShaderRecords); ++TableIndex)
				{
					ShaderRecord* CurrentRecord = reinterpret_cast<ShaderRecord*>(ShaderTablePointer);
					memcpy(CurrentRecord, &ShaderRecords[TableIndex], sizeof(ShaderRecord));
					ShaderTablePointer += RecordSize;
				}
			}

			StateObjectProperties->Release();
		}

		D3D12_GPU_VIRTUAL_ADDRESS_RANGE D3D12ShaderTable::GetD3D12RayGenerationShaderRecord() const
		{
			return { _GetD3D12GPUVirtualAddress(), RecordSize};
		}

		D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE D3D12ShaderTable::GetD3D12MissShaderTable() const
		{
			D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE MissShaderTable = { _GetD3D12GPUVirtualAddress() + RecordSize, RecordSize, RecordSize };
			return _Pipeline.GetPipelineCreateInformation().ShaderRayTracingMiss ? MissShaderTable : NullShaderTable;
		}

		D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE D3D12ShaderTable::GetD3D12HitGroupTable() const
		{
			uint32_t HitGroupTableSize = 0;
			HitGroupTableSize += _Pipeline.GetPipelineCreateInformation().ShaderRayTracingClosestHit ? 1 : 0;
			HitGroupTableSize += _Pipeline.GetPipelineCreateInformation().ShaderRayTracingAnyHit ? 1 : 0;
			D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE HitGroupShaderTable = { _GetD3D12GPUVirtualAddress() + RecordSize * 2, RecordSize * HitGroupTableSize, RecordSize };
			return HitGroupTableSize > 0 ? HitGroupShaderTable : NullShaderTable;
		}

		D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE D3D12ShaderTable::GetD3D12CallableShaderTable() const
		{
			return {};
		}

		D3D12_GPU_VIRTUAL_ADDRESS D3D12ShaderTable::_GetD3D12GPUVirtualAddress() const
		{
			return static_cast<D3D12Resource*>(_ShaderTable)->GetD3D12Resource()->GetGPUVirtualAddress();
		}
	}
}

#endif
