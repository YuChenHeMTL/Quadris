#include "ZBlock.h"

// Private Constructor
ZBlock::ZBlock(bool isHeavy, int levelGenerated) : Block(isHeavy, levelGenerated, 'Z') {
  std::vector<int> cell1 = {3, 0};
  std::vector<int> cell2 = {3, 1};
  std::vector<int> cell3 = {4, 1};
  std::vector<int> cell4 = {4, 2};
  occupiedCellPositions_ = {cell1, cell2, cell3, cell4};
}

// Create method used for BlockFactory since the constructor is private
std::shared_ptr<Block> ZBlock::Factory::create(bool isHeavy, int levelGenerated) {
  return std::shared_ptr<ZBlock>(new ZBlock(isHeavy, levelGenerated));
}