#include <iostream>

#include <SFML/Graphics.hpp>

#include "managers/fontmanager.hpp"
#include "managers/texturemanager.hpp"
#include "models/map.hpp"
#include "models/hud.hpp"
#include "models/player.hpp"
#include "utils/timer.hpp"
#include "utils/file.hpp"

constexpr unsigned int Window_width {600};
constexpr unsigned int Window_height {160};
constexpr unsigned int Player_x_pos {20};
constexpr unsigned int Player_y_pos {120};

int main()
{
  // Create window
  sf::RenderWindow window(sf::VideoMode(Window_width, Window_height), "AI Chrome Easter Game");
  window.setFramerateLimit(60);

  // Init managers
  texture::TextureManager::init("resources/textures/");
  font::FontManager::init("resources/fonts/");

  // Init random
  srand(time(NULL));

  // Read highest score
  const std::string player_filepath {"player/highest"};
  unsigned int highest_score {0};
  std::string highest_score_str;
  if( utils::files::read(player_filepath, highest_score_str) )
  {
    try {
      highest_score = std::stoul(highest_score_str);
    }
    catch(...)
    {
      std::cerr << "Wrong player file" << std::endl;
    }
  }

  // Models creation
  models::Map map {Window_width, Window_height};
  models::Player player { sf::Vector2f{Player_x_pos, Player_y_pos} };
  models::HUD hud {highest_score, Window_width, Window_height};

  // Game loop
  const sf::Color background_color {247, 247, 247};
  utils::time::Timer timer;
  bool game_started {false};
  while(window.isOpen())
  {
    sf::Event event;
    while(window.pollEvent(event))
    {
      // Remove compiler warnings about some enumeration values not handled in switch
      #pragma GCC diagnostic push
      #pragma GCC diagnostic ignored "-Wswitch"
      switch(event.type)
      {
        case sf::Event::Closed:
        {
          // Save score
          uint score = hud.getScore();
          if( score > highest_score )
            utils::files::create(player_filepath, std::to_string(score), true);

          // Close
          window.close();
          break;
        }
        case sf::Event::KeyPressed:
        {
          if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
          {
            // Game already started, player jump
            if(game_started)
              player.jump();
            // Start the game
            else
            {
              game_started = true;
              timer.start();
              hud.setDrawStarting(false);
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
      map.update(elapsed_time, hud.getScore());
      player.update(elapsed_time);
      hud.update(elapsed_time);
    }

    // Draw
    window.clear(background_color);
    window.draw(map);
    window.draw(player);
    window.draw(hud);
    window.display();
  }

  return 0;
}
