#include "d3d12/D3D12Resource.hpp"

using namespace Eternal::Graphics;

D3D12Resource::~D3D12Resource()
{
	_Resource->Release();
	_Resource = nullptr;
}
