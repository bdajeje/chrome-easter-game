#include "map.hpp"

#include "managers/texturemanager.hpp"
#include "utils/graphics.hpp"
#include "utils/random.hpp"
#include <iostream>
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

  // Set bird sprite
  _bird.setTexture( texture::TextureManager::get("bird.png") );
  _bird.setPosition(0, _height - _bird.getGlobalBounds().height - 30);

  reset();
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

  // Move bird
  if( score > _bird_score_limit )
  {
     _bird.move(-move * _bird_speed_modificator, 0);

     // Bird goes out of screen, randomly re-place it
     const static float bird_screen_limit = - _bird.getGlobalBounds().width;
     if( _bird.getPosition().x < bird_screen_limit )
       placeBird();
  }
  std::cout << _bird.getPosition().x << std::endl;
  // Take care of trees (move and generate)
  updateTrees(move, score);
}

void Map::placeBird()
{ std::cout << "place bird !!!!!!!!!!!!!!!1" << std::endl;
//  _bird.move(_width * utils::random<int>(1, 3), 0);
  _bird.move(_width, 0);
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

  target.draw(_bird, states);
}

void Map::positionCloud(sf::Sprite& cloud, unsigned int x_offset)
{
  static int max_height = static_cast<int>((_height - _backgrounds[0].getGlobalBounds().height - clouds_margin_bottom) - cloud.getGlobalBounds().height);
  const int x = utils::random<int>(0, _width * 2);
  const int y = utils::random<int>(0, max_height);
  cloud.setPosition(x + x_offset, y);
}

unsigned int Map::randomTreeHeight()
{  
  return (utils::random<int>(0, 10) >= 5) ? 40 : 30;
}

bool Map::isCollision(const Player& player) const
{
  return isTreeCollision(player) || isBirdCollision(player);
}

bool Map::isCollide(float a_left, float a_right, float b_left, float b_right)
{
  // On the right of the range, so no collision
  if( a_left > b_right || a_right < b_left )
    return false;

  if( a_right > b_left || a_left < b_right )
    return true;

  return false;
}

bool Map::isTreeCollision(const Player& player) const
{
  // Only check first tree
  const sf::Sprite& tree           = _trees.front();
  const sf::Vector2f& tree_pos     = tree.getPosition();
  const sf::Vector2f& player_pos   = player.getPosition();
  const sf::FloatRect& tree_bounds = tree.getGlobalBounds();

  // If higher than tree, no collision
  if( player_pos.y < tree_pos.y - tree_bounds.height )
    return false;

  return isCollide(player_pos.x, player_pos.x + player.getWidth(),
                   tree_pos.x, tree_pos.x + tree_bounds.width);
}

bool Map::isBirdCollision(const Player& player) const
{
  if(player.isCrouched())
    return false;

  const sf::Vector2f player_pos = player.getPosition();
  const sf::Vector2f bird_pos   = _bird.getPosition();
  return isCollide( player_pos.x, player_pos.x + player.getWidth(),
                    bird_pos.x, bird_pos.x + _bird.getGlobalBounds().width );
}

void Map::reset()
{
  // Remove all trees
  _trees.clear();

  // Add initial tree
  generateTree(0);

  // Re-place bird
  placeBird();
}

}
