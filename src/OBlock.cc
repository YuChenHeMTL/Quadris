#include "OBlock.h"

// Private Constructor
OBlock::OBlock(bool isHeavy, int levelGenerated) : Block(isHeavy, levelGenerated, 'O') {
  std::vector<int> cell1 = {3, 0};
  std::vector<int> cell2 = {3, 1};
  std::vector<int> cell3 = {4, 0};
  std::vector<int> cell4 = {4, 1};
  occupiedCellPositions_ = {cell1, cell2, cell3, cell4};
}

// Create method used for BlockFactory since the constructor is private
std::shared_ptr<Block> OBlock::Factory::create(bool isHeavy, int levelGenerated) {
  return std::shared_ptr<OBlock>(new OBlock(isHeavy, levelGenerated));
}

// Oblock rotate has no effect since it is a square
void OBlock::rotate(bool) {
  moveDownIfHeavy();
  return;
}