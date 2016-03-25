#include <iostream>

#include <SFML/Graphics.hpp>

#include "managers/fontmanager.hpp"
#include "managers/texturemanager.hpp"
#include "models/game.hpp"
#include "utils/file.hpp"

int main()
{
  // Read highest score
  const std::string player_filepath {"player/highest"};
  uint highest_score {0};
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

  // Create window
  sf::RenderWindow window(sf::VideoMode(600, 160), "AI Chrome Easter Game");
  window.setFramerateLimit(60);

  // Init random
  srand(time(NULL));

  // Init managers
  texture::TextureManager::init("resources/textures/");
  font::FontManager::init("resources/fonts/");

  // Game loop
  models::Game game {window, window.getSize(), highest_score};
  uint score = game.start();

  // Save score
  if( score > highest_score )
    utils::files::create(player_filepath, std::to_string(score), true);

  return EXIT_SUCCESS;
}
