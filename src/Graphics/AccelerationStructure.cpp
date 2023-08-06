#include "Graphics/AccelerationStructure.hpp"

#include "Graphics/ResourceFactory.hpp"

namespace Eternal
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////
		// AccelerationStructure

		AccelerationStructure::AccelerationStructure(_In_ const AccelerationStructureCreateInformation& InAccelerationStructureCreateInformation)
			: _AccelerationStructureCreateInformation(InAccelerationStructureCreateInformation)
		{
		}

		AccelerationStructure::~AccelerationStructure()
		{
			DestroyResource(_AccelerationStructureResource);
		}

		//////////////////////////////////////////////////////////////////////////
		// BottomLevelAccelerationStructure

		BottomLevelAccelerationStructure::BottomLevelAccelerationStructure(_In_ const BottomLevelAccelerationStructureCreateInformation& InBottomLevelAccelerationStructureCreateInformation)
			: _Geometries(InBottomLevelAccelerationStructureCreateInformation.Geometries)
		{
		}

		//////////////////////////////////////////////////////////////////////////
		// TopLevelAccelerationStructure
		
		TopLevelAccelerationStructure::TopLevelAccelerationStructure(_In_ const TopLevelAccelerationStructureCreateInformation& InTopLevelAccelerationStructureCreateInformation)
		{
		}
	}
}
