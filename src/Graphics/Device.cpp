#include "Graphics/Device.hpp"

using namespace Eternal::Graphics;

Device* Device::_inst = 0;

Device::Device()
{
	assert(!_inst);
	_inst = this;
}
Device* Device::Get()
{
	assert(_inst);
	return _inst;
}
