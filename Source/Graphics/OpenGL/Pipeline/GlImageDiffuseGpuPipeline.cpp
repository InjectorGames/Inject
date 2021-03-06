#include "Injector/Graphics/OpenGL/Pipeline/GlImageDiffuseGpuPipeline.hpp"
#include "Injector/Exception/NullException.hpp"
#include "Injector/Graphics/OpenGL/GlGpuImageWrap.hpp"
#include "Injector/Graphics/OpenGL/GlGpuImageFilter.hpp"

namespace Injector
{
	GlImageDiffuseGpuPipeline::GlImageDiffuseGpuPipeline(
		GpuDrawMode drawMode,
		GpuImageFilter _imageMinFilter,
		GpuImageFilter _imageMagFilter,
		GpuImageFilter _mipmapFilter,
		GpuImageWrap _imageWrapU,
		GpuImageWrap _imageWrapV,
		GpuImageWrap _imageWrapW,
		const std::shared_ptr<GlGpuShader>& vertexShader,
		const std::shared_ptr<GlGpuShader>& fragmentShader,
		const std::shared_ptr<GlGpuImage>& _image,
		const UniformBufferObject& _ubo) :
		GlGpuPipeline(drawMode),
		imageMinFilter(_imageMinFilter),
		imageMagFilter(_imageMagFilter),
		mipmapFilter(_mipmapFilter),
		imageWrapU(_imageWrapU),
		imageWrapV(_imageWrapV),
		imageWrapW(_imageWrapW),
		ubo(_ubo),
		image(_image)
	{
		if (!vertexShader)
		{
			throw NullException(
				THIS_FUNCTION_NAME,
				"vertexShader");
		}
		if (!fragmentShader)
		{
			throw NullException(
				THIS_FUNCTION_NAME,
				"fragmentShader");
		}
		if (!_image)
		{
			throw NullException(
				THIS_FUNCTION_NAME,
				"image");
		}

		glImageMinFilter = toGlGpuImageFilter(
			_imageMinFilter,
			_image->isUseMipmap(),
			_mipmapFilter);
		glImageMagFilter = toGlGpuImageFilter(
			_imageMagFilter,
			false,
			_mipmapFilter);

		glImageWrapU = toGlGpuImageWrap(_imageWrapU);
		glImageWrapV = toGlGpuImageWrap(_imageWrapV);
		glImageWrapW = toGlGpuImageWrap(_imageWrapW);

		glAttachShader(
			program,
			vertexShader->getShader());
		glAttachShader(
			program,
			fragmentShader->getShader());

		glLinkProgram(program);

		glDetachShader(
			program,
			vertexShader->getShader());
		glDetachShader(
			program,
			fragmentShader->getShader());

		if (!getLinkStatus(program))
		{
			auto log = getInfoLog(program);
			glDeleteProgram(program);

			throw Exception(
				THIS_FUNCTION_NAME,
				"Failed to link program, " + log);
		}

		mvpLocation = getUniformLocation(
			program,
			"u_MVP");
		normalLocation = getUniformLocation(
			program,
			"u_Normal");

		auto uniformBlockIndex = getUniformBlockIndex(
			program,
			"FragmentBufferObject");
		glUniformBlockBinding(
			program,
			uniformBlockIndex,
			0);

		uniformBuffer = std::make_shared<GlGpuBuffer>(
			GpuBufferType::Uniform,
			sizeof(UniformBufferObject),
			GL_DYNAMIC_DRAW,
			nullptr);

		imageLocation = getUniformLocation(
			program,
			"u_Image");

		GlGpuPipeline::bind();
		glUniform1i(imageLocation, 0);
	}

	GpuImageFilter GlImageDiffuseGpuPipeline::getImageMinFilter() const
	{
		return imageMinFilter;
	}
	GpuImageFilter GlImageDiffuseGpuPipeline::getImageMagFilter() const
	{
		return imageMagFilter;
	}
	GpuImageFilter GlImageDiffuseGpuPipeline::getMipmapFilter() const
	{
		return mipmapFilter;
	}

