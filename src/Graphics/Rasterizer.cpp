#include "Graphics/Rasterizer.hpp"

namespace Eternal
{
	namespace Graphics
	{
		const Rasterizer RasterizerDefault;
		const Rasterizer RasterizerClockWise(FrontFace::FRONT_FACE_CLOCKWISE);
		const Rasterizer RasterizerLine(FillMode::FILL_MODE_WIREFRAME);
	}
}
