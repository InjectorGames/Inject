#pragma once
#include "Injector/Graphics/GpuImageType.hpp"
#include "Injector/Graphics/GpuImageWrap.hpp"
#include "Injector/Graphics/GpuImageFormat.hpp"
#include "Injector/Graphics/GpuImageFilter.hpp"
#include "Injector/Mathematics/IntVector3.hpp"

namespace Injector
{
	class GpuImage
	{
	 protected:
		GpuImageType type;
		IntVector3 size;
		GpuImageFormat format;
		GpuImageFilter minFilter;
		GpuImageFilter magFilter;
		GpuImageWrap wrapU;
		GpuImageWrap wrapV;
		GpuImageWrap wrapW;
		bool useMipmap;
	 public:
		GpuImage(
			GpuImageType type,
			IntVector3 size,
			GpuImageFormat format,
			GpuImageFilter minFilter,
			GpuImageFilter magFilter,
			GpuImageWrap wrapU,
			GpuImageWrap wrapV,
			GpuImageWrap wrapW,
			bool useMipmap);
		virtual ~GpuImage();

		GpuImageType getType() const noexcept;
		const IntVector3& getSize() const noexcept;
		GpuImageFormat getFormat() const noexcept;
		GpuImageFilter getMinFilter() const noexcept;
		GpuImageFilter getMagFilter() const noexcept;
		GpuImageWrap getWrapU() const noexcept;
		GpuImageWrap getWrapV() const noexcept;
		GpuImageWrap getWrapW() const noexcept;
		bool isUseMipmap() const noexcept;
	};
}