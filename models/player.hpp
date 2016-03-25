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

    void jump();

  private:

    // Player default and starting position
    float _default_y_pos;

    // Target position to reach
    float _target_y_position;

    // Player sprite
    graphics::Animation _animation;

    // Pixels / ms
    float _speed {0.3};

    // Jump height
    static constexpr float _jump_height {80};
};

}

#endif // PLAYER_HPP
