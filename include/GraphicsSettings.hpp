#ifndef _GRAPHICS_SETTINGS_HPP_
#define _GRAPHICS_SETTINGS_HPP_

#ifndef ETERNAL_USE_DXMATH_TYPES
#define ETERNAL_USE_DXMATH_TYPES
#endif

#define FRAME_LAG								(2)
#define MAX_VERTICES_BUFFERS					(8)
#define MAX_RENDER_TARGETS						(8)
#define UPLOAD_BUFFER_SIZE						(32768)

#define ETERNAL_USE_DEBUG_SHADERS				((ETERNAL_DEBUG && 1) || 1)

#define ETERNAL_DEBUG_MATCHING_ROOT_SIGNATURE	(ETERNAL_DEBUG_VERBOSE && 0)

#include <cstdint>
#include <vector>
#include "Macros/Macros.hpp"

// TODO: Rasterizer Ordered Views research

#endif
