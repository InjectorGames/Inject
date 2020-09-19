#pragma once
#include "Injector/Graphics/Pipeline/GlGpuPipeline.hpp"
#include "Injector/Graphics/Pipeline/ColorGpuPipeline.hpp"
#include "Injector/Graphics/GlGpuShader.hpp"

namespace Injector
{
	class GlColorGpuPipeline :
		public GlGpuPipeline,
		public ColorGpuPipeline
	{
	 protected:
		GLint mvpLocation;
		GLint colorLocation;

		Vector4 color;
	 public:
		GlColorGpuPipeline(
			const std::shared_ptr<GlGpuShader>& vertexShader,
			const std::shared_ptr<GlGpuShader>& fragmentShader,
			const Vector4& color = Vector4::one);
		virtual ~GlColorGpuPipeline();

		const Vector4& getColor() const override;
		void setColor(const Vector4& color) override;

		void bind() override;
		void flush() override;
		void setAttributes() override;

		void setUniforms(
			const Matrix4& model,
			const Matrix4& view,
			const Matrix4& proj,
			const Matrix4& viewProj,
			const Matrix4& mvp) override;
	};
}