#include "d3d12/D3D12AccelerationStructure.hpp"

#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12Format.hpp"
#include "d3d12/D3D12Resource.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Resource.hpp"
#include "Graphics/ResourceFactory.hpp"
#include "Graphics/View.hpp"
#include "Graphics/ViewFactory.hpp"

namespace Eternal
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////
		// D3D12AccelerationStructure

		D3D12AccelerationStructure::D3D12AccelerationStructure(_In_ const AccelerationStructureCreateInformation& InAccelerationStructureCreateInformation)
			: AccelerationStructure(InAccelerationStructureCreateInformation)
		{
		}

		void D3D12AccelerationStructure::CreateAccelerationStructureBuffer(_In_ GraphicsContext& InContext)
		{
			ID3D12Device5* Device5 = static_cast<D3D12Device&>(InContext.GetDevice()).GetD3D12Device5();

			D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS BuildRayTracingAccelerationStructureInputs = {};
			GetD3D12BuildRaytracingAccelerationStructureInputs(BuildRayTracingAccelerationStructureInputs);

			D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO RayTracingAccelerationStructurePrebuildInfo = {};
			Device5->GetRaytracingAccelerationStructurePrebuildInfo(&BuildRayTracingAccelerationStructureInputs, &RayTracingAccelerationStructurePrebuildInfo);
			ETERNAL_ASSERT(RayTracingAccelerationStructurePrebuildInfo.ResultDataMaxSizeInBytes > 0);

			_AccelerationStructureResource = CreateBuffer(
				BufferResourceCreateInformation(
					InContext.GetDevice(),
					GetAccelerationStructureName(),
					AccelerationStructureBufferCreateInformation(static_cast<uint32_t>(RayTracingAccelerationStructurePrebuildInfo.ResultDataMaxSizeInBytes)),
					ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_MEMORY,
					TransitionState::TRANSITION_RAYTRACING_ACCELERATION_STRUCTURE
				)
			);
		}

		//////////////////////////////////////////////////////////////////////////
		// D3D12BottomLevelAccelerationStructure

		D3D12BottomLevelAccelerationStructure::D3D12BottomLevelAccelerationStructure(_In_ GraphicsContext& InContext, _In_ const BottomLevelAccelerationStructureCreateInformation& InBottomLevelAccelerationStructureCreateInformation)
			: BottomLevelAccelerationStructure(InBottomLevelAccelerationStructureCreateInformation)
			, D3D12AccelerationStructure(InBottomLevelAccelerationStructureCreateInformation)
		{
			const vector<AccelerationStructureGeometry>& InGeometries = GetGeometries();
			_D3D12Geometries.reserve(InGeometries.size());

			for (uint32_t GeometryIndex = 0; GeometryIndex < InGeometries.size(); ++GeometryIndex)
			{
				const AccelerationStructureGeometry& CurrentGeometry			= InGeometries[GeometryIndex];
				_D3D12Geometries.push_back({});

				D3D12_RAYTRACING_GEOMETRY_DESC& CurrentGeometryDescription		= _D3D12Geometries.back();
				CurrentGeometryDescription.Type									= D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
				CurrentGeometryDescription.Flags								= D3D12_RAYTRACING_GEOMETRY_FLAG_OPAQUE;
				CurrentGeometryDescription.Triangles.Transform3x4				= static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(0);
				CurrentGeometryDescription.Triangles.IndexFormat				= ConvertFormatToD3D12Format(CurrentGeometry.IndexBuffer->GetFormat()).Format;
				CurrentGeometryDescription.Triangles.VertexFormat				= DXGI_FORMAT_R32G32B32_FLOAT;//ConvertFormatToD3D12Format(CurrentGeometry.VertexBuffer->GetFormat()).Format;
				CurrentGeometryDescription.Triangles.IndexCount					= CurrentGeometry.IndexBuffer->GetElementCount();
				CurrentGeometryDescription.Triangles.VertexCount				= CurrentGeometry.VertexBuffer->GetElementCount();
				CurrentGeometryDescription.Triangles.IndexBuffer				= static_cast<const D3D12Resource*>(CurrentGeometry.IndexBuffer)->GetD3D12Resource()->GetGPUVirtualAddress();
				CurrentGeometryDescription.Triangles.VertexBuffer.StartAddress	= static_cast<const D3D12Resource*>(CurrentGeometry.VertexBuffer)->GetD3D12Resource()->GetGPUVirtualAddress();
				CurrentGeometryDescription.Triangles.VertexBuffer.StrideInBytes	= CurrentGeometry.VertexBuffer->GetBufferStride();
			}

			CreateAccelerationStructureBuffer(InContext);
		}

		void D3D12BottomLevelAccelerationStructure::RebuildAccelerationStructure(_In_ GraphicsContext& InContext, _In_ const RebuildAccelerationStructureInput& InRebuildInput)
		{
			ETERNAL_BREAK();
		}

		void D3D12BottomLevelAccelerationStructure::GetD3D12BuildRaytracingAccelerationStructureInputs(_Out_ D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& OutInputs)
		{
			OutInputs					= {};
			OutInputs.Type				= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL;
			OutInputs.Flags				= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
			OutInputs.NumDescs			= static_cast<UINT>(_D3D12Geometries.size());
			OutInputs.DescsLayout		= D3D12_ELEMENTS_LAYOUT_ARRAY;
			OutInputs.pGeometryDescs	= _D3D12Geometries.data();
		}

		//////////////////////////////////////////////////////////////////////////
		// D3D12ToplevelAccelerationStructure

		D3D12TopLevelAccelerationStructure::D3D12TopLevelAccelerationStructure(_In_ const TopLevelAccelerationStructureCreateInformation& InTopLevelAccelerationStructureCreateInformation)
			: TopLevelAccelerationStructure(InTopLevelAccelerationStructureCreateInformation)
			, D3D12AccelerationStructure(InTopLevelAccelerationStructureCreateInformation)
		{
		}

		void D3D12TopLevelAccelerationStructure::RebuildAccelerationStructure(_In_ GraphicsContext& InContext, _In_ const RebuildAccelerationStructureInput& InRebuildInput)
		{
			_D3D12Instances.resize(InRebuildInput.Instances.size());
			
			for (uint32_t InstanceIndex = 0; InstanceIndex < InRebuildInput.Instances.size(); ++InstanceIndex)
			{
				_D3D12Instances[InstanceIndex]							= {};
				memcpy(_D3D12Instances[InstanceIndex].Transform, InRebuildInput.Instances[InstanceIndex].InstanceToWorld.m, sizeof(float) * 3 * 4);
				_D3D12Instances[InstanceIndex].InstanceMask				= 0x1;
				_D3D12Instances[InstanceIndex].AccelerationStructure	= static_cast<D3D12Resource*>(InRebuildInput.Instances[InstanceIndex].BottomLevelAccelerationStructureBuffer->GetAccelerationStructure())->GetD3D12Resource()->GetGPUVirtualAddress();
			}

			if (_InstancesBuffer)
				InContext.DelayedDelete(_InstancesBuffer);

			if (_AccelerationStructureView)
				DestroyView(_AccelerationStructureView);
			if (_AccelerationStructureResource)
				InContext.DelayedDelete(_AccelerationStructureResource);
			CreateAccelerationStructureBuffer(InContext);
			_AccelerationStructureView = CreateShaderResourceView(
				ShaderResourceViewAccelerationStructureCreateInformation(InContext, _AccelerationStructureResource)
			);

			_InstancesBuffer = CreateBuffer(
				BufferResourceCreateInformation(
					InContext.GetDevice(),
					GetAccelerationStructureName() + "_InstancesBuffer",
					BufferCreateInformation(
						Format::FORMAT_UNKNOWN,
						BufferResourceUsage::BUFFER_RESOURCE_USAGE_STRUCTURED_BUFFER,
						sizeof(D3D12_RAYTRACING_INSTANCE_DESC),
						static_cast<uint32_t>(_D3D12Instances.size())
					),
					ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_UPLOAD,
					TransitionState::TRANSITION_NON_PIXEL_SHADER_READ
				)
			);

			{
				MapScope<D3D12_RAYTRACING_INSTANCE_DESC> InstancesBufferMapScope(*_InstancesBuffer);
				memcpy(InstancesBufferMapScope.GetDataPointer(), _D3D12Instances.data(), _D3D12Instances.size() * sizeof(D3D12_RAYTRACING_INSTANCE_DESC));
			}
		}

		void D3D12TopLevelAccelerationStructure::GetD3D12BuildRaytracingAccelerationStructureInputs(_Out_ D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& OutInputs)
		{
			OutInputs				= {};
			OutInputs.Type			= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL;
			OutInputs.Flags			= D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAG_PREFER_FAST_TRACE;
			OutInputs.NumDescs		= static_cast<UINT>(_D3D12Instances.size());
			OutInputs.DescsLayout	= D3D12_ELEMENTS_LAYOUT_ARRAY;
			OutInputs.InstanceDescs	= _InstancesBuffer ? static_cast<D3D12Resource*>(_InstancesBuffer)->GetD3D12Resource()->GetGPUVirtualAddress() : static_cast<D3D12_GPU_VIRTUAL_ADDRESS>(0);
		}
	}
}
