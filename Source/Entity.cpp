#include "Injector/Entity.hpp"

namespace Injector
{
	Entity::Entity() :
		components()
	{}
	Entity::~Entity()
	{
		destroyComponents();
	}

	void Entity::destroyComponents() noexcept
	{
		for (auto pair : components)
			delete pair.second;

		components.clear();
	}
	size_t Entity::getComponentCount() const noexcept
	{
		return components.size();
	}
}