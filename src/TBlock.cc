#include "TBlock.h"

// Private Constructor
TBlock::TBlock(bool isHeavy, int levelGenerated) : Block(isHeavy, levelGenerated, 'T') {
  std::vector<int> cell1 = {3, 0};
  std::vector<int> cell2 = {3, 1};
  std::vector<int> cell3 = {3, 2};
  std::vector<int> cell4 = {4, 1};
  occupiedCellPositions_ = {cell1, cell2, cell3, cell4};
}

// Create method used for BlockFactory since the constructor is private
std::shared_ptr<Block> TBlock::Factory::create(bool isHeavy, int levelGenerated) {
  return std::shared_ptr<TBlock>(new TBlock(isHeavy, levelGenerated));
}