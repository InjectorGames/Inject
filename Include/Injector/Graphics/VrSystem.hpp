#pragma once
#include "Injector/System.hpp"
#include "Injector/Entity.hpp"
#include "Injector/Graphics/CameraComponent.hpp"

#include <memory>

namespace Injector
{
	class VrSystem : public System
	{
	public:
		std::shared_ptr<Entity> hmd;
		
		void update() override;
	};
}