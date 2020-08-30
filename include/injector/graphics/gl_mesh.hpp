#pragma once
#include <injector/graphics/mesh.hpp>
#include <injector/graphics/gl_buffer.hpp>
#include <injector/graphics/gl_vertex_attribute.hpp>

#include <vector>

namespace INJECTOR_NAMESPACE
{
	class GlMesh : public Mesh
	{
	protected:
		GLuint vertexArray;
		GlBufferHandle vertexBuffer;
		GlBufferHandle indexBuffer;
	public:
		GlMesh(size_t indexCount,
			BufferIndex indexType,
			const GlBufferHandle& vertexBuffer,
			const GlBufferHandle& indexBuffer);
		virtual ~GlMesh();

		GLuint getVertexArray() const noexcept;
		const GlBufferHandle& getVertexBuffer() const noexcept;
		const GlBufferHandle& getIndexBuffer() const noexcept;

		void draw(GLuint mode,
			const std::vector<GlVertexAttribute>& vertexAttributes) noexcept;

		void setVertexData(void* data, size_t size) override;
		void setVertexData(void* data, size_t size, size_t offset) override;

		void setIndexData(void* data, size_t size) override;
		void setIndexData(void* data, size_t size, size_t offset) override;

		/*template<class TV, class TI>
		inline static const std::shared_ptr<GlMesh> Create(
			const std::vector<TV>& vertices,
			const std::vector<TI>& indices,
			const std::vector<GlAttribute>& attributes,
			const GlBuffer::Usage usage,
			const DrawType drawType) noexcept
		{
			auto vertexBuffer = std::make_shared<GlBuffer>(
				GlBuffer::Type::Array, usage);
			vertexBuffer->bind();
			vertexBuffer->setData<TV>(vertices);
			vertexBuffer->unbind();

			auto indexBuffer = std::make_shared<GlBuffer>(
				GlBuffer::Type::ElementArray, usage);
			indexBuffer->bind();
			indexBuffer->setData<TI>(indices);
			indexBuffer->unbind();

			return std::make_shared<GlMesh>(
				vertexBuffer,
				indexBuffer,
				attributes,
				indices.size(),
				drawType);
		}
		template<class TV, class TI>
		inline static const std::shared_ptr<GlMesh> Create(
			const std::vector<const std::vector<TV>*>& vertexArrays,
			const std::vector<TI>& indices,
			const std::vector<GlAttribute>& attributes,
			const GlBuffer::Usage usage,
			const DrawType drawType) noexcept
		{
			if (vertexArrays.size() != attributes.size())
				throw std::runtime_error("Incorrect data sizez");

			auto vertexCount = static_cast<size_t>(0);
			for (size_t i = 0; i < vertexArrays.size(); i++)
				vertexCount += vertexArrays[i]->size();

			std::vector<TV> vertices(vertexCount);

			for (size_t i = 0; i < vertexArrays.size(); i++)
			{
				const auto _vertices = vertexArrays[i];
				const auto size = static_cast<size_t>(attributes[i].size);
				const auto stride = attributes[i].stride / sizeof(TV);
				const auto offset = attributes[i].offset / sizeof(TV);

				for (size_t j = 0, o = offset; j < _vertices->size(); j += size, o += stride)
					std::copy_n(&_vertices[0][j], size, &vertices[o]);
			}

			auto vertexBuffer = std::make_shared<GlBuffer>(
				GlBuffer::Type::Array, usage);
			vertexBuffer->bind();
			vertexBuffer->setData<TV>(vertices);
			vertexBuffer->unbind();

			auto indexBuffer = std::make_shared<GlBuffer>(
				GlBuffer::Type::ElementArray, usage);
			indexBuffer->bind();
			indexBuffer->setData<TI>(indices);
			indexBuffer->unbind();

			return std::make_shared<GlMesh>(
				vertexBuffer,
				indexBuffer,
				attributes,
				indices.size(),
				drawType);
		}

		inline static const std::shared_ptr<GlMesh> CreateSquareV() noexcept
		{
			auto attributes = std::vector<GlAttribute>
			{
				GlAttribute(0, GlAttribute::Size::Three, GlAttribute::Type::Float, false, 0, 0),
			};

			return Create(GlPrimitives::squareVertices, GlPrimitives::squareIndices,
				attributes, GlBuffer::Usage::StaticDraw, DrawType::UnsignedByte);
		}
		inline static const std::shared_ptr<GlMesh> CreateSquareVN() noexcept
		{
			auto vertices = std::vector<const std::vector<float>*>
			{
				&GlPrimitives::squareVertices,
				&GlPrimitives::squareNormals,
			};
			auto attributes = std::vector<GlAttribute>
			{
				GlAttribute(0, GlAttribute::Size::Three, GlAttribute::Type::Float, false, sizeof(float) * 6, 0),
				GlAttribute(1, GlAttribute::Size::Three, GlAttribute::Type::Float, false, sizeof(float) * 6, sizeof(float) * 3),
			};

			return Create(vertices, GlPrimitives::squareIndices,
				attributes, GlBuffer::Usage::StaticDraw, DrawType::UnsignedByte);
		}

		inline static const std::shared_ptr<GlMesh> CreateCubeV() noexcept
		{
			auto attributes = std::vector<GlAttribute>
			{
				GlAttribute(0, GlAttribute::Size::Three, GlAttribute::Type::Float, false, 0, 0),
			};

			return Create(GlPrimitives::cubeVertices, GlPrimitives::cubeIndices,
				attributes, GlBuffer::Usage::StaticDraw, DrawType::UnsignedByte);
		}
		inline static const std::shared_ptr<GlMesh> CreateCubeVN() noexcept
		{
			auto vertices = std::vector<const std::vector<float>*>
			{
				&GlPrimitives::cubeVertices,
				&GlPrimitives::cubeNormals,
			};
			auto attributes = std::vector<GlAttribute>
			{
				GlAttribute(0, GlAttribute::Size::Three, GlAttribute::Type::Float, false, sizeof(float) * 6, 0),
				GlAttribute(1, GlAttribute::Size::Three, GlAttribute::Type::Float, false, sizeof(float) * 6, sizeof(float) * 3),
			};

			return Create(vertices, GlPrimitives::cubeIndices,
				attributes, GlBuffer::Usage::StaticDraw, DrawType::UnsignedByte);
		}

		inline static const std::shared_ptr<GlMesh> CreateAxisV() noexcept
		{
			auto attributes = std::vector<GlAttribute>
			{
				GlAttribute(0, GlAttribute::Size::Three, GlAttribute::Type::Float, false, 0, 0),
			};

			return Create(GlPrimitives::axisVertices, GlPrimitives::axisIndices,
				attributes, GlBuffer::Usage::StaticDraw, DrawType::UnsignedByte);
		}
		inline static const std::shared_ptr<GlMesh> CreateAxisVC() noexcept
		{
			auto vertices = std::vector<const std::vector<float>*>
			{
				&GlPrimitives::axisVertices,
				&GlPrimitives::axisColors,
			};

			auto attributes = std::vector<GlAttribute>
			{
				GlAttribute(0, GlAttribute::Size::Three, GlAttribute::Type::Float, false, sizeof(float) * 6, 0),
				GlAttribute(1, GlAttribute::Size::Three, GlAttribute::Type::Float, false, sizeof(float) * 6, sizeof(float) * 3),
			};

			return Create(vertices, GlPrimitives::axisIndices,
				attributes, GlBuffer::Usage::StaticDraw, DrawType::UnsignedByte);
		}

		inline static const std::shared_ptr<GlMesh> CreateGradientSky() noexcept
		{
			auto attributes = std::vector<GlAttribute>
			{
				GlAttribute(0, GlAttribute::Size::Three, GlAttribute::Type::Float, false, 0, 0),
			};

			return Create(GlPrimitives::gradientSkyVertices, GlPrimitives::gradientSkyIndices,
				attributes, GlBuffer::Usage::StaticDraw, DrawType::UnsignedByte);
		}*/
	};

	using GlMeshHandle = std::shared_ptr<GlMesh>;
}
