#ifndef _CONTEXT_FACTORY_HPP_
#define _CONTEXT_FACTORY_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Context;
		class Device;

		Context* CreateContext(_In_ Device& DeviceObj);
	}
}

#endif
