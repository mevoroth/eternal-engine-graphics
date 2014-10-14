#ifndef _VERTEX_HPP_
#define _VERTEX_HPP_

#include "GraphicsSettings.hpp"
#include "Types/Types.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace Types;
		class Vertex
		{
		public:
			Vector4 Pos;
			Vector2 Tex;
		};
	}
}

#endif
