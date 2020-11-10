#include "game.h"
#include <iostream>
#include "SFML/Window/Event.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Audio.hpp"

Game::Game() : player_(*this), platform_(*this), contactTrigger_(*this), WinningChest_(*this)
{
    
}

void Game::Init()
{
    world_.SetContactListener(&contactTrigger_);

    window_.create(sf::VideoMode(1280, 720),
        "Platformer Sara Adventure");
    window_.setVerticalSyncEnabled(true);
    
    //load la music du jeu et le joue avec le volume à 30
    if(!music_.openFromFile("data/Music/CrocoRocket.wav"))
    {
        std::cerr << "Could not load music!\n";
    }
    music_.play();
    music_.setVolume(30.f);
    player_.Init();
    platform_.Init();
    WinningChest_.Init();
}

void Game::Loop()
{
    sf::Clock gameClock_;
    while(window_.isOpen())
    {
        auto dt = gameClock_.restart();
        sf::Event event;
        while (window_.pollEvent(event))
        {
            // permet de fermer la fenêtre avec la croix "X"
            if (event.type == sf::Event::Closed)
            {
                window_.close();
                return;
            }
            //permet de paramêtrer la taille de la fenêtre de jeu
            if(event.type == sf::Event::Resized)
            {
                auto view = window_.getView();
                view.setSize(event.size.width, event.size.height);
                window_.setView(view);
            }
        }

        fixedTimer_ += dt.asSeconds();
        while(fixedTimer_ > fixedTimestep_)
        {
            world_.Step(fixedTimestep_, 8, 3);
            fixedTimer_ -= fixedTimestep_;
        }

        player_.Update(dt.asSeconds());
        //la caméra suit le joueur
        auto view = window_.getView();
        view.setCenter(player_.GetPosition());
        window_.setView(view);


        window_.clear(sf::Color::White);
        const auto windowsSize = window_.getSize();
        sprite_.setPosition(windowsSize.x/2.0f, windowsSize.y/2.0f);
        window_.draw(sprite_);

        player_.Render(window_);
        platform_.Render(window_);
        WinningChest_.Render(window_);
        //met un background, il manque quelque chose
        //background_.DrawBackground(window_);
        //loop de la music
        //music_.play();

        window_.display();
    }
}

b2Body* Game::CreateBody(const b2BodyDef& bodyDef)
{
    return world_.CreateBody(&bodyDef);
}

void Game::BeginContact(UserDataType userData, UserDataType userData1)
{
    if((userData == UserDataType::PLATFORM && userData1 == UserDataType::PLAYER_FOOT) ||
        (userData1 == UserDataType::PLATFORM && userData == UserDataType::PLAYER_FOOT)
        )
    {
        player_.BeginContactGround();
    }
}

void Game::EndContact(UserDataType userData, UserDataType userData1)
{
    if ((userData == UserDataType::PLATFORM && userData1 == UserDataType::PLAYER_FOOT) ||
        (userData1 == UserDataType::PLATFORM && userData == UserDataType::PLAYER_FOOT))
    {
        player_.EndContactGround();
    }
}
