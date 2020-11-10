#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include "Background.h"
#include "contact_trigger.h"
#include "platform.h"
#include "player.h"
#include "winningChest.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "box2d/b2_world.h"
#include "SFML/Audio.hpp"

class Game
{
public:
    Game();
    void Init();
    void Loop();

    b2Body* CreateBody(const b2BodyDef& bodyDef);
    void BeginContact(UserDataType userData, UserDataType userData1);
    void EndContact(UserDataType userData, UserDataType userData1);
    const float pixelToMeter = 100.0f;
private:
    //met la physique du temps à 50Hz
    const float fixedTimestep_ = 0.02f;
    float fixedTimer_ = 0.0f;

    sf::RenderWindow window_;
    sf::Sprite sprite_;
    b2World world_{b2Vec2(0.0f, 9.81f)};
    
    //Background background_;
    Player player_;
    Platform platform_;
    WinningChest WinningChest_; // il me manque quelque chose je n'arrive pas a l'appeller
    sf::Music music_;

    MyContactListener contactTrigger_;
};