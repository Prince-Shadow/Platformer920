
#define _USE_MATH_DEFINES
#include <cmath>
#include "player.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <SFML/Window/Keyboard.hpp>
#include <sstream>
#include "game.h"
#include <box2d/box2d.h>


Player::Player(Game& game) : game_(game)
{
}

void Player::Init()
{
    //load l'image d'attente de sara
    if (!idleTexture_.loadFromFile("data/sprites/Sara/idle/Sara_Idle.png"))
    {
        std::cerr << "Could not load hero idle texture!\n";
    }

    //load plusieurs image pour la marche de sara
    for (int i = 0; i < 8; i++)
    {
        std::ostringstream oss;
        oss << "data/sprites/Sara/walk/SaraWalk" << (i + 1) << ".png";
        walkTexture_[i].loadFromFile(oss.str());
    }

    //load l'image de "jump" de sara
    jumpTexture_.loadFromFile("data/sprites/Sara/jump/Sarajump.png");
    UpdateSpriteTexture(idleTexture_);
    sprite_.setScale(playerScale_, playerScale_);


    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(1.0f, 1.0f);
    body_ = game_.CreateBody(bodyDef);

    const auto textureSize = idleTexture_.getSize() - borderMargin_ * 2u;
    b2Vec2 boxSize;
    boxSize.Set(
        textureSize.x / game_.pixelToMeter / 2.0f * playerScale_,
        textureSize.y / game_.pixelToMeter / 2.0f * playerScale_);
    b2PolygonShape playerBox;
    playerBox.SetAsBox(boxSize.x, boxSize.y);

    b2FixtureDef playerFixtureDef;
    playerFixtureDef.shape = &playerBox;
    playerFixtureDef.density = 1.0f;
    playerFixtureDef.friction = 0.0f;
    playerFixtureDef.userData = &playerBoxData;
    body_->CreateFixture(&playerFixtureDef);

    b2PolygonShape playerFootBox;
    b2Vec2 footCenter;
    footCenter.Set(0.0f, boxSize.y);
    playerFootBox.SetAsBox(boxSize.x, 0.2f, footCenter, 0.0f);

    b2FixtureDef playerFootFixtureDef;
    playerFootFixtureDef.isSensor = true;
    playerFootFixtureDef.shape = &playerFootBox;
    playerFootFixtureDef.userData = &playerFootData;
    body_->CreateFixture(&playerFootFixtureDef);





}

void Player::Update(float dt)
{
    float dx = 0.0f;
    float dy = 0.0f;

    //touche permettant de jump avec la condition d'être en contacte d'une platform
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && footContact_ >= 0)
    {
        //std::cout << "\njump";
        dy -= 4.0f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        dx -= 2.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            dx -= 3.5f;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        dx += 2.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            dx += 3.5f;
        }
    }

    auto velocity = body_->GetLinearVelocity();
    velocity.Set(dx * speed_, velocity.y);
    //calcule pour jump ... il manque quelque chose.
    auto jumpVelocity = body_->GetLinearVelocity();
    jumpVelocity.Set(velocity.x, dy * jumpVelocity_);
    body_->SetLinearVelocity(velocity);


    switch (state_)
    {
    case State::IDLE:
    {
        if (footContact_ == 0)
        {
            ChangeState(State::JUMP);
        }
        if (std::abs(dx) > deadZone_)
        {
            ChangeState(State::WALK);
        }
        if (footContact_ == 0)
        {
            ChangeState(State::JUMP);
        }
        break;

    }
    case State::WALK:
    {
        if (footContact_ == 0)
        {
            ChangeState(State::JUMP);
        }
        //flip le sprite à droite
        if (facingRight_ && dx < 0.0f)
        {
            Flip();
        }
        //flip le sprite à gauche
        if (!facingRight_ && dx > 0.0f)
        {
            Flip();
        }

        //switch les images pour la marche de sara
        walkAnimTimer_ += dt;
        if (walkAnimTimer_ > walkAnimPeriod_)
        {
            walkAnimIndex_++;
            if (walkAnimIndex_ >= walkTexture_.size())
            {
                walkAnimIndex_ = 0;
            }

            UpdateSpriteTexture(walkTexture_[walkAnimIndex_]);
            walkAnimTimer_ -= walkAnimPeriod_;
        }

        //permet de switch entre les images d'attente et de marche
        if (std::abs(dx) < deadZone_)
        {
            ChangeState(State::IDLE);
        }
        break;
    }
    case State::JUMP:
    {
        //permet de switch entre les image d'attente ou de marche avec le jump
        if (footContact_ > 0)
        {
            ChangeState(State::IDLE);
        }
        //flip le sprite à droite
        if (facingRight_ && dx < 0.0f)
        {
            Flip();
        }
        //flip le sprite à gauche
        if (!facingRight_ && dx > 0.0f)
        {
            Flip();
        }
        break;
    }
    default:;
    }
    const auto& b2Position = body_->GetPosition();
    sprite_.setPosition(sf::Vector2f(b2Position.x * game_.pixelToMeter, b2Position.y * game_.pixelToMeter));

}

void Player::Render(sf::RenderWindow& window)
{
    window.draw(sprite_);
}

sf::Vector2<float> Player::GetPosition() const
{
    return sprite_.getPosition();
}

void Player::BeginContactGround()
{
    footContact_++;
}

void Player::EndContactGround()
{
    footContact_--;
}

void Player::ChangeState(State state)
{
    switch (state)
    {
    case State::IDLE:
    {
        UpdateSpriteTexture(idleTexture_);
        break;
    }
    case State::WALK:
    {
        UpdateSpriteTexture(walkTexture_[0]);
        break;
    }
    case State::JUMP:
    {
        UpdateSpriteTexture(jumpTexture_);
        break;
    }
    default: break;
    }
    state_ = state;
}

void Player::UpdateSpriteTexture(const sf::Texture& texture)
{
    sprite_.setTexture(texture);
    const auto textureSize = texture.getSize();
    sprite_.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
}

void Player::Flip()
{
    facingRight_ = !facingRight_;
    auto scale = sprite_.getScale();
    scale.x = -scale.x;
    sprite_.setScale(scale);
}
