#include "d3d12/D3D12ShaderTable.hpp"

#include "Graphics/Format.hpp"
#include "Graphics/ResourceFactory.hpp"
#include "d3d12/D3D12Pipeline.hpp"
#include "d3d12/D3D12Resource.hpp"
#include "d3d12/D3D12Shader.hpp"
#include "d3d12/D3D12Utils.hpp"
#include "Math/Math.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Graphics::D3D12;

		namespace GraphicsPrivate
		{
			struct ShaderRecord
			{
				ShaderRecord(_In_ ID3D12StateObjectProperties* StateObjectProperties, _In_ Shader* InShader)
				{
					ShaderIdentifier.Pointer	= InShader ? StateObjectProperties->GetShaderIdentifier(static_cast<D3D12Shader*>(InShader)->GetD3D12StageEntryPoint()) : nullptr;
					ShaderIdentifier.Size		= InShader ? D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES : 0u;
				}

				ShaderRecord(_In_ ID3D12StateObjectProperties* StateObjectProperties)
				{
					ShaderIdentifier.Pointer	= StateObjectProperties->GetShaderIdentifier(L"HitGroup");
					ShaderIdentifier.Size		= D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;
				}

				ShaderRecord() {}

				struct PointerWithSize
				{
					void* Pointer	= nullptr;
					uint32_t Size	= 0u;
				};

				PointerWithSize ShaderIdentifier;
				PointerWithSize LocalRootArguments;
			};

			D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE NullShaderTable = {};
		}

		D3D12ShaderTable::D3D12ShaderTable(_In_ Device& InDevice, _In_ Pipeline& InPipeline)
			: ShaderTable(InPipeline)
		{
			using namespace Eternal::Graphics::GraphicsPrivate;
			
			uint32_t RecordSize = Math::Align<uint32_t>(sizeof(ShaderRecord), D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT);

			const PipelineCreateInformation& InPipelineCreateInformation = InPipeline.GetPipelineCreateInformation();

			ID3D12StateObjectProperties* StateObjectProperties = nullptr;
			VerifySuccess(
				static_cast<D3D12Pipeline&>(_Pipeline).GetD3D12StateObject()->QueryInterface(
					__uuidof(ID3D12StateObjectProperties),
					reinterpret_cast<void**>(&StateObjectProperties)
				)
			);

			std::string PipelineStateName	= "RayGeneration_" + string(InPipelineCreateInformation.ShaderRayTracingRayGeneration->GetName())
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

			auto CreateShaderTableInternal = [&InDevice, &PipelineStateName, &RecordSize](Resource*& ShaderTable, ShaderRecord* ShaderTableRecord)
			{
				ShaderTable = CreateBuffer(
					BufferResourceCreateInformation(
						InDevice,
						PipelineStateName,
						BufferCreateInformation(
							Format::FORMAT_UNKNOWN,
							BufferResourceUsage::BUFFER_RESOURCE_USAGE_BUFFER,
							RecordSize
						),
						ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_UPLOAD
					)
				);

				{
					MapRange ShaderTableMapRange(RecordSize);
					MapScope<> ShaderTableMapScope(*ShaderTable, ShaderTableMapRange);
					ShaderRecord* ShaderTablePointer = reinterpret_cast<ShaderRecord*>(ShaderTableMapScope.GetDataPointer());
					memcpy(ShaderTablePointer, ShaderTableRecord, sizeof(ShaderRecord));
					//for (uint32_t TableIndex = 0; TableIndex < ETERNAL_ARRAYSIZE(ShaderTable); ++TableIndex)
					//{
					//	ShaderRecord* CurrentRecord = reinterpret_cast<ShaderRecord*>(ShaderTablePointer);
					//	if (ShaderTable[TableIndex].ShaderIdentifier.Size > 0)
					//		memcpy(CurrentRecord, &ShaderTable[TableIndex], sizeof(ShaderRecord));
					//	else
					//		memset(CurrentRecord, 0x0, sizeof(ShaderRecord));
					//	ShaderTablePointer += RecordSize;
					//}
				}
			};

			CreateShaderTableInternal(_ShaderTable, &ShaderRecords[0]);
			CreateShaderTableInternal(_ShaderTable1, &ShaderRecords[1]);
			CreateShaderTableInternal(_ShaderTable2, &ShaderRecords[2]);

			//_ShaderTable = CreateBuffer(
			//	BufferResourceCreateInformation(
			//		InDevice,
			//		PipelineStateName,
			//		BufferCreateInformation(
			//			Format::FORMAT_UNKNOWN,
			//			BufferResourceUsage::BUFFER_RESOURCE_USAGE_BUFFER,
			//			TableSize
			//		),
			//		ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_UPLOAD
			//	)
			//);

			//{
			//	MapRange ShaderTableMapRange(TableSize);
			//	MapScope<> ShaderTableMapScope(*_ShaderTable, ShaderTableMapRange);
			//	uint8_t* ShaderTablePointer = ShaderTableMapScope.GetDataPointer();
			//	for (uint32_t TableIndex = 0; TableIndex < ETERNAL_ARRAYSIZE(ShaderTable); ++TableIndex)
			//	{
			//		ShaderRecord* CurrentRecord = reinterpret_cast<ShaderRecord*>(ShaderTablePointer);
			//		if (ShaderTable[TableIndex].ShaderIdentifier.Size > 0)
			//			memcpy(CurrentRecord, &ShaderTable[TableIndex], sizeof(ShaderRecord));
			//		else
			//			memset(CurrentRecord, 0x0, sizeof(ShaderRecord));
			//		ShaderTablePointer += RecordSize;
			//	}
			//}

			StateObjectProperties->Release();
		}

		D3D12_GPU_VIRTUAL_ADDRESS_RANGE D3D12ShaderTable::GetD3D12RayGenerationShaderRecord() const
		{
			uint32_t RecordSize = Math::Align<uint32_t>(sizeof(GraphicsPrivate::ShaderRecord), D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT);

			return { _GetD3D12GPUVirtualAddress(), RecordSize};
		}

		D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE D3D12ShaderTable::GetD3D12MissShaderTable() const
		{
			uint32_t RecordSize = Math::Align<uint32_t>(sizeof(GraphicsPrivate::ShaderRecord), D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT);
			//D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE MissShaderTable = { _GetD3D12GPUVirtualAddress() + RecordSize, RecordSize, RecordSize };
			D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE MissShaderTable = { static_cast<D3D12Resource*>(_ShaderTable1)->GetD3D12Resource()->GetGPUVirtualAddress(), RecordSize, RecordSize };
			return _Pipeline.GetPipelineCreateInformation().ShaderRayTracingMiss ? MissShaderTable : GraphicsPrivate::NullShaderTable;
		}

		D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE D3D12ShaderTable::GetD3D12HitGroupTable() const
		{
			uint32_t RecordSize = Math::Align<uint32_t>(sizeof(GraphicsPrivate::ShaderRecord), D3D12_RAYTRACING_SHADER_TABLE_BYTE_ALIGNMENT);

			uint32_t HitGroupTableSize = 0;
			HitGroupTableSize += _Pipeline.GetPipelineCreateInformation().ShaderRayTracingClosestHit ? 1 : 0;
			HitGroupTableSize += _Pipeline.GetPipelineCreateInformation().ShaderRayTracingAnyHit ? 1 : 0;
			//D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE HitGroupShaderTable = { _GetD3D12GPUVirtualAddress() + RecordSize * 2, RecordSize * HitGroupTableSize, RecordSize };
			D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE HitGroupShaderTable = { static_cast<D3D12Resource*>(_ShaderTable2)->GetD3D12Resource()->GetGPUVirtualAddress(), RecordSize * HitGroupTableSize, RecordSize };
			return HitGroupTableSize > 0 ? HitGroupShaderTable : GraphicsPrivate::NullShaderTable;
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
