#pragma once
#include "Injector/Graphics/CameraSystem.hpp"
#include "Injector/Graphics/VkWindow.hpp"

namespace Injector
{
	class VkCameraSystem : public CameraSystem
	{
	 protected:
		VkWindow& window;
	 public:
		explicit VkCameraSystem(
			VkWindow& window);

		void update() override;
	};
}
