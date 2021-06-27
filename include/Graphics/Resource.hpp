#pragma once

#include "Graphics/CommandUtils.h"
#include <string>

namespace Eternal
{
	namespace Graphics
	{
		class Device;

		enum class ResourceSource
		{
			UNKNOWN,
			BACK_BUFFER
		};

		struct ResourceCreateInformation
		{
			ResourceCreateInformation(_In_ Device& InDevice, _In_  const std::string& InName, _In_ const TransitionState& InInitialTransitionState)
				: GfxDevice(InDevice)
				, Name(InName)
				, ResourceState(InInitialTransitionState)
			{
			}

			Device&			GfxDevice;
			std::string		Name;
			TransitionState	ResourceState	= TransitionState::TRANSITION_UNDEFINED;
		};

		class Resource
		{
		public:
			Resource(_In_ const ResourceCreateInformation& InResourceCreateInformation, _In_ const ResourceSource& InResourceSource);
			virtual ~Resource() {}

			void SetResourceState(const TransitionState& InTransitionState) { _ResourceCreateInformation.ResourceState = InTransitionState; }
			inline bool IsMultisample() const { return _Multisample; }
			inline const TransitionState& GetResourceState() const { return _ResourceCreateInformation.ResourceState; }

		protected:
			inline ResourceCreateInformation& GetResourceCreateInformation() { return _ResourceCreateInformation; }
			inline ResourceSource GetResourceSource() const { return _ResourceSource; }

		private:
			ResourceSource				_ResourceSource = ResourceSource::UNKNOWN; // Used to track down type of resource
			ResourceCreateInformation	_ResourceCreateInformation;
			bool						_Multisample = false;
		};
	}
}
