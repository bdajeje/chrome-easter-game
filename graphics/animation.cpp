#include "animation.hpp"

#include <iostream>

#include "managers/texturemanager.hpp"

namespace graphics {

Animation::Animation(const std::string& textures_filename, const sf::Vector2f& position,
                     unsigned int texture_width, unsigned int texture_height, unsigned int sprite_time)
  : _texture_width{texture_width}
  , _texture_height{texture_height}
  , _time_between_textures {sprite_time}
{
  _sprite.setTexture( texture::TextureManager::get(textures_filename) );
  _sprite.setPosition( position );
  updateSprite();
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(_sprite, states);
}

void Animation::update(const sf::Time& elapsed_time)
{
  // Do we need a sprite update
  bool need_update = false;
  if( elapsed_time.asMilliseconds() - _elapsed_time >= _time_between_textures )
    need_update = true;

  _elapsed_time += elapsed_time.asMilliseconds();

  if(need_update)
    updateSprite();
}

void Animation::updateSprite()
{
  int texture_nbr = _elapsed_time / _time_between_textures;
  uint x_offset = (texture_nbr * _texture_width);

  if(x_offset >= _sprite.getTexture()->getSize().x)
  {
    _elapsed_time = 0;
    x_offset = 0;
  }

  _sprite.setTextureRect(sf::IntRect(x_offset, 0, _texture_width, _texture_height));
}

}
