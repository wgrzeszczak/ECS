#pragma once
#include <algorithm>
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>

namespace ECS {
	typedef unsigned int Entity;

	struct Component {
		unsigned int componentId;
		Entity entity;
	};

	class BaseComponentContainer {
	public:
		BaseComponentContainer() = default;
		BaseComponentContainer(const BaseComponentContainer& baseComponentContainer) = delete;
		BaseComponentContainer(BaseComponentContainer&& baseComponentContainer) noexcept = delete;
		BaseComponentContainer& operator=(const BaseComponentContainer& baseComponentContainer) = delete;
		BaseComponentContainer& operator=(BaseComponentContainer&& baseComponentContainer) noexcept = delete;
		virtual ~BaseComponentContainer() = default;
		virtual void removeComponent(Entity entity) = 0;
	};

	template<typename TComponent = Component>
	class ComponentContainer : public BaseComponentContainer {
	public:
		ComponentContainer() {
			lastComponentId = 0;
		}
		~ComponentContainer() {};

		template<typename... TComponentArgs>
		void createComponent(Entity entity, TComponentArgs&&... args) {
			if(getComponent(entity) != nullptr) {
				return;
			}
			components.push_back({ lastComponentId++, entity, std::forward<TComponentArgs>(args)... });
			refreshComponentsCache();
		}

		TComponent* getComponent(Entity entity) {
			if(entityToComponent.find(entity) == entityToComponent.end()) {
				return nullptr;
			}

			return entityToComponent[entity];
		}

		void removeComponent(TComponent* component) {
			unsigned int componentId = component->componentId;
			const auto& iterator = std::find_if(components.begin(), components.end(), [componentId](const TComponent& component) {
				return component.componentId == componentId;
			});
			if(iterator != components.end()) {
				components.erase(iterator);
				refreshComponentsCache();
			}
		}

		void removeComponent(Entity entity) override {
			const auto& iterator = std::find_if(components.begin(), components.end(), [entity](const TComponent& component) {
				return component.entity == entity;
			});
			if(iterator != components.end()) {
				components.erase(iterator);
				refreshComponentsCache();
			}
		}

		std::vector<TComponent>& getComponents() {
			return components;
		}

	private:
		std::vector<TComponent> components;
		unsigned int lastComponentId;
		std::unordered_map<Entity, TComponent*> entityToComponent;

		void refreshComponentsCache() {
			entityToComponent.clear();
			for(TComponent& component : components) {
				entityToComponent[component.entity] = &component;
			}
		}
	};

	class System {
	public:
		System(class EntityManager* entityManager) {
			this->entityManager = entityManager;
		}
		System(const System& system) = delete;
		System(System&& system) noexcept = delete;
		System& operator=(const System& system) = delete;
		System& operator=(System&& system) noexcept = delete;
		~System() = default;

		virtual void update(const float delta) = 0;
	protected:
		class EntityManager* entityManager;
	};

	class EntityManager {
	public:
		EntityManager() {
			lastEntityId = 1;
		}
		EntityManager(const EntityManager& entityManager) = delete;
		EntityManager(EntityManager&& entityManager) noexcept = delete;
		EntityManager& operator=(const EntityManager& entityManager) = delete;
		EntityManager& operator=(EntityManager&& entityManager) noexcept = delete;
		~EntityManager() = default;

		Entity createEntity() {
			return entities.emplace_back(lastEntityId++);
		}

		void removeEntity(Entity entity) {
			entitiesToRemove.emplace_back(entity);
		}

		template<typename TComponent, typename... TComponentArgs>
		void createComponent(Entity entity, TComponentArgs&&...  args) {
			static_assert(std::is_base_of<Component, TComponent>::value, "Components must inherit from Component");
			ComponentContainer<TComponent>* container = getComponentsContainer<TComponent>();
			container->createComponent(entity, std::forward<TComponentArgs>(args)...);
		}

		template<typename TComponent>
		TComponent* getComponent(Entity entity) {
			static_assert(std::is_base_of<Component, TComponent>::value, "Components must inherit from Component");
			ComponentContainer<TComponent>* container = getComponentsContainer<TComponent>();
			return container->getComponent(entity);
		}

		template<typename TComponent>
		TComponent* getComponent() {
			static_assert(std::is_base_of<Component, TComponent>::value, "Components must inherit from Component");
			ComponentContainer<TComponent>* container = getComponentsContainer<TComponent>();
			std::vector<TComponent> components = container->getComponents();
			
			if(components.size()) {
				return &components.front();
			}
			return nullptr;
		}

		template<typename TComponent>
		void removeComponent(TComponent* component) {
			static_assert(std::is_base_of<Component, TComponent>::value, "Components must inherit from Component");
			ComponentContainer<TComponent>* container = getComponentsContainer<TComponent>();
			container->removeComponent(component);
		}

		template<typename TComponent>
		void removeComponent(Entity entity) {
			static_assert(std::is_base_of<Component, TComponent>::value, "Components must inherit from Component");
			ComponentContainer<TComponent>* container = getComponentsContainer<TComponent>();
			container->removeComponent(entity);
		}

		template<typename TComponent>
		std::vector<TComponent>& getComponents() {
			static_assert(std::is_base_of<Component, TComponent>::value, "Components must inherit from Component");
			ComponentContainer<TComponent>* container = getComponentsContainer<TComponent>();
			return container->getComponents();
		}

		std::vector<Entity>& getEntities() {
			return entities;
		}

		template<typename TSystem, typename... TSystemArgs>
		TSystem* createSystem(TSystemArgs&&... args) {
			static_assert(std::is_base_of<System, TSystem>::value, "Systems must inherit from System");
			systems[typeid(TSystem)] = std::make_unique<TSystem>(this, std::forward<TSystemArgs>(args)...);
			return dynamic_cast<TSystem*>(systems[typeid(TSystem)].get());
		}

		template<typename TSystem>
		TSystem* getSystem() {
			static_assert(std::is_base_of<System, TSystem>::value, "Systems must inherit from System");
			if(systems.find(typeid(TSystem)) == system.end()) {
				return nullptr;
			}

			return dynamic_cast<TSystem*>(systems[typeid(TSystem)].get());
		}

		void update(const float delta) {
			removePendingEntites();
			for(auto& system : systems) {
				system.second->update(delta);
			}
		}
	private:
		Entity lastEntityId;
		std::vector<Entity> entities;
		std::vector<Entity> entitiesToRemove;
		std::unordered_map<std::type_index, std::unique_ptr<BaseComponentContainer>> componentContainers;
		std::unordered_map<std::type_index, std::unique_ptr<System>> systems;

		template<typename TComponent>
		ComponentContainer<TComponent>* getComponentsContainer() {
			if(componentContainers.find(typeid(TComponent)) == componentContainers.end()) {
				componentContainers[typeid(TComponent)] = std::make_unique<ComponentContainer<TComponent>>();
			}

			ComponentContainer<TComponent>* container = dynamic_cast<ComponentContainer<TComponent>*>(componentContainers[typeid(TComponent)].get());
			return container;
		}

		void removePendingEntites() {
			for(Entity entity : entitiesToRemove) {
				for(auto& container : componentContainers) {
					container.second->removeComponent(entity);
				}
				if(const auto& iterator = std::find(entities.begin(), entities.end(), entity); iterator != entities.end()) {
					entities.erase(iterator);
				}
			}
			entitiesToRemove.clear();
		}
	};
}