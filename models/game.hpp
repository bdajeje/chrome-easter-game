#ifndef GAME_HPP
#define GAME_HPP

#include "models/map.hpp"
#include "models/hud.hpp"
#include "models/player.hpp"

namespace models {

class Game final
{
  public:

    Game(sf::RenderWindow& window, sf::Vector2u game_size, uint highest_score);

    uint start();

  private:

    sf::RenderWindow& _window;
    Map _map;
    Player _player;
    HUD _hud;

    static constexpr unsigned int Player_x_pos {20};
    static constexpr unsigned int Player_y_pos {120};
};

}

#endif // GAME_HPP
