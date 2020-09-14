#include "Block1X1.h"
using namespace std;

// Private Constructor
Block1X1::Block1X1(bool isHeavy, int levelGenerated) : Block(isHeavy, levelGenerated, '*') {
  //This should be the middle top position, ready to drop
  occupiedCellPositions_ = {{0, 5}};
}

// Create method used for BlockFactory since the constructor is private
std::shared_ptr<Block> Block1X1::Factory::create(bool isHeavy, int levelGenerated) {
  return std::shared_ptr<Block1X1>(new Block1X1(isHeavy, levelGenerated));
}