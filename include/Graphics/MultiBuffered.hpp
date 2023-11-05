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
				: _Context(nullptr)
			{
			}

			template<typename ConstructionFunction>
			MultiBuffered(_In_ GraphicsContext& InContext, ConstructionFunction InConstructResourceFunctor)
				: _Context(&InContext)
			{
				for (uint32_t ResourceIndex = 0; ResourceIndex < ResourcesCount; ++ResourceIndex)
					_Resources[ResourceIndex] = InConstructResourceFunctor();
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
				ETERNAL_ASSERT(_Context != nullptr);
				return *_Resources[_Context->GetCurrentFrameIndex()];
			}

			operator GraphicsResourceType*()
			{
				ETERNAL_ASSERT(_Context != nullptr);
				return _Resources[_Context->GetCurrentFrameIndex()];
			}

			GraphicsResourceType* operator&()
			{
				return _Resources[_Context->GetCurrentFrameIndex()];
			}

			GraphicsResourceType* operator->()
			{
				ETERNAL_ASSERT(_Context != nullptr);
				return _Resources[_Context->GetCurrentFrameIndex()];
			}

			GraphicsResourceType* operator[](_In_ size_t InIndex)
			{
				ETERNAL_ASSERT(_Resources[InIndex]);
				return _Resources[InIndex];
			}

			MultiBuffered& operator=(_In_ const MultiBuffered& InOther)
			{
				if (this == &InOther)
					return *this;

				_Context = InOther._Context;
				for (uint32_t ResourceIndex = 0; ResourceIndex < ResourcesCount; ++ResourceIndex)
					_Resources[ResourceIndex] = InOther._Resources[ResourceIndex];

				return *this;
			}

		private:
			GraphicsContext*		_Context					= nullptr;
			GraphicsResourceType*	_Resources[ResourcesCount]	= { nullptr };
		};
	}
}
