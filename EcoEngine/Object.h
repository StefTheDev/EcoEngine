#pragma once

#include "Component.h"

class Object
{
public:
	void Process() { 
		for (auto& component : components) {
			component.second->Process();
		}
	}
	void Render() {
		for (auto& component : components) {
			component.second->Render();
		}
	}

	template <typename T, typename... A> T& AddComponent(A&& ... a)
	{
		T* component(new T(std::forward<A>(a)...));
		component->object = this;
		if (!component->Initialise()) return *nullptr;

		components.insert(std::pair<std::type_index, std::unique_ptr<Component>>(
			std::type_index(typeid(T)), 
			std::move(std::unique_ptr<Component>(component))
		));

		return *component;
	}

	template <typename T> T& GetComponent()
	{
		return *dynamic_cast<T*>(components[std::type_index(typeid(T))].get());
	}

private:
	std::map<std::type_index, std::unique_ptr<Component>> components;
};
