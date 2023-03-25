#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Entity.h"
#include "Ship.h"
#include "Component.h"
#include "Graphics.h"
#include "rand.h"

class EnemyInputComponent : public InputComponent {
public:
    void update(Entity * entity) override {
        int velocityMultiplier{1};
        int randomDir = rand(0, 11);

        if(entity->getInputComponent()->isSpaceKeyPressed()) {
            velocityMultiplier = 2;
        }

        if(randomDir < 6) {
            entity->velocityX = -100 * velocityMultiplier;
        }
        else {
            entity->velocityX = 140 * velocityMultiplier;
        }
    }
};

class EnemyPhysicsComponent : public PhysicsComponent {
public:
    void update(Entity * entity, World * world) override {
        const float t = 0.01;
        entity->x += entity->velocityX * t;
        entity->y += entity->velocityY * t;
    }
};

class EnemyGraphicsComponent : public GraphicsComponent {
private:
    sf::Sprite enemySprite{};
    sf::Texture texture{};
public:
    void update(Entity * entity, Graphics * graphics) override {
        if(!texture.loadFromFile("assets/Tfighter.png")) {
            std::cout << "Failed to load enemy texture\n";
        }
        enemySprite.setTexture(texture);
        enemySprite.setPosition(entity->x, entity->y);
        graphics->render(enemySprite);
    }

    float getWidth() const override {
        return static_cast<float>(texture.getSize().x);
    }

    float getHeight() const override {
        return static_cast<float>(texture.getSize().y);
    }

    std::string getType() const override {
        return "EnemyGraphicsComponent";
    }
};

class Enemy2InputComponent : public InputComponent {
public:
    void update(Entity * entity) override {
        int velocityMultiplier{1};
        int randomDir = rand(0, 11);

        if(entity->getInputComponent()->isSpaceKeyPressed()) {
            velocityMultiplier = 2;
        }

        if(randomDir < 6) {
            entity->velocityX = -80 * velocityMultiplier;
        }
        else {
            entity->velocityX = 130 * velocityMultiplier;
        }
    }
};

class Enemy2PhysicsComponent : public PhysicsComponent {
public:
    void update(Entity * entity, World * world) override {
        const float t = 0.01;
        entity->x += entity->velocityX * t;
        entity->y += entity->velocityY * t;
    }
};

class Enemy2GraphicsComponent : public GraphicsComponent {
private:
    sf::Sprite enemySprite{};
    sf::Texture texture{};
public:
    void update(Entity * entity, Graphics * graphics) override {
        if(!texture.loadFromFile("assets/DarthVader.png")) {
            std::cout << "Failed to load enemy texture\n";
        }
        enemySprite.setTexture(texture);
        enemySprite.setPosition(entity->x, entity->y + 100);
        graphics->render(enemySprite);
    }
    float getWidth() const override {
        return static_cast<float>(texture.getSize().x);
    }

    float getHeight() const override {
        return static_cast<float>(texture.getSize().y);
    }

    std::string getType() const override {
        return "Enemy2GraphicsComponent";
    }
};

class EnemyFactory {
public:
    static std::unique_ptr<Entity> createEnemy(int type) {
        switch (type) {
            case 1:
                return std::make_unique<Entity>(new EnemyInputComponent{}, new EnemyPhysicsComponent{}, new EnemyGraphicsComponent{});
            case 2:
                return std::make_unique<Entity>(new Enemy2InputComponent{}, new Enemy2PhysicsComponent{}, new Enemy2GraphicsComponent{});
            default:
                return nullptr;
        }
    }
};