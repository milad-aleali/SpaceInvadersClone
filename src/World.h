#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Entity.h"
#include "Ship.h"
#include "Bullet.h"
#include "Graphics.h"
#include "Enemy.h"

class World {
public:
    World() {
        if(!mainMenuMusic.openFromFile("sound/mainMenuMusic.mp3")){
            std::cout << "Could not load main menu music\n";
        }
        std::unique_ptr<Entity> ship = createShip(this);
        shipPosition = ship.get();
        entities.push_back(ship.get());
        ship.release();
        for(int i = 0; i < 10; i++) {
            int randEnemy = rand(1, 2);
            std::unique_ptr<Entity> enemy = EnemyFactory::createEnemy(randEnemy);
            entities.push_back(enemy.get());
            enemy.release();
        }
        if(!bulletSoundBuffer.loadFromFile("sound/bulletSound.mp3")) {
            std::cout << "Could not load bullet sound\n";
        }
        bulletSound.setBuffer(bulletSoundBuffer);
    }

    ~World() {
        for (auto & entity : entities) {
            delete entity;
        }
    }
    
    bool checkCollision(Entity *entity1, Entity *entity2) {
        sf::FloatRect rect1(entity1->x, entity1->y, entity1->getGraphicsComponent()->getWidth(), entity1->getGraphicsComponent()->getHeight());
        sf::FloatRect rect2(entity2->x, entity2->y, entity2->getGraphicsComponent()->getWidth(), entity2->getGraphicsComponent()->getHeight());

        return rect1.intersects(rect2);
    }

    void displayStartScreen() {
        sf::Font font;
        if(!font.loadFromFile("fonts/Alkatra.ttf")) {
            std::cout << "Could not load font\n";
        }
        sf::Text text;
        text.setFont(font);
            text.setString("Press ENTER to start the game\nKill all enemies to win\nPress A to Shoot, Space to scare enemies\nPress ESCAPE after winning to close the game");
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Red);
        text.setPosition(300, 200);

        mainMenuMusic.setLoop(true);
        mainMenuMusic.play();
        bool gameStart{};
        while(!gameStart && graphics.window.isOpen()) {
            sf::Event event;
            while(graphics.window.pollEvent(event)) {
                if(event.type == sf::Event::Closed) {
                    graphics.window.close();
                } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    gameStart = true;
                }
            }
            graphics.window.clear();
            graphics.window.draw(text);
            graphics.window.display();
            if(gameStart){
                mainMenuMusic.stop();
            }
        }
    }

    void displayEndScreen(const std::string &message) {
        sf::Font font;
            if (!font.loadFromFile("fonts/Alkatra.ttf")) {
                std::cout << "Failed to load font\n";
            }

        sf::Text text;
        text.setFont(font);
        text.setString(message);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Red);
        text.setPosition(400, 300);

        sf::Clock clock;
        const float timeToWait = 5.0f;
        while (graphics.window.isOpen() && clock.getElapsedTime().asSeconds() < timeToWait) {
            sf::Event event;
            while (graphics.window.pollEvent(event)) {
                if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                    graphics.window.close();
                    return;
                }
            }
            graphics.window.clear();
            graphics.window.draw(text);
            graphics.window.display();
        }
    }

    void checkWinLoseCondition() {
        size_t enemyCount{};
        bool enemyOutOfBounds{};
        for (const auto &entity : entities) {
            if(entity->getGraphicsComponent()->getType() == "EnemyGraphicsComponent" ||
               entity->getGraphicsComponent()->getType() == "Enemy2GraphicsComponent") {
                enemyCount++;
                if(entity->y < 0 || entity->y > graphics.window.getSize().y) {
                    enemyOutOfBounds = true;
                    break;
                }
            }
        }
        if(enemyOutOfBounds) {
            gameOver = true;
            gameOverMessage = "You Lose!!";
        } else if(enemyCount == 00) {
            gameOver = true;
            gameOverMessage = "You Win!!!";
        }
    }

    void run() {
        displayStartScreen();

        while (graphics.window.isOpen()) {
            processGlobalEvents();
            if (!gameOver) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                    float bulletX = shipPosition->x + 450;
                    float bulletY = shipPosition->y + 450;
                    std::unique_ptr<Entity> bullet = BulletFactory::createBullet(bulletX, bulletY, 0.0f, -200.0f);
                    entities.push_back(bullet.get());
                    bullet.release();
                    bulletSound.play();
                }

                graphics.window.clear();
                for (auto &entity : entities) {
                    entity->update(this, &graphics);
                }

                for (auto &bullet : entities) {
                    if (bullet->getGraphicsComponent()->getType() == "BulletGraphicsComponent") {
                        for (auto &enemy : entities) {
                            if (enemy->getGraphicsComponent()->getType() == "EnemyGraphicsComponent" ||
                                enemy->getGraphicsComponent()->getType() == "Enemy2GraphicsComponent") {

                                if (checkCollision(bullet, enemy)) {
                                    bullet->markAsDead();
                                    enemy->markAsDead();
                                }
                            }
                        }
                    }
                }
                entities.erase(std::remove_if(entities.begin(), entities.end(), [](Entity *e) { return e->isDead(); }), entities.end());
                checkWinLoseCondition();
                graphics.window.display();
            } else {
                displayEndScreen(gameOverMessage);
                break;
            }
        }
    }

private:
    Graphics graphics;
    std::vector<Entity *> entities;
    Entity * shipPosition;
    bool gameOver{};
    std::string gameOverMessage{};
    sf::SoundBuffer bulletSoundBuffer;
    sf::Sound bulletSound;
    sf::Music mainMenuMusic;
    void processGlobalEvents() {
        sf::Event event;
        while (graphics.window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                graphics.window.close();
                break;
            }
        }
    }
};