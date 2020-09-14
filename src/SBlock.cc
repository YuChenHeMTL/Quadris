#include "SBlock.h"

// Private Constructor
SBlock::SBlock(bool isHeavy, int levelGenerated) : Block(isHeavy, levelGenerated, 'S') {
  std::vector<int> cell1 = {3, 1};
  std::vector<int> cell2 = {3, 2};
  std::vector<int> cell3 = {4, 0};
  std::vector<int> cell4 = {4, 1};
  occupiedCellPositions_ = {cell1, cell2, cell3, cell4};
}

// Create method used for BlockFactory since the constructor is private
std::shared_ptr<Block> SBlock::Factory::create(bool isHeavy, int levelGenerated) {
  return std::shared_ptr<SBlock>(new SBlock(isHeavy, levelGenerated));
}