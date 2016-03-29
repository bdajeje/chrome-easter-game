#include <iostream>

#include <SFML/Graphics.hpp>

#include "managers/fontmanager.hpp"
#include "managers/texturemanager.hpp"
#include "models/game.hpp"

int main()
{
  // Create window
  sf::RenderWindow window(sf::VideoMode(600, 160), "AI Chrome Easter Game");
  window.setFramerateLimit(60);

  // Init random
  srand(time(NULL));

  // Init managers
  texture::TextureManager::init("resources/textures/");
  font::FontManager::init("resources/fonts/");

  // Game loop
  models::Game game {window, window.getSize()};
  game.start();

  return EXIT_SUCCESS;
}
