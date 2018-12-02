# ECS
Simple single header ECS implementation with continuous memory allocation of components.

### Entities
Entities are simple identifiers, which allow systems to identify connected components that the entity comprises of.

### Components
Components are simple data storage objects, which do not possess any logic.

### Systems
Systems are responsible for processing and modifying entities by manipulating their components.

## Usage
### Creating entities
1. Create an entity manager instance:
```
ECS::EntityManager entityManager;
```
2. Create an entity:
```
Entity entity = entityManager.createEntity();
```

### Creating systems
1. Create a new system:
```
entityManager.createSystem<MovementSystem>();
```

You can pass arguments to the createSystem method. They will be forwarded to the created system's constructor.

### Creating entities with components
1. Create a new component:
```
entityManager.createComponent<TransformComponent>(entity, position);
entityManager.createComponent<MovementComponent>(entity, velocity);
```

You can pass arguments to the createComponent method. They will be forwarded to the created component's constructor.

### Retrieving entities
1. Get all entities:
```
std::vector<ECS::Entity> entities = entityManager.getEntities();
```

### Retrieving systems
1. Get a specified system:
```
MovementSystem* movementSystem = entityManager.getSystem<MovementSystem>();
```

### Retrieving components
1. Retrieve components belonging to an entity e:
```
TransformComponent* transformComponent = entityManager.getComponent<TransformComponent>(e);
```
2. Get all components of specified type:
```
std::vector<TransformComponent> transformComponents = entityManager.getComponents<TransformComponent>();
```
3. Get the first component of specified type:
```
TransformComponent* transformComponent = entityManager.getComponent<TransformComponent>();
```

## Removing entities
1. Remove a specific entity e:
```
entityManager.removeEntity(e);
```
Selected entity will be removed during the next update cycle, before updating existing systems.

## Removing components
1. Remove a specific component of entity e:
```
entityManager.removeComponent<TransformComponent>(e);
```
