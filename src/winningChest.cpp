#include "winningChest.h"
#include <iostream>
#include "SFML/Graphics.hpp"
#include <box2d/box2d.h>
#include "game.h"
#include "user_data.h"

WinningChest::WinningChest(Game& game) : game_(game)
{
    isTextureLoaded_ = false;
}

void WinningChest::Init()
{
	if (!chestTexture_.loadFromFile("data/sprites/Chest.png"))
	{
		std::cerr << "[Error] Could not load texture\n";
	}
	chestSprite_.setTexture(chestTexture_);
    b2BodyDef bodyDef;
    bodyDef.position.Set(originPos.x, originPos.y);


    chestSprite_.setPosition(
        originPos.x * game_.pixelToMeter,
        originPos.y * game_.pixelToMeter);

    body_ = game_.CreateBody(bodyDef);


    const auto textureSize = chestTexture_.getSize();
    chestSprite_.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
    chestSprite_.setScale(scale_, scale_);


    b2Vec2 boxSize;
    boxSize.Set(
        textureSize.x / game_.pixelToMeter / 2.0f * scale_,
        textureSize.y / game_.pixelToMeter / 2.0f * scale_
    );


    b2PolygonShape platformShape;
    platformShape.SetAsBox(boxSize.x, boxSize.y);
    b2FixtureDef platformFixtureDef;
    platformFixtureDef.userData = &chestUserData_;
    platformFixtureDef.shape = &platformShape;
    body_->CreateFixture(&platformFixtureDef);

}

void WinningChest::Render(sf::RenderWindow& window)
{
    window.draw(chestSprite_);
}