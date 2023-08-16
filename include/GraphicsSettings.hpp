#pragma once

#ifndef ETERNAL_USE_DXMATH_TYPES
#define ETERNAL_USE_DXMATH_TYPES				(1)
#endif

#define FRAME_LAG								(2)
#define MAX_VERTICES_BUFFERS					(8)
#define MAX_RENDER_TARGETS						(8)
#define MAX_RENDER_TARGETS_AND_DEPTH_STENCIL	(MAX_RENDER_TARGETS + 1)
#define UPLOAD_BUFFER_SIZE						(32768)

#define ETERNAL_USE_NVIDIA_AFTERMATH			(0)
#define ETERNAL_USE_DEBUG_LAYER					(ETERNAL_DEBUG && !ETERNAL_USE_NVIDIA_AFTERMATH && 1)
#define ETERNAL_USE_DEBUG_SHADERS				((ETERNAL_DEBUG && 1) || 1)
#define ETERNAL_USE_DEBUG_VERBOSE				(ETERNAL_DEBUG && 0)

#define ETERNAL_DEBUG_MATCHING_ROOT_SIGNATURE	(ETERNAL_DEBUG_VERBOSE && 0)
#define ETERNAL_DEBUG_COMMAND_LIST_SUBMIT		((ETERNAL_DEBUG_VERBOSE || 0) && 0)

#define USE_PIX									(1)

#include <cstdint>
#include <vector>
#include "Macros/Macros.hpp"

// TODO: Rasterizer Ordered Views research
