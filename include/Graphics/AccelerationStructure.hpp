#pragma once

#include "Types/Types.hpp"
#include <vector>
#include <string>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		using namespace Eternal::Types;

		class GraphicsContext;
		class AccelerationStructure;
		class Resource;
		class View;

		struct AccelerationStructureGeometry
		{
			Resource* VertexBuffer		= nullptr;
			Resource* IndexBuffer		= nullptr;
			Resource* TransformBuffer	= nullptr;
			uint32_t IndicesCount		= 0u;
			uint32_t IndicesOffset		= 0u;
			uint32_t VerticesOffset		= 0u;
			uint32_t TransformsOffet	= 0u;
		};

		struct AccelerationStructureInstance
		{
			Matrix3x4 InstanceToWorld										= Matrix3x4::Identity;
			AccelerationStructure* BottomLevelAccelerationStructureBuffer	= nullptr;
		};

		struct AccelerationStructureCreateInformation
		{
			AccelerationStructureCreateInformation() = delete;

			string Name;

		protected:

			AccelerationStructureCreateInformation(_In_ const string& InName)
				: Name(InName)
			{
			}

		};

		struct BottomLevelAccelerationStructureCreateInformation : public AccelerationStructureCreateInformation
		{
			BottomLevelAccelerationStructureCreateInformation(_In_ const string& InName, _In_ const vector<AccelerationStructureGeometry>& InGeometries)
				: AccelerationStructureCreateInformation(InName)
				, Geometries(InGeometries)
			{
			}

			vector<AccelerationStructureGeometry> Geometries;
		};

		struct TopLevelAccelerationStructureCreateInformation : public AccelerationStructureCreateInformation
		{
			TopLevelAccelerationStructureCreateInformation(_In_ const string& InName)
				: AccelerationStructureCreateInformation(InName)
			{
			}
		};

		struct RebuildAccelerationStructureInput
		{
			vector<AccelerationStructureGeometry> Geometries;
			vector<AccelerationStructureInstance> Instances;
		};

		class AccelerationStructure
		{
		public:
			virtual ~AccelerationStructure();

			virtual void RebuildAccelerationStructure(_In_ GraphicsContext& InContext, _In_ const RebuildAccelerationStructureInput& InRebuildInput) = 0;
			Resource* GetAccelerationStructure() { return _AccelerationStructureResource; }
			View* GetView() { return _AccelerationStructureView; }
			const string& GetAccelerationStructureName() const { return _AccelerationStructureCreateInformation.Name; }

		protected:
			AccelerationStructure(_In_ const AccelerationStructureCreateInformation& InAccelerationStructureCreateInformation);

			AccelerationStructureCreateInformation _AccelerationStructureCreateInformation;
			Resource*	_AccelerationStructureResource	= nullptr;
			View*		_AccelerationStructureView		= nullptr;
		};

		class BottomLevelAccelerationStructure
		{
		public:
			BottomLevelAccelerationStructure(_In_ const BottomLevelAccelerationStructureCreateInformation& InBottomLevelAccelerationStructureCreateInformation);

			const vector<AccelerationStructureGeometry>& GetGeometries() const { return _Geometries; }

		private:
			vector<AccelerationStructureGeometry> _Geometries;
		};

		class TopLevelAccelerationStructure
		{
		public:
			TopLevelAccelerationStructure(_In_ const TopLevelAccelerationStructureCreateInformation& InTopLevelAccelerationStructureCreateInformation);

		protected:
			Resource* _InstancesBuffer = nullptr;
		};
	}
}
