#pragma once

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
			ResourceCreateInformation(_In_ Device& InDevice, _In_  const std::string& InName)
				: GfxDevice(InDevice)
				, Name(InName)
			{
			}

			Device& GfxDevice;
			std::string Name;
		};

		class Resource
		{
		public:
			Resource(_In_ const ResourceCreateInformation& InResourceCreateInformation, _In_ const ResourceSource& InResourceSource);
			virtual ~Resource() {}

		protected:
			inline ResourceCreateInformation& GetResourceCreateInformation() { return _ResourceCreateInformation; }
			inline ResourceSource GetResourceSource() const { return _ResourceSource; }

		private:
			ResourceSource				_ResourceSource = ResourceSource::UNKNOWN; // Used to track down type of resource
			ResourceCreateInformation	_ResourceCreateInformation;
		};
	}
}
