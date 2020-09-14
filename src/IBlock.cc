#include "IBlock.h"

// Private Constructor
IBlock::IBlock(bool isHeavy, int levelGenerated) : Block(isHeavy, levelGenerated, 'I') {
  std::vector<int> cell1 = {3, 0};
  std::vector<int> cell2 = {3, 1};
  std::vector<int> cell3 = {3, 2};
  std::vector<int> cell4 = {3, 3};
  occupiedCellPositions_ = {cell1, cell2, cell3, cell4};
}

// Create method used for BlockFactory since the constructor is private
std::shared_ptr<Block> IBlock::Factory::create(bool isHeavy, int levelGenerated) {
  return std::shared_ptr<IBlock>(new IBlock(isHeavy, levelGenerated));
}

// IBlock rotate is different than other blocks
// Requires overriden rotate function
void IBlock::rotate(bool clockwise) {
  std::shared_ptr<Board> board = board_.lock();
  std::shared_ptr<Block> currentBlock;

  std::vector<int> boundingBox = boundingRect(occupiedCellPositions_);
  std::vector<int> pivot;

  if (boundingBox[0] < boundingBox[1]) {
    clockwise = true;
  } else {
    clockwise = false;
  }

  pivot = {boundingBox[1], boundingBox[2]};

  // Gets the current position of the cells in the block and creates a vector newPositions
  // which contains the position of the cells after the block is rotated
  std::vector<std::vector<int>> newPositions;
  for (auto &p : occupiedCellPositions_) {
    currentBlock = board->getCell(p[0], p[1])->getBlock();
    std::vector<int> cellRowColumn;

    int shiftedRow = p[0] - pivot[0];
    int shiftedColumn = p[1] - pivot[1];

    // newRow = shiftedColumn + pivotRow
    // newColumn = shiftRow*-1 + pivotColumn
    if (clockwise) {
      cellRowColumn.push_back(shiftedColumn + pivot[0]);
      cellRowColumn.push_back((shiftedRow * -1) + pivot[1]);
    } else {
      cellRowColumn.push_back((shiftedColumn * -1) + pivot[0]);
      cellRowColumn.push_back(shiftedRow + pivot[1]);
    }

    newPositions.push_back(cellRowColumn);
    board->getCell(p[0], p[1])->clear();
  }

  // Checks if the move is valid, if so move the block on the board
  if (board->isValidMove(newPositions))  // If the move is valid
  {
    occupiedCellPositions_.clear();  // Clears the old cell positions in the occupiedCells_ vector
    for (auto &c : newPositions) {
      board->getCell(c[0], c[1])->setBlock(currentBlock);  // Updates the new cells to have a reference to the current block
    }
    occupiedCellPositions_ = newPositions;

    // Otherwise, the move is not valid and so restore the cells to their original position
  } else {
    for (auto &c : occupiedCellPositions_) {
      board->getCell(c[0], c[1])->setBlock(currentBlock);
    }
  }
  moveDownIfHeavy();
}