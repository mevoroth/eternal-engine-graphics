#include "Graphics/Renderer.hpp"

using namespace Eternal::Graphics;

Renderer* Renderer::_inst = 0;

Renderer::Renderer(_In_ const RenderMode& mode, _In_ const AntiAliasing& aa)
	: _mode(mode)
	, _aa(aa)
	, _matrix(NewIdentity())
{
	_inst = this;
}

void Renderer::_SetBackBuffer(RenderTarget* backBuffer)
{
	_backBuffer = backBuffer;
}

RenderTarget* Renderer::GetBackBuffer() const
{
	return _backBuffer;
}

void Renderer::SetViewport(_In_ Viewport* viewport)
{
	viewport->Apply();
}

void Renderer::PushContext()
{
	_contexts.Push(_matrix);
}

void Renderer::PopContext()
{
	_matrix = _contexts.Head();
	_contexts.Pop();
}

void Renderer::LoadMatrix(const Matrix4x4& mat)
{
	_matrix = mat;
}

void Renderer::MulMatrix(const Matrix4x4& mat)
{
	_matrix = mat * _matrix;
}

Renderer* Renderer::Get()
{
	assert(_inst);
	return _inst;
}
