#ifndef _ROOT_SIGNATURE_FACTORY_HPP_
#define _ROOT_SIGNATURE_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class RootSignature;

		RootSignature* CreateRootSignature(_In_ Device& DeviceObj);
	}
}

#endif
