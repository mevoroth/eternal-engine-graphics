#include "Device.hpp"

#include <cassert>

using namespace Eternal::Graphics;

template <class DeviceType> DeviceType* Device<DeviceType>::_inst = 0;

template <class DeviceType> Device<DeviceType>::Device(DeviceType* device)
{
	assert(!_inst);
	_inst = device;
}

template <class DeviceType> DeviceType* Device<DeviceType>::get()
{
	return _inst;
}
