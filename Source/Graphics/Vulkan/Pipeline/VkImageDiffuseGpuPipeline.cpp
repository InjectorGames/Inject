#include "Injector/Graphics/Vulkan/Pipeline/VkImageDiffuseGpuPipeline.hpp"
#include "Injector/Exception/NullException.hpp"

namespace Injector
{
	vk::DescriptorSetLayout VkImageDiffuseGpuPipeline::createDescriptorSetLayout(
		vk::Device device)
	{
		vk::DescriptorSetLayout descriptorSetLayout;

		auto descriptorSetLayoutBinding = vk::DescriptorSetLayoutBinding(
			0,
			vk::DescriptorType::eUniformBuffer,
			1,
			vk::ShaderStageFlagBits::eFragment,
			nullptr);
		auto descriptorSetLayoutCreateInfo = vk::DescriptorSetLayoutCreateInfo(
			vk::DescriptorSetLayoutCreateFlags(),
			1,
			&descriptorSetLayoutBinding);

		auto result = device.createDescriptorSetLayout(
			&descriptorSetLayoutCreateInfo,
			nullptr,
			&descriptorSetLayout);

		if(result != vk::Result::eSuccess)
		{
			throw Exception(
				"VkImageDiffuseGpuPipeline",
				"createDescriptorSetLayout",
				"Failed to create descriptor set layout");
		}

		return descriptorSetLayout;
	}
	vk::PipelineLayout VkImageDiffuseGpuPipeline::createPipelineLayout(
		vk::Device device,
		vk::DescriptorSetLayout descriptorSetLayout)
	{
		vk::PipelineLayout pipelineLayout;

		auto pushConstantRange = vk::PushConstantRange(
			vk::ShaderStageFlagBits::eVertex,
			0,
			sizeof(Matrix4) + sizeof(Matrix3));

		auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo(
			vk::PipelineLayoutCreateFlags(),
			1,
			&descriptorSetLayout,
			1,
			&pushConstantRange);

		auto result = device.createPipelineLayout(
			&pipelineLayoutCreateInfo,
			nullptr,
			&pipelineLayout);

		if (result != vk::Result::eSuccess)
		{
			throw Exception(
				"VkImageDiffuseGpuPipeline",
				"createPipelineLayout",
				"Failed to create pipeline layout");
		}

		return pipelineLayout;
	}
	vk::Pipeline VkImageDiffuseGpuPipeline::createPipeline(
		vk::Device device,
		vk::PipelineCache pipelineCache,
		vk::PipelineLayout pipelineLayout,
		vk::RenderPass renderPass,
		const vk::Extent2D& surfaceExtent,
		vk::PrimitiveTopology primitiveTopology,
		const std::shared_ptr<VkGpuShader>& vertexShader,
		const std::shared_ptr<VkGpuShader>& fragmentShader)
	{
		auto pipelineShaderStageCreateInfos = std::vector<vk::PipelineShaderStageCreateInfo>{
			vk::PipelineShaderStageCreateInfo(
				vk::PipelineShaderStageCreateFlags(),
				vk::ShaderStageFlagBits::eVertex,
				vertexShader->getShaderModule(),
				"main",
				nullptr),
			vk::PipelineShaderStageCreateInfo(
				vk::PipelineShaderStageCreateFlags(),
				vk::ShaderStageFlagBits::eFragment,
				fragmentShader->getShaderModule(),
				"main",
				nullptr),
		};

		auto vertexInputBindingDescription = vk::VertexInputBindingDescription(
			0,
			sizeof(Vector3) * 2,
			vk::VertexInputRate::eVertex);

		auto vertexInputAttributeDescriptions = std::vector<vk::VertexInputAttributeDescription>{
			vk::VertexInputAttributeDescription(
				0,
				0,
				vk::Format::eR32G32B32Sfloat,
				0),
			vk::VertexInputAttributeDescription(
				1,
				0,
				vk::Format::eR32G32B32Sfloat,
				sizeof(Vector3)),
		};

		auto pipelineVertexInputStateCreateInfo = vk::PipelineVertexInputStateCreateInfo(
			vk::PipelineVertexInputStateCreateFlags(),
			1,
			&vertexInputBindingDescription,
			static_cast<uint32_t>(vertexInputAttributeDescriptions.size()),
			vertexInputAttributeDescriptions.data());

		auto pipelineInputAssemblyStateCreateInfo = vk::PipelineInputAssemblyStateCreateInfo(
			vk::PipelineInputAssemblyStateCreateFlags(),
			primitiveTopology,
			false);

		auto viewport = vk::Viewport(
			0.0f,
			0.0f,
			static_cast<float>(surfaceExtent.width),
			static_cast<float>(surfaceExtent.height),
			0.0f,
			1.0f);
		auto scissor = vk::Rect2D(
			vk::Offset2D(0, 0),
			surfaceExtent);
		auto pipelineViewportStateCreateInfo = vk::PipelineViewportStateCreateInfo(
			vk::PipelineViewportStateCreateFlags(),
			1,
			&viewport,
			1,
			&scissor);

		auto pipelineRasterizationStateCreateInfo = vk::PipelineRasterizationStateCreateInfo(
			vk::PipelineRasterizationStateCreateFlags(),
			false,
			false,
			vk::PolygonMode::eFill,
			vk::CullModeFlagBits::eBack,
			vk::FrontFace::eClockwise,
			false,
			0.0f,
			0.0f,
			0.0f,
			1.0f);

		auto pipelineMultisampleStateCreateInfo = vk::PipelineMultisampleStateCreateInfo(
			vk::PipelineMultisampleStateCreateFlags(),
			vk::SampleCountFlagBits::e1,
			false,
			{},
			{},
			false,
			false);

		auto pipelineColorBlendAttachmentStateCreateInfo = vk::PipelineColorBlendAttachmentState(
			false,
			vk::BlendFactor(),
			vk::BlendFactor(),
			vk::BlendOp(),
			vk::BlendFactor(),
			vk::BlendFactor(),
			vk::BlendOp(),
			vk::ColorComponentFlagBits::eR |
			vk::ColorComponentFlagBits::eG |
			vk::ColorComponentFlagBits::eB |
			vk::ColorComponentFlagBits::eA);

		auto pipelineColorBlendStateCreateInfo = vk::PipelineColorBlendStateCreateInfo(
			vk::PipelineColorBlendStateCreateFlags(),
			false,
			vk::LogicOp(),
			1,
			&pipelineColorBlendAttachmentStateCreateInfo);

		auto graphicsPipelineCreateInfo = vk::GraphicsPipelineCreateInfo(
			vk::PipelineCreateFlags(),
			static_cast<uint32_t>(pipelineShaderStageCreateInfos.size()),
			pipelineShaderStageCreateInfos.data(),
			&pipelineVertexInputStateCreateInfo,
			&pipelineInputAssemblyStateCreateInfo,
			nullptr,
			&pipelineViewportStateCreateInfo,
			&pipelineRasterizationStateCreateInfo,
			&pipelineMultisampleStateCreateInfo,
			nullptr,
			&pipelineColorBlendStateCreateInfo,
			nullptr,
			pipelineLayout,
			renderPass,
			0,
			nullptr,
			-1);

		auto resultValue = device.createGraphicsPipeline(
			pipelineCache,
			graphicsPipelineCreateInfo);

		if (resultValue.result != vk::Result::eSuccess)
		{
			throw Exception(
				"VkImageDiffuseGpuPipeline",
				"createPipeline",
				"Failed to create pipeline");
		}

		return resultValue.value;
	}
	vk::DescriptorPool VkImageDiffuseGpuPipeline::createDescriptorPool(
		vk::Device device,
		uint32_t imageCount)
	{
		vk::DescriptorPool descriptorPool;

		auto descriptorPoolSize = vk::DescriptorPoolSize(
			vk::DescriptorType::eUniformBuffer,
			imageCount);
		auto descriptorPoolCreateInfo = vk::DescriptorPoolCreateInfo(
			vk::DescriptorPoolCreateFlags(),
			imageCount,
			1,
			&descriptorPoolSize);

		auto result = device.createDescriptorPool(
			&descriptorPoolCreateInfo,
			nullptr,
			&descriptorPool);

		if (result != vk::Result::eSuccess)
		{
			throw Exception(
				"VkImageDiffuseGpuPipeline",
				"createDescriptorPool",
				"Failed to create descriptor pool");
		}

		return descriptorPool;
	}
	vk::ImageView VkImageDiffuseGpuPipeline::createImageView(
		vk::Device device,
		vk::Image image)
	{
		vk::ImageView imageView;

		auto imageViewCreateInfo = vk::ImageViewCreateInfo(
			vk::ImageViewCreateFlags(),
			image,
			vk::ImageViewType::e2D,
			vk::Format::eR8G8B8A8Srgb,
			vk::ComponentMapping(),
			vk::ImageSubresourceRange(
				vk::ImageAspectFlagBits::eColor,
				0,
				1,
				0,
				1));

		auto result = device.createImageView(
			&imageViewCreateInfo,
			nullptr,
			&imageView);

		if(result != vk::Result::eSuccess)
		{
			throw Exception(
				"VkImageDiffuseGpuPipeline",
				"createImageView",
				"Failed to create image view");
		}

		return imageView;
	}
	vk::Sampler VkImageDiffuseGpuPipeline::createImageSampler(
		vk::Device,
		vk::Filter magFilter,
		vk::Filter minFilter,
		)
	{
		vk::Sampler sampler;

		auto samplerCreateInfo = vk::SamplerCreateInfo(
			vk::SamplerCreateFlags(),

			);
	}
	std::vector<std::shared_ptr<VkGpuBuffer>> VkImageDiffuseGpuPipeline::createUniformBuffers(
		VmaAllocator allocator,
		uint32_t imageCount)
	{
		auto uniformBuffers = std::vector<std::shared_ptr<VkGpuBuffer>>(imageCount);

		for (size_t i = 0; i < imageCount; i++)
		{
			uniformBuffers[i] = std::make_shared<VkGpuBuffer>(
				allocator,
				static_cast<vk::BufferUsageFlags>(0),
				VMA_MEMORY_USAGE_CPU_TO_GPU,
				GpuBufferType::Uniform,
				sizeof(UniformBufferObject));
		}

		return uniformBuffers;
	}
	std::vector<vk::DescriptorSet> VkImageDiffuseGpuPipeline::createDescriptorSets(
		vk::Device device,
		vk::DescriptorPool descriptorPool,
		vk::DescriptorSetLayout descriptorSetLayout,
		size_t imageCount,
		const std::vector<std::shared_ptr<VkGpuBuffer>>& uniformBuffers)
	{
		auto descriptorSetLayouts = std::vector<vk::DescriptorSetLayout>(
			imageCount,
			descriptorSetLayout);
		auto descriptorSetAllocateInfo = vk::DescriptorSetAllocateInfo(
			descriptorPool,
			static_cast<uint32_t>(imageCount),
			descriptorSetLayouts.data());

		auto descriptorSets = std::vector<vk::DescriptorSet>(imageCount);

		auto result = device.allocateDescriptorSets(
			&descriptorSetAllocateInfo,
			descriptorSets.data());

		if (result != vk::Result::eSuccess)
		{
			throw Exception(
				"VkImageDiffuseGpuPipeline",
				"createDescriptorSets",
				"Failed to allocate descriptor sets");
		}

		for (size_t i = 0; i < imageCount; i++)
		{
			auto descriptorBufferInfo = vk::DescriptorBufferInfo(
				uniformBuffers[i]->getBuffer(),
				0,
				sizeof(Vector4) * 3 + sizeof(Vector3));
			auto writeDescriptorSet = vk::WriteDescriptorSet(
				descriptorSets[i],
				0,
				0,
				1,
				vk::DescriptorType::eUniformBuffer,
				nullptr,
				&descriptorBufferInfo,
				nullptr);

			device.updateDescriptorSets(
				1,
				&writeDescriptorSet,
				0,
				nullptr);
		}

		return descriptorSets;
	}

