#include "Graphics/Renderer.hpp"

#include "Macros/Macros.hpp"

using namespace Eternal::Graphics;

Renderer* Renderer::_Inst = 0;

Renderer::Renderer(_In_ const RenderMode& mode, _In_ const AntiAliasing& aa)
	: _Mode(mode)
	, _AA(aa)
{
	_Inst = this;
}

void Renderer::_SetBackBuffer(RenderTarget* BackBuffer)
{
	_BackBuffer = BackBuffer;
}

RenderTarget* Renderer::GetBackBuffer() const
{
	return _BackBuffer;
}

Renderer* Renderer::Get()
{
	ETERNAL_ASSERT(_Inst);
	return _Inst;
}
