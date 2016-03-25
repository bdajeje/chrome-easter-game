#include "player.hpp"

namespace models {

Player::Player(const sf::Vector2f& starting_pos)
  : _default_y_pos {starting_pos.y}
  , _target_y_position {starting_pos.y}
  , _animation {"player.png", starting_pos, 40, 43, 200}
{}

void Player::update(const sf::Time& elapsed_time)
{
  _animation.update(elapsed_time);

  // If player needs to reach a position
  const sf::Vector2f& position = _animation.getPosition();
  if( position.y != _target_y_position )
  {
    float move = elapsed_time.asMilliseconds() * _speed;
    // Going up
    if( _animation.getPosition().y > _target_y_position )
    {
      _animation.setPosition(position.x, std::max(position.y - move, _target_y_position));

      // If we reach max high, now let's go down
      if( position.y == _target_y_position )
        _target_y_position = _default_y_pos;
    }
    // Going down
    else
      _animation.setPosition(position.x, std::min(position.y + move, _target_y_position));
  }
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(_animation);
}

void Player::jump()
{
  // If player's still in the air, exit
  if(_animation.getPosition().y != _default_y_pos)
    return;

  _target_y_position = _default_y_pos - _jump_height;
}

}