	VkImageDiffuseGpuPipeline::VkImageDiffuseGpuPipeline(
		vk::Device device,
		VmaAllocator allocator,
		vk::RenderPass renderPass,
		uint32_t imageCount,
		const vk::Extent2D& surfaceExtent,
		PrimitiveTopology primitiveTopology,
		const std::shared_ptr<VkGpuShader>& _vertexShader,
		const std::shared_ptr<VkGpuShader>& _fragmentShader,
		const std::shared_ptr<VkGpuImage>& _image,
		const UniformBufferObject& _ubo) :
		VkGpuPipeline(device, primitiveTopology),
		vertexShader(_vertexShader),
		fragmentShader(_fragmentShader),
		image(_image),
		ubo(_ubo)
	{
		if(!allocator)
		{
			throw NullException(
				"VkImageDiffuseGpuPipeline",
				"VkImageDiffuseGpuPipeline",
				"allocator");
		}
		if(!renderPass)
		{
			throw NullException(
				"VkImageDiffuseGpuPipeline",
				"VkImageDiffuseGpuPipeline",
				"renderPass");
		}
		if (!_vertexShader)
		{
			throw NullException(
				"VkImageDiffuseGpuPipeline",
				"VkImageDiffuseGpuPipeline",
				"vertexShader");
		}
		if (!_fragmentShader)
		{
			throw NullException(
				"VkImageDiffuseGpuPipeline",
				"VkImageDiffuseGpuPipeline",
				"fragmentShader");
		}
		if (!_image)
		{
			throw NullException(
				"VkImageDiffuseGpuPipeline",
				"VkImageDiffuseGpuPipeline",
				"image");
		}

		descriptorSetLayout = createDescriptorSetLayout(
			device);
		pipelineLayout = createPipelineLayout(
			device,
			descriptorSetLayout);
		pipeline = createPipeline(
			device,
			pipelineCache,
			pipelineLayout,
			renderPass,
			surfaceExtent,
			toVkPrimitiveTopology(primitiveTopology),
			_vertexShader,
			_fragmentShader);
		descriptorPool = createDescriptorPool(
			device,
			imageCount);
		imageView = createImageView(
			device,
			_image->getImage());
		uniformBuffers = createUniformBuffers(
			allocator,
			imageCount);
		descriptorSets = createDescriptorSets(
			device,
			descriptorPool,
			descriptorSetLayout,
			imageCount,
			uniformBuffers);
	}
	VkImageDiffuseGpuPipeline::~VkImageDiffuseGpuPipeline()
	{
		device.destroySampler(
			imageSampler);
		device.destroyImageView(
			imageView);
		device.destroyDescriptorPool(
			descriptorPool);
		device.destroyPipeline(
			pipeline);
		device.destroyPipelineLayout(
			pipelineLayout);
		device.destroyDescriptorSetLayout(
			descriptorSetLayout);
	}

