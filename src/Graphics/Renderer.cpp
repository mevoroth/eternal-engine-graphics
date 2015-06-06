#include "Graphics/Renderer.hpp"

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

//void Renderer::PushContext()
//{
//	_Contexts.Push(_Matrix);
//}
//
//void Renderer::PopContext()
//{
//	_Matrix = _Contexts.Head();
//	_Contexts.Pop();
//}
//
//void Renderer::LoadMatrix(const Matrix4x4& mat)
//{
//	_Matrix = mat;
//}
//
//void Renderer::MulMatrix(const Matrix4x4& Mat)
//{
//	XMMATRIX TempMatrix = XMLoadFloat4x4(&Mat) * XMLoadFloat4x4(&_Matrix);
//	XMStoreFloat4x4(&_Matrix, TempMatrix);
//}

Renderer* Renderer::Get()
{
	assert(_Inst);
	return _Inst;
}
