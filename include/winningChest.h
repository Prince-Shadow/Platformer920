#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "game.h"

namespace sf {
	class RenderWindow;
}

class b2Body;
class Game;

class WinningChest
{
public:
	explicit WinningChest(Game& game);
	void Init();
	void Update(float dt);
	WinningChest();
	void Render(sf::RenderWindow& window);

private:
	Game& game_;
	b2Body* body_ = nullptr;
	const float scale_ = 3.0f;
	sf::Sprite chestSprite_;
	inline static bool isTextureLoaded_;
	inline static sf::Texture chestTexture_;
	const sf::Vector2f originPos = sf::Vector2f(3.0f, 3.0f);
	UserData chestUserData_ = { UserDataType::CHEST };
};
