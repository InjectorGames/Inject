#pragma once
#include "Injector/Graphics/GpuMesh.hpp"
#include "Injector/Graphics/Vulkan/VkGpuBuffer.hpp"

#include <memory>

namespace Injector
{
	class VkGpuMesh : public GpuMesh
	{
	 public:
		VkGpuMesh(
			size_t indexCount,
			GpuBufferIndex indexType,
			const std::shared_ptr<GpuBuffer>& vertexBuffer,
			const std::shared_ptr<GpuBuffer>& indexBuffer);

		void draw(vk::CommandBuffer commandBuffer);
	};
}