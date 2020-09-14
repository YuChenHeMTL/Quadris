#include "JBlock.h"

// Private Constructor
JBlock::JBlock(bool isHeavy, int levelGenerated) : Block(isHeavy, levelGenerated, 'J') {
  std::vector<int> cell1 = {3, 0};
  std::vector<int> cell2 = {4, 0};
  std::vector<int> cell3 = {4, 1};
  std::vector<int> cell4 = {4, 2};
  occupiedCellPositions_ = {cell1, cell2, cell3, cell4};
}

// Create method used for BlockFactory since the constructor is private
std::shared_ptr<Block> JBlock::Factory::create(bool isHeavy, int levelGenerated) {
  return std::shared_ptr<JBlock>(new JBlock(isHeavy, levelGenerated));
}