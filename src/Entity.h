#pragma once

class World;
class Graphics;

#include "Component.h"
#include <vector>

class Entity {
public:
  float velocityX{};
  float velocityY{};
  float x{};
  float y{};

  Entity(InputComponent * inputComponent, 
         PhysicsComponent * physicsComponent, 
         GraphicsComponent * graphicsComponent)
  : inputComponent{inputComponent}, physicsComponent{physicsComponent}, graphicsComponent{graphicsComponent} {}

  void update(World * world, Graphics * graphics) {
    inputComponent->update(this);
    physicsComponent->update(this, world);
    graphicsComponent->update(this, graphics);
  }

  InputComponent * getInputComponent() const {
    return inputComponent;
  }

  GraphicsComponent * getGraphicsComponent() const {
    return graphicsComponent;
  }

  void markAsDead() {
      dead = true;
  }

  bool isDead() const {
    return dead;
  }
private:
  InputComponent * inputComponent;
  PhysicsComponent * physicsComponent;
  GraphicsComponent * graphicsComponent;
  bool dead{false};
};