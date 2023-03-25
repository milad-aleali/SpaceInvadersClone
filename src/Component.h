#pragma once

class Entity;
class World;
class Graphics;

class InputComponent {
public:
    virtual ~InputComponent() {}
    virtual void update(Entity * entity) = 0;
    virtual bool isSpaceKeyPressed() const {
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

    }
};

class PhysicsComponent {
public:
    virtual ~PhysicsComponent() {}
    virtual void update(Entity * entity, World * world) = 0;
};

class GraphicsComponent {
public:
    virtual ~GraphicsComponent() {}
    virtual void update(Entity * entity, Graphics * graphics) = 0;
    virtual float getWidth() const = 0;
    virtual float getHeight() const = 0;
    virtual std::string getType() const = 0;
};