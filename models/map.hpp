#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <list>

#include <SFML/Graphics.hpp>

#include "models/player.hpp"

namespace models {

class Map final : public sf::Drawable
{
  public:

    Map(unsigned int width, unsigned int height, unsigned short nbr_clouds = 8);

    void update(const sf::Time& elapsed_time, uint score);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void reset();

    bool isCollision(const Player& player) const;

  private:

    void positionCloud(sf::Sprite& cloud, unsigned int x_offset);
    void updateTrees(float move, uint score);
    void generateTree(uint score);
    void placeBird();
    bool isBirdCollision(const Player& player) const;
    bool isTreeCollision(const Player& player) const;
    static unsigned int randomTreeHeight();
    static bool isCollide(float a_left, float a_right, float b_left, float b_right);

  private:

    int _width; // we need an int here to be able to get minus _width
    unsigned int _height;
    std::vector<sf::Sprite> _clouds; // \todo use an std::array ?
    std::array<sf::Sprite, 2> _backgrounds;
    std::list<sf::Sprite> _trees;
    sf::Sprite _bird;

    // Pixels / ms
    float _speed {0.2};

    static constexpr unsigned int clouds_margin_bottom {20}; // in pixel
    static constexpr uint _bird_score_limit {800}; // Birds are generated only if score's superior to this limit
    static constexpr float _bird_speed_modificator {2};
};

}

#endif // MAP_HPP
