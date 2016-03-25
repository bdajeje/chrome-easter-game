#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <string>

#include <SFML/Graphics.hpp>

namespace graphics {

class Animation final : public sf::Drawable
{
  public:

    Animation(const std::string& textures_filename, const sf::Vector2f& position,
              unsigned int texture_width, unsigned int texture_height, unsigned int sprite_time);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update(const sf::Time& elapsed_time);

    const sf::Vector2f& getPosition() const { return _sprite.getPosition(); }
    unsigned int getWidth() const { return _texture_width; }

    void setPosition(sf::Vector2f& position) { _sprite.setPosition(position); }
    void setPosition(float x, float y) { _sprite.setPosition(x, y); }

  private:

    void updateSprite();

  private:

    unsigned int _texture_width;
    unsigned int _texture_height;
    sf::Sprite _sprite;
    unsigned int _elapsed_time {0}; // milliseconds
    unsigned int _time_between_textures; // milliseconds
};

}

#endif // ANIMATION_HPP