	void VkImageDiffuseGpuPipeline::recreate(
		VmaAllocator allocator,
		vk::RenderPass renderPass,
		uint32_t imageCount,
		const vk::Extent2D& extent)
	{
		device.destroyDescriptorPool(
			descriptorPool);
		device.destroyPipeline(
			pipeline);

		pipeline = createPipeline(
			device,
			pipelineCache,
			pipelineLayout,
			renderPass,
			extent,
			toVkPrimitiveTopology(primitiveTopology),
			vertexShader,
			fragmentShader);
		descriptorPool = createDescriptorPool(
			device,
			imageCount);
		imageView = createImageView(
			device,
			image->getImage());
		uniformBuffers = createUniformBuffers(
			allocator,
			imageCount);
		descriptorSets = createDescriptorSets(
			device,
			descriptorPool,
			descriptorSetLayout,
			imageCount,
			uniformBuffers);
	}
	void VkImageDiffuseGpuPipeline::flush(
		size_t imageIndex)
	{
		auto uniformBuffer = uniformBuffers[imageIndex];
		auto mappedData = uniformBuffer->map(GpuBufferAccess::WriteOnly);
		memcpy(mappedData, &ubo, sizeof(UniformBufferObject));
		uniformBuffer->unmap();
	}
	void VkImageDiffuseGpuPipeline::bind(
		vk::CommandBuffer commandBuffer,
		size_t imageIndex)
	{
		VkGpuPipeline::bind(commandBuffer, imageIndex);

		commandBuffer.bindPipeline(
			vk::PipelineBindPoint::eGraphics,
			pipeline);
		commandBuffer.bindDescriptorSets(
			vk::PipelineBindPoint::eGraphics,
			pipelineLayout,
			0,
			1,
			&descriptorSets[imageIndex],
			0,
			nullptr);
	}

