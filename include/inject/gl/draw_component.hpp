#pragma once
#include <inject/gl/mesh.hpp>
#include <inject/gl/material.hpp>

#include <entityx/entityx.h>

namespace inject
{
	struct GlDrawComponent final : public entityx::Component<GlDrawComponent>
	{
		enum class Order
		{
			Ascending,
			Descending,
			Front,
			Back,
		};

		int queue;
		Order order;
		int offset;
		std::shared_ptr<GlMaterial> material;
		std::shared_ptr<GlMesh> mesh;
		GlMesh::DrawMode drawMode;
		bool draw;

		GlDrawComponent(const int _queue = 0,
			const Order _order = Order::Ascending,
			const int _offset = 0,
			const std::shared_ptr<GlMaterial>& _material = std::shared_ptr<GlMaterial>(),
			const std::shared_ptr<GlMesh>& _mesh = std::shared_ptr<GlMesh>(),
			const GlMesh::DrawMode _drawMode = GlMesh::DrawMode::Triangles,
			const bool _draw = true) :
			queue(_queue),
			order(_order),
			offset(_offset),
			material(_material),
			mesh(_mesh),
			drawMode(_drawMode),
			draw(_draw)
		{}
	};
}