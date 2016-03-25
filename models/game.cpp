#include "game.hpp"

#include "utils/timer.hpp"

namespace models {

Game::Game(sf::RenderWindow& window, sf::Vector2u game_size, uint highest_score)
  : _window {window}
  , _map {game_size.x, game_size.y}
  , _player { sf::Vector2f{Player_x_pos, Player_y_pos} }
  , _hud {highest_score, game_size.x, game_size.y}
{}

uint Game::start()
{
  bool game_started {false};
  const sf::Color background_color {247, 247, 247};
  utils::time::Timer timer;

  while(_window.isOpen())
  {
    sf::Event event;
    while(_window.pollEvent(event))
    {
      // Remove compiler warnings about some enumeration values not handled in switch
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wswitch"
      switch(event.type)
      {
        case sf::Event::Closed:
        {
          return _hud.getScore();
        }
        case sf::Event::KeyPressed:
        {
          if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
          {
            // Game already started, player jump
            if(game_started)
              _player.jump();
            // Start the game
            else
            {
              game_started = true;
              timer.start();
              _hud.setDrawStarting(false);
            }
          }
        }
      }
    }

    if(game_started)
    {
      // Get elapsed time since last update
      const sf::Time elapsed_time = timer.restart();

      // Update models
      _map.update(elapsed_time, _hud.getScore());
      _player.update(elapsed_time);
      _hud.update(elapsed_time);

      // Is there collision
      const sf::Vector2f& position = _player.getPosition();
      if(_map.isTreeBetween(position.x, position.x + _player.getWidth()))
      {
        game_started = false;
        _hud.setDrawGameOver(true);
      }
    }

    // Draw
    _window.clear(background_color);
    _window.draw(_map);
    _window.draw(_player);
    _window.draw(_hud);
    _window.display();
  }

  return _hud.getScore();
}

}