	void VkImageDiffuseGpuPipeline::setUniforms(
		const Matrix4& model,
		const Matrix4& view,
		const Matrix4& proj,
		const Matrix4& viewProj,
		const Matrix4& mvp)
	{
		auto normal = model.getInverted().getInverted().getMatrix3();

		bindedCommandBuffer.pushConstants(
			pipelineLayout,
			vk::ShaderStageFlagBits::eVertex,
			0,
			sizeof(Matrix4),
			&mvp);
		bindedCommandBuffer.pushConstants(
			pipelineLayout,
			vk::ShaderStageFlagBits::eVertex,
			sizeof(Matrix4),
			sizeof(Matrix3),
			&normal);
	}

	const Vector4& VkImageDiffuseGpuPipeline::getObjectColor() const
	{
		return ubo.objectColor;
	}
	void VkImageDiffuseGpuPipeline::setObjectColor(const Vector4& color)
	{
		ubo.objectColor = Vector4(color);
	}

	const Vector4& VkImageDiffuseGpuPipeline::getAmbientColor() const
	{
		return ubo.ambientColor;
	}
	void VkImageDiffuseGpuPipeline::setAmbientColor(const Vector4& color)
	{
		ubo.ambientColor = Vector4(color);
	}

	const Vector4& VkImageDiffuseGpuPipeline::getLightColor() const
	{
		return ubo.lightColor;
	}
	void VkImageDiffuseGpuPipeline::setLightColor(const Vector4& color)
	{
		ubo.lightColor = Vector4(color);
	}

	const Vector3& VkImageDiffuseGpuPipeline::getLightDirection() const
	{
		return ubo.lightDirection;
	}
	void VkImageDiffuseGpuPipeline::setLightDirection(const Vector3& direction)
	{
		ubo.lightDirection = Vector3(direction.getNormalized());
	}

	const Vector2& VkImageDiffuseGpuPipeline::getImageScale() const
	{
		return ubo.imageScale;
	}
	void VkImageDiffuseGpuPipeline::setImageScale(const Vector2& scale)
	{
		ubo.imageScale = Vector2(scale);
	}

	const Vector2& VkImageDiffuseGpuPipeline::getImageOffset() const
	{
		return ubo.imageOffset;
	}
	void VkImageDiffuseGpuPipeline::setImageOffset(const Vector2& offset)
	{
		ubo.imageOffset = Vector2(offset);
	}
}
