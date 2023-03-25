#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Entity.h"
#include "Component.h"
#include "Graphics.h"

class BulletInputComponent : public InputComponent {
public:
   void update(Entity * entity) override {
   } 
};

class BulletPhysicsComponent : public PhysicsComponent {
public:
    void update(Entity * entity, World * world) override {
        const float t = 0.01;
        entity->x += entity->velocityX * t;
        entity->y += entity->velocityY * t; 
    }
};

class BulletGraphicsComponent : public GraphicsComponent {
private:
    sf::Sprite bulletSprite{};
    sf::Texture texture{};
public:
    void update(Entity * entity, Graphics * graphics) override {
        if(!texture.loadFromFile("assets/bullet.png")) {
            std::cout << "Could not load texture\n";
        }
        bulletSprite.setTexture(texture);
        bulletSprite.setPosition(entity->x, entity->y);
        graphics->render(bulletSprite);
    }

    float getWidth() const override {
        return static_cast<float>(texture.getSize().x);
    }

    float getHeight() const override {
        return static_cast<float>(texture.getSize().y);
    }

    std::string getType() const override {
        return "BulletGraphicsComponent";
    }
};

class BulletFactory {
public:
    static std::unique_ptr<Entity> createBullet(float x, float y, float velocityX, float velocityY) {
        auto bullet = std::make_unique<Entity>(new BulletInputComponent{}, new BulletPhysicsComponent{}, new BulletGraphicsComponent{});
        bullet->x = x;
        bullet->y = y;
        bullet->velocityX = velocityX;
        bullet->velocityY = velocityY;
        return bullet;
    }
};