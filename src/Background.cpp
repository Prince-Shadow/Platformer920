#include "Background.h"
#include "SFML/Graphics.hpp"
#include "Box2D/Box2D.h"
#include "game.h"
#include <iostream>


Background::Background()
{
	//load le background
	if (!backGroundTexture_.loadFromFile("data/sprites/background/back.png"))
	{
		std::cerr << "[Error] Could not load hero texture\n";
	}
	backGroundSprite_.setTexture(backGroundTexture_);
}

void Background::DrawBackground(sf::RenderWindow& window)
{
	window.draw(backGroundSprite_);
}
