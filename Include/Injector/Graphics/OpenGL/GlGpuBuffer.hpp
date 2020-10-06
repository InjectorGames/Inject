#pragma once
#include "Injector/Graphics/GpuBuffer.hpp"
#include "Injector/Graphics/GlfwDefines.hpp"

#include <cstdint>

namespace Injector
{
	class GlGpuBuffer : public GpuBuffer
	{
	 protected:
		GLuint buffer;
		GLenum glType;
	 public:
		GlGpuBuffer(
			GpuBufferType type,
			size_t size,
			GLenum usage,
			const void* data = 0);
		~GlGpuBuffer() override;

		GLuint getBuffer() const noexcept;
		GLenum getGlType() const noexcept;

		void bind() noexcept;
		void unbind() noexcept;

		void* map(
			GpuBufferAccess access =
				GpuBufferAccess::ReadWrite) override;
		void* map(
			GpuBufferAccess access,
			size_t size,
			size_t offset) override;
		void unmap() override;

		void setData(
			const void* data,
			size_t size) override;
		void setData(
			const void* data,
			size_t size,
			size_t offset) override;

		static GLenum toGlType(GpuBufferType type);
		static bool isGlMappable(GLenum usage);
		static GLbitfield toGlAccess(GpuBufferAccess access);
	};
}