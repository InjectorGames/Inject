#pragma once
#include "Injector/Graphics/Vulkan/VkGpuPipeline.hpp"
#include "Injector/Graphics/Pipeline/ColorGpuPipeline.hpp"
#include "Injector/Graphics/Vulkan/VkGpuShader.hpp"

namespace Injector
{
	class VkColorGpuPipeline :
		public VkGpuPipeline,
		public ColorGpuPipeline
	{
	 protected:
		vk::PipelineLayout pipelineLayout;
		vk::Pipeline pipeline;
		std::shared_ptr<VkGpuShader> vertexShader;
		std::shared_ptr<VkGpuShader> fragmentShader;

		Vector4 color;

		static vk::PipelineLayout createPipelineLayout(
			vk::Device device);
		static vk::Pipeline createPipeline(
			vk::Device device,
			vk::PipelineCache pipelineCache,
			vk::PipelineLayout pipelineLayout,
			vk::RenderPass renderPass,
			const vk::Extent2D& surfaceExtent,
			vk::PrimitiveTopology primitiveTopology,
			const std::shared_ptr<VkGpuShader>& vertexShader,
			const std::shared_ptr<VkGpuShader>& fragmentShader);
	 public:
		VkColorGpuPipeline(
			vk::Device device,
			vk::RenderPass renderPass,
			const vk::Extent2D& surfaceExtent,
			PrimitiveTopology primitiveTopology,
			const std::shared_ptr<VkGpuShader>& vertexShader,
			const std::shared_ptr<VkGpuShader>& fragmentShader,
			const Vector4& color = Vector4::one);
		~VkColorGpuPipeline() override;

		const Vector4& getColor() const override;
		void setColor(const Vector4& color) override;

		void recreate(
			VmaAllocator allocator,
			vk::RenderPass renderPass,
			uint32_t imageCount,
			const vk::Extent2D& surfaceExtent) override;
		void flush(
			size_t imageIndex) override;
		void bind(
			vk::CommandBuffer commandBuffer,
			size_t imageIndex) override;
		void setUniforms(
			const Matrix4& model,
			const Matrix4& view,
			const Matrix4& proj,
			const Matrix4& viewProj,
			const Matrix4& mvp) override;
	};
}
