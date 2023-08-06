#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;
		class AccelerationStructure;
		struct BottomLevelAccelerationStructureCreateInformation;
		struct TopLevelAccelerationStructureCreateInformation;

		AccelerationStructure* CreateBottomLevelAccelerationStructure(_In_ GraphicsContext& InContext, _In_ const BottomLevelAccelerationStructureCreateInformation& InBottomLevelAccelerationStructureCreateInformation);
		AccelerationStructure* CreateTopLevelAccelerationStructure(_In_ GraphicsContext& InContext, _In_ const TopLevelAccelerationStructureCreateInformation& InTopLevelAccelerationStructureCreateInformation);
		void DestroyAccelerationStructure(_Inout_ AccelerationStructure*& InOutAccelerationStructure);
	}
}
