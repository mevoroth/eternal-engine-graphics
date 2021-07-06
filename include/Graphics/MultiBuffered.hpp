#pragma once

#include "Graphics/GraphicsContext.hpp"

namespace Eternal
{
	namespace Graphics
	{
		template<typename GraphicsResourceType, uint32_t ResourcesCount = GraphicsContext::FrameBufferingCount>
		class MultiBuffered
		{
		public:

			MultiBuffered()
				: _Context(*(GraphicsContext*)nullptr)
			{
			}

			template<typename ConstructionFunctor>
			MultiBuffered(_In_ GraphicsContext& InContext, ConstructionFunctor InConstructResourceFunction)
				: _Context(InContext)
			{
				for (uint32_t ResourceIndex = 0; ResourceIndex < ResourcesCount; ++ResourceIndex)
					_Resources[ResourceIndex] = InConstructResourceFunction();
			}

			~MultiBuffered()
			{
				for (uint32_t ResourceIndex = 0; ResourceIndex < ResourcesCount; ++ResourceIndex)
				{
					delete _Resources[ResourceIndex];
					_Resources[ResourceIndex] = nullptr;
				}
			}

			operator GraphicsResourceType&()
			{
				ETERNAL_ASSERT(&_Context != nullptr);
				return *_Resources[_Context.GetCurrentFrameIndex()];
			}

			operator GraphicsResourceType* ()
			{
				ETERNAL_ASSERT(&_Context != nullptr);
				return _Resources[_Context.GetCurrentFrameIndex()];
			}

			GraphicsResourceType* operator->()
			{
				ETERNAL_ASSERT(&_Context != nullptr);
				return _Resources[_Context.GetCurrentFrameIndex()];
			}

			GraphicsResourceType* operator[](size_t Index)
			{
				ETERNAL_ASSERT(_Resources[Index]);
				return _Resources[Index];
			}

			MultiBuffered& operator=(const MultiBuffered& Other)
			{
				if (this == &Other)
					return *this;

				_Context = std::move(Other._Context);
				for (uint32_t ResourceIndex = 0; ResourceIndex < ResourcesCount; ++ResourceIndex)
					_Resources[ResourceIndex] = std::move(Other._Resources[ResourceIndex]);

				return *this;
			}

		private:
			GraphicsContext&		_Context;
			GraphicsResourceType*	_Resources[ResourcesCount];
		};
	}
}
