#include "Injector/Graphics/OpenGL/GlCameraEcsSystem.hpp"

namespace Injector
{
	void GlCameraEcsSystem::onUpdate()
	{
		for (auto& camera : cameras)
		{
			CameraEcsComponent* cameraComponent;

			if (!camera->getComponent(cameraComponent))
				continue;

			if (cameraComponent->type == CameraType::Perspective)
			{
				cameraComponent->matrix = glCreatePerspectiveMatrix(
					cameraComponent->fieldOfView,
					cameraComponent->aspectRatio,
					cameraComponent->clipPlane.x,
					cameraComponent->clipPlane.y);
			}
			else if(cameraComponent->type == CameraType::Orthographic)
			{
				cameraComponent->matrix = glCreateOrthographicMatrix(
					cameraComponent->frustum.x * cameraComponent->aspectRatio,
					cameraComponent->frustum.y * cameraComponent->aspectRatio,
					cameraComponent->frustum.z,
					cameraComponent->frustum.w,
					cameraComponent->clipPlane.x,
					cameraComponent->clipPlane.y);
			}
		}
	}
}
