#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "World.h"
#include "Component.h"
#include "Graphics.h"
#include "Bullet.h"
#include "rand.h"

class ShipInputComponent : public InputComponent {
public:
  void update(Entity * entity) override {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      entity->velocityX = -100;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      entity->velocityX = 100;
    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
      int offsetX = rand(-25, 25);
      int offsetY = rand(-25, 25);
      entity->velocityX += offsetX;
      entity->velocityY += offsetY;
    } else {
      entity->velocityX = 0;
      entity->velocityY = 0;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
      entity->velocityY = -100;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      entity->velocityY = 100;
    } else {
      entity->velocityY = 0;
    }
  }
};

class ShipPhysicsComponent : public PhysicsComponent {
public:
  void update(Entity * entity, World * world) override {
    const float t = 0.01;
    entity->x += entity->velocityX*t;
    entity->y += entity->velocityY*t;
  }
};

class ShipGraphicsComponent : public GraphicsComponent {
private:
  sf::Sprite hero{};
  sf::Texture texture{};
public:
  void update(Entity * entity, Graphics * graphics) override { 
    if(!texture.loadFromFile("assets/spaceship.png")) {
      std::cout << "Could not load texture\n";
    }
    hero.setTexture(texture);
    hero.setPosition(entity->x + 450, entity->y + 450);
    graphics->render(hero);
  }

  float getWidth() const override {
    return static_cast<float>(texture.getSize().x);
  }

  float getHeight() const override {
    return static_cast<float>(texture.getSize().y);
  }

  std::string getType() const override {
    return "ShipGraphicsComponent";
  }
};

static std::unique_ptr<Entity> createShip(World * world) {
  return std::make_unique<Entity>(new ShipInputComponent{}, new ShipPhysicsComponent{}, new ShipGraphicsComponent{});
}