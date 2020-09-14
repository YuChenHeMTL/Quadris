#include "LBlock.h"

// Private Constructor
LBlock::LBlock(bool isHeavy, int levelGenerated) : Block(isHeavy, levelGenerated, 'L') {
  std::vector<int> cell1 = {3, 2};
  std::vector<int> cell2 = {4, 0};
  std::vector<int> cell3 = {4, 1};
  std::vector<int> cell4 = {4, 2};
  occupiedCellPositions_ = {cell1, cell2, cell3, cell4};
}

// Create method used for BlockFactory since the constructor is private
std::shared_ptr<Block> LBlock::Factory::create(bool isHeavy, int levelGenerated) {
  return std::shared_ptr<LBlock>(new LBlock(isHeavy, levelGenerated));
}