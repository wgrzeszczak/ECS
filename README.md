# ECS
Simple ECS implementation with continuous memory allocation of components.

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
1. Retrieve a component:
```
TransformComponent* transformComponent = entityManager.getComponent<TransformComponent>(entity);
```
2. Get all components of type:
```
std::vector<TransformComponent> transformComponents = entityManager.getComponents<TransformComponent>();
```
3. Get the first component of type:
```
TransformComponent* transformComponent = entityManager.getComponent<TransformComponent>();
```

## Removing entities
1. Remove a specific entity:
```
entityManager.removeEntity(entity);
```
Selected entity will be removed during the next update cycle, before updating existing systems.

## Removing components
1. Remove a specific component of entity:
```
entityManager.removeComponent<TransformComponent>(entity);
```

## License
```
MIT License

Copyright (c) 2018 Wojciech Grzeszczak

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
