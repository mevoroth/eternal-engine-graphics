#include "Graphics/Device.hpp"

using namespace Eternal::Graphics;

Device* Device::_Inst = 0;

//Device::Device()
//{
//	ETERNAL_ASSERT(!_Inst);
//	_Inst = this;
//}
Device* Device::Get()
{
	ETERNAL_ASSERT(_Inst);
	return _Inst;
}
