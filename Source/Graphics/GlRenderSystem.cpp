#include "Injector/Graphics/GlRenderSystem.hpp"
#include "Injector/Graphics/GlGpuMesh.hpp"
#include "Injector/Graphics/Pipeline/GlGpuPipeline.hpp"

#include <map>

namespace Injector
{
	GlRenderSystem::GlRenderSystem(GlWindow& _window) :
		window(_window)
	{
	}
	GlRenderSystem::~GlRenderSystem()
	{
	}

	void GlRenderSystem::update()
	{
		struct CameraData
		{
			CameraComponent* camera;
			TransformComponent* transform;
		};

		struct RenderData
		{
			std::shared_ptr<GlGpuPipeline> pipeline;
			std::shared_ptr<GlGpuMesh> mesh;
			TransformComponent* transform;
		};

		auto cameraPairs = std::multimap<int, CameraData>();

		for (auto& camera : cameras)
		{
			auto cameraData = CameraData();

			if (!camera->getComponent(cameraData.camera) ||
				!camera->getComponent(cameraData.transform) ||
				!cameraData.camera->render)
				continue;

			cameraPairs.emplace(cameraData.camera->queue, cameraData);
		}

		window.makeCurrent();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		for (auto& cameraPair : cameraPairs)
		{
			auto cameraData = cameraPair.second;
			auto renderPairs = std::multimap<float, RenderData>();

			for (auto& render : cameraData.camera->renders)
			{
				auto renderData = RenderData();
				RenderComponent* renderComponent;

				if (!render->getComponent<RenderComponent>(renderComponent) ||
					!render->getComponent<TransformComponent>(renderData.transform) ||
					!renderComponent->render ||
					!renderComponent->pipeline ||
					!renderComponent->mesh)
					continue;

				renderData.pipeline = std::dynamic_pointer_cast<GlGpuPipeline>(
					renderComponent->pipeline);
				renderData.mesh = std::dynamic_pointer_cast<GlGpuMesh>(
					renderComponent->mesh);

				if (!renderData.pipeline || !renderData.mesh)
					continue;

				auto distance = cameraData.transform->position.getDistance(
					-renderData.transform->position);
				renderPairs.emplace(distance, renderData);
				renderData.pipeline->flush();
			}

			auto& viewMatrix = cameraData.transform->matrix;
			auto& projMatrix = cameraData.camera->matrix;
			auto viewProjMatrix = projMatrix * viewMatrix;

			std::shared_ptr<GlGpuPipeline> lastPipeline = nullptr;

			for (auto& renderPair : renderPairs)
			{
				auto renderData = renderPair.second;

				auto& modelMatrix = renderData.transform->matrix;
				auto mvpMatrix = viewProjMatrix * modelMatrix;

				if (renderData.pipeline != lastPipeline)
				{
					lastPipeline = renderData.pipeline;
					renderData.pipeline->bind();
				}

				renderData.pipeline->setUniforms(
					modelMatrix, viewMatrix, projMatrix, viewProjMatrix, mvpMatrix);
				renderData.mesh->draw(renderData.pipeline);
			}
		}

		window.swapBuffers();
	}
}
