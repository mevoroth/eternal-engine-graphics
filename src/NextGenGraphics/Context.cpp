#include "NextGenGraphics/Context.hpp"

#include "Graphics/FenceFactory.hpp"

using namespace Eternal::Graphics;

Context::Context(_In_ Device& DeviceObj)
{
	_FrameFence = CreateFence(DeviceObj);
}

Context::~Context()
{
	delete _FrameFence;
	_FrameFence = nullptr;
}
