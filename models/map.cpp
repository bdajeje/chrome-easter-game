#include "map.hpp"

#include "managers/texturemanager.hpp"
#include "utils/graphics.hpp"

namespace models {

Map::Map(unsigned int width, unsigned int height, unsigned short nbr_clouds)
  : _width {static_cast<int>(width)}
  , _height {height}
{
  // Set backgrounds
  for(size_t i = 0; i < _backgrounds.size(); ++i)
  {
    sf::Sprite& background = _backgrounds[i];
    background.setTexture( texture::TextureManager::get("background.png") );
    const sf::FloatRect bounds = background.getGlobalBounds();
    background.setPosition(i * bounds.width, height - bounds.height);
  }

  // Generate clouds
  const sf::Texture& cloud_texture = texture::TextureManager::get("cloud.png");
  _clouds.reserve(nbr_clouds);
  for(unsigned short i = 0; i < nbr_clouds; ++i)
  {
    _clouds.emplace_back( cloud_texture );
    positionCloud(_clouds.back(), 0);
  }

  // Create first tree
  generateTree(0);
}

void Map::update(const sf::Time& elapsed_time, uint score)
{
  // How many pixels of moving
  float move = elapsed_time.asMilliseconds() * _speed;

  // Move backgrounds
  for(sf::Sprite& background : _backgrounds)
  {
    background.move(-move, 0);
    if(background.getPosition().x < -_width )
      background.move(_width * 2, 0);
  }

  // Move clouds
  for( sf::Sprite& cloud : _clouds )
  {
    cloud.move(-move, 0);
    if(cloud.getPosition().x < -cloud.getGlobalBounds().width)
      positionCloud(cloud, _width);
  }

  // Take care of trees (move and generate)
  updateTrees(move, score);
}

void Map::updateTrees(float move, uint score)
{
  // Check if most left tree is out of screen, if yes, remove it, and generate a new one
  sf::Sprite& tree = _trees.front();
  if(tree.getPosition().x < -tree.getGlobalBounds().width)
  {
    _trees.pop_front();
    generateTree(score);
  }

  // Move trees
  for( sf::Sprite& tree : _trees )
    tree.move(-move, 0);
}

void Map::generateTree(uint score)
{
  // Depending on current score, how maximum width the tree could be
  unsigned short max_tree_width;
  if(score < 100)
    max_tree_width = 1;
  else if(score < 300)
    max_tree_width = 2;
  else
    max_tree_width = 3;

  _trees.emplace_back( texture::TextureManager::get("tree.png") );
  sf::Sprite& tree = _trees.back();
  const float random_height = randomTreeHeight();
  utils::graphics::resize(tree, tree.getGlobalBounds().width, random_height);
  tree.setPosition(_width, _height - random_height);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  for(const sf::Sprite& background : _backgrounds)
    target.draw(background, states);

  for( const sf::Sprite& cloud : _clouds )
    target.draw(cloud, states);

  for( const sf::Sprite& tree : _trees )
    target.draw(tree, states);
}

void Map::positionCloud(sf::Sprite& cloud, unsigned int x_offset)
{
  static int max_height = static_cast<int>((_height - _backgrounds[0].getGlobalBounds().height - clouds_margin_bottom) - cloud.getGlobalBounds().height);
  const int x = rand() % _width * 2;
  const int y = rand() % max_height;
  cloud.setPosition(x + x_offset, y);
}

unsigned int Map::randomTreeHeight()
{
  return (rand() % 10 >= 5) ? 40 : 30;
}

bool Map::isTreeBetween(float min, float max) const
{
  for( const sf::Sprite& tree : _trees )
  {
    const sf::Vector2f& position = tree.getPosition();

    // On the right of the range, so no collision
    if( position.x > max )
      continue;

    const sf::FloatRect& bounds = tree.getGlobalBounds();

    // On the left of the range, so no collision
    const float tree_right_bound = position.x + bounds.width;
    if( tree_right_bound < min )
      continue;

    if( position.x >= min || tree_right_bound < max )
      return true;
  }

  return false;
}

}
