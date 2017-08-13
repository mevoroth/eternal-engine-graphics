#include "Graphics/CommandList.hpp"
#include "Graphics/CommandAllocatorFactory.hpp"

using namespace Eternal::Graphics;

CommandList::CommandList(_In_ Device& DeviceObj, _In_ const CommandListType& Type)
{
	_CommandAllocator = CreateCommandAllocator(DeviceObj, Type);
}

CommandList::~CommandList()
{
	delete _CommandAllocator;
	_CommandAllocator = nullptr;
}
