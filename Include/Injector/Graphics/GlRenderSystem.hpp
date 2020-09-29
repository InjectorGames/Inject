#pragma once
#include "Injector/Graphics/RenderSystem.hpp"
#include "Injector/Graphics/GlWindow.hpp"

namespace Injector
{
	class GlRenderSystem : public RenderSystem
	{
	 protected:
		GlWindow& window;
	 public:
		explicit GlRenderSystem(
			GlWindow& window);

		void update() override;
	};
}
