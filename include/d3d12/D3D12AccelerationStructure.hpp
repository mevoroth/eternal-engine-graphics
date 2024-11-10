#pragma once

#include "Graphics/AccelerationStructure.hpp"

#if ETERNAL_ENABLE_D3D12

#include "d3d12/D3D12Library.h"
#include <string>

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;

		class D3D12AccelerationStructure : public AccelerationStructure
		{
		public:
			D3D12AccelerationStructure(_In_ const AccelerationStructureCreateInformation& InAccelerationStructureCreateInformation);

			virtual void GetD3D12BuildRaytracingAccelerationStructureInputs(_Out_ D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& OutInputs) = 0;

		protected:

			void CreateAccelerationStructureBuffer(_In_ GraphicsContext& InContext);
		};

		class D3D12BottomLevelAccelerationStructure
			: public BottomLevelAccelerationStructure
			, public D3D12AccelerationStructure
		{
		public:
			D3D12BottomLevelAccelerationStructure(_In_ GraphicsContext& InContext, _In_ const BottomLevelAccelerationStructureCreateInformation& InBottomLevelAccelerationStructureCreateInformation);

			virtual void RebuildAccelerationStructure(_In_ GraphicsContext& InContext, _In_ const RebuildAccelerationStructureInput& InRebuildInput) override final;
			virtual void GetD3D12BuildRaytracingAccelerationStructureInputs(_Out_ D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& OutInputs) override final;

		private:
			vector<D3D12_RAYTRACING_GEOMETRY_DESC> _D3D12Geometries;
		};

		class D3D12TopLevelAccelerationStructure
			: public TopLevelAccelerationStructure
			, public D3D12AccelerationStructure
		{
		public:
			D3D12TopLevelAccelerationStructure(_In_ const TopLevelAccelerationStructureCreateInformation& InTopLevelAccelerationStructureCreateInformation);

			virtual void RebuildAccelerationStructure(_In_ GraphicsContext& InContext, _In_ const RebuildAccelerationStructureInput& InRebuildInput) override final;
			virtual void GetD3D12BuildRaytracingAccelerationStructureInputs(_Out_ D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS& OutInputs) override final;

		private:
			vector<D3D12_RAYTRACING_INSTANCE_DESC> _D3D12Instances;
		};
	}
}

#endif
