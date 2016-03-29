#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <list>

#include <SFML/Graphics.hpp>

namespace models {

class Map final : public sf::Drawable
{
  public:

    Map(unsigned int width, unsigned int height, unsigned short nbr_clouds = 8);

    void update(const sf::Time& elapsed_time, uint score);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void reset();

    bool isTreeBetween(float player_y, float x_min, float x_max) const;

  private:

    void positionCloud(sf::Sprite& cloud, unsigned int x_offset);
    void updateTrees(float move, uint score);
    void generateTree(uint score);
    static unsigned int randomTreeHeight();

  private:

    int _width; // we need an int here for -_width
    unsigned int _height;
    std::vector<sf::Sprite> _clouds; // \todo use an std::array ?
    std::array<sf::Sprite, 2> _backgrounds;
    std::list<sf::Sprite> _trees;

    // Pixels / ms
    float _speed {0.2};

    static constexpr unsigned int clouds_margin_bottom {20}; // in pixel
};

}

#endif // MAP_HPP
