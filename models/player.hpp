#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

#include "graphics/animation.hpp"

namespace models {

class Player final : public sf::Drawable
{
  public:

    Player(const sf::Vector2f& starting_pos);

    void update(const sf::Time& elapsed_time);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void reset();

    void jump();
    void standUp();
    void standDown();

    bool isCrouched() const { return _current_animation == &_animation_down; }

    const sf::Vector2f& getPosition() const { return _sprite.getPosition(); }
    uint getWidth() const { return _sprite.getGlobalBounds().width; }

  private:

    void setAnimation(graphics::Animation& animation);
    bool isJumping() const;

  private:

    // Player default and starting position
    float _default_y_pos;

    // Target position to reach
    float _target_y_position;

    // Player sprite
    sf::Sprite _sprite;

    // Player stands up animation
    graphics::Animation _animation_up;

    // Player stands down animation
    graphics::Animation _animation_down;

    // Current animation
    graphics::Animation* _current_animation {nullptr};

    // Pixels / ms
    float _speed {0.25};

    // Jump height
    static constexpr float _jump_height {80};
};

}

#endif // PLAYER_HPP
