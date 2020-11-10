#pragma once
#include "SFML/Graphics.hpp"
#include "game.h"
#include "player.h"

class Background
{
public:
	Background();
	void DrawBackground(sf::RenderWindow& window);

private:
	sf::Texture backGroundTexture_;
	sf::Sprite backGroundSprite_;

	sf::Texture back_;
	
};