	GpuImageWrap GlImageDiffuseGpuPipeline::getImageWrapU() const
	{
		return imageWrapU;
	}
	GpuImageWrap GlImageDiffuseGpuPipeline::getImageWrapV() const
	{
		return imageWrapV;
	}
	GpuImageWrap GlImageDiffuseGpuPipeline::getImageWrapW() const
	{
		return imageWrapW;
	}

	std::shared_ptr<GpuImage> GlImageDiffuseGpuPipeline::getImage() const
	{
		return image;
	}

	const FloatVector4& GlImageDiffuseGpuPipeline::getObjectColor() const
	{
		return ubo.objectColor;
	}
	void GlImageDiffuseGpuPipeline::setObjectColor(const FloatVector4& color)
	{
		ubo.objectColor = Vector4(color);
	}

	const FloatVector4& GlImageDiffuseGpuPipeline::getAmbientColor() const
	{
		return ubo.ambientColor;
	}
	void GlImageDiffuseGpuPipeline::setAmbientColor(const FloatVector4& color)
	{
		ubo.ambientColor = Vector4(color);
	}

	const FloatVector4& GlImageDiffuseGpuPipeline::getLightColor() const
	{
		return ubo.lightColor;
	}
	void GlImageDiffuseGpuPipeline::setLightColor(const FloatVector4& color)
	{
		ubo.lightColor = Vector4(color);
	}

	const FloatVector3& GlImageDiffuseGpuPipeline::getLightDirection() const
	{
		return ubo.lightDirection;
	}
	void GlImageDiffuseGpuPipeline::setLightDirection(const FloatVector3& direction)
	{
		ubo.lightDirection = direction.getNormalized();
	}

	const FloatVector2& GlImageDiffuseGpuPipeline::getImageScale() const
	{
		return ubo.imageScale;
	}
	void GlImageDiffuseGpuPipeline::setImageScale(const FloatVector2& scale)
	{
		ubo.imageScale = Vector2(scale);
	}

	const FloatVector2& GlImageDiffuseGpuPipeline::getImageOffset() const
	{
		return ubo.imageOffset;
	}
	void GlImageDiffuseGpuPipeline::setImageOffset(const FloatVector2& offset)
	{
		ubo.imageOffset = Vector2(offset);
	}

	void GlImageDiffuseGpuPipeline::bind()
	{
		GlGpuPipeline::bind();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_BLEND);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);

		glActiveTexture(GL_TEXTURE0);
		image->bind();

		auto imageType = image->getGlType();

		glTexParameteri(
			imageType,
			GL_TEXTURE_MIN_FILTER,
			glImageMinFilter);
		glTexParameteri(
			imageType,
			GL_TEXTURE_MAG_FILTER,
			glImageMagFilter);

		glTexParameteri(
			imageType,
			GL_TEXTURE_WRAP_S,
			glImageWrapU);
		glTexParameteri(
			imageType,
			GL_TEXTURE_WRAP_T,
			glImageWrapV);
		glTexParameteri(
			imageType,
			GL_TEXTURE_WRAP_R,
			glImageWrapW);

		glBindBufferBase(
			GL_UNIFORM_BUFFER,
			0,
			uniformBuffer->getBuffer());
	}
	void GlImageDiffuseGpuPipeline::flush()
	{
		uniformBuffer->setData(
			&ubo,
			sizeof(UniformBufferObject));
	}
	void GlImageDiffuseGpuPipeline::setAttributes()
	{
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		setVertexAttributePointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(FloatVector3) * 2 + sizeof(FloatVector2),
			0);
		setVertexAttributePointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(FloatVector3) * 2 + sizeof(FloatVector2),
			sizeof(FloatVector3));
		setVertexAttributePointer(
			2, 2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(FloatVector3) * 2 + sizeof(FloatVector2),
			sizeof(FloatVector3) * 2);
	}

	void GlImageDiffuseGpuPipeline::setUniforms(
		const FloatMatrix4& model,
		const FloatMatrix4& view,
		const FloatMatrix4& proj,
		const FloatMatrix4& viewProj,
		const FloatMatrix4& mvp)
	{
		auto normal = model.getInverted().getTransposed();

		setUniform(
			mvpLocation,
			mvp);
		setUniform(
			normalLocation,
			normal);
	}
}
