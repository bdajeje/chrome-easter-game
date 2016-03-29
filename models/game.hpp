#ifndef GAME_HPP
#define GAME_HPP

#include "models/map.hpp"
#include "models/hud.hpp"
#include "models/player.hpp"
#include "utils/timer.hpp"

namespace models {

class Game final
{
  public:

    Game(sf::RenderWindow& window, sf::Vector2u game_size);

    void start();

  private:

    void restart();
    void saveScore();
    void updatePlayerState();

  private:

    sf::RenderWindow& _window;

    Map _map;
    Player _player;
    HUD _hud;

    uint _highest_score {0};
    bool _game_started {false};
    utils::time::Timer _timer;

    static constexpr unsigned int Player_x_pos {20};
    static constexpr unsigned int Player_y_pos {120};
    static const std::string _player_filepath;
};

}

#endif // GAME_HPP
