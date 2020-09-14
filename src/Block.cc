#include "Block.h"

#include <assert.h>

#include <algorithm>
using namespace std;

const bool DEV_TESTING = false;

// Constructor
Block::Block(bool isHeavy, int levelGenerated, char character) : isHeavy_(isHeavy), levelGenerated_(levelGenerated), character_(character), countsForPoints_(false) {}

// Destructor
Block::~Block() {
  // If a block counts for points, once it is destroyed the respective bonus points should be added to the score
  if (countsForPoints_) {  // Things like hint block and replacing with I,L... commands don't count for points
    int blockPoints = (levelGenerated_ + 1) * (levelGenerated_ + 1);
    game_->addToScore(blockPoints);
  }
}

////////////////////////////////////////////////////////////////////////////////
// Block movement functions
////////////////////////////////////////////////////////////////////////////////

// Helper which moves the block by the amount specified in the paratemers (rows to move, columns to move)
// Returns true if the move is valid, else returns false
bool Block::moveBlock(int rowsToMove, int columnsToMove) {
  // Asserts that the block moves at most one cell in each direction
  assert(abs(rowsToMove) <= 1 && abs(columnsToMove) <= 1);

  shared_ptr<Board> board = board_.lock();
  shared_ptr<Block> currentBlock;

  // Gets the current position of the cells in the block and creates a vector newPositions
  // which contains the position of the cells after the block is moved

  vector<vector<int>> newPositions;
  for (auto &p : occupiedCellPositions_) {
    currentBlock = board->getCell(p[0], p[1])->getBlock();

    // Creates a vector that contains the new cell position (x,y)
    vector<int> cellRowColumn;
    cellRowColumn.push_back(p[0] + rowsToMove);
    cellRowColumn.push_back(p[1] + columnsToMove);

    // Adds the new position to the vector
    newPositions.push_back(cellRowColumn);

    // Clears the old cell (sets the block reference in the cell to nullptr)
    board->getCell(p[0], p[1])->clear();
  }

  // Checks if the move is valid, if so move the block on the board
  if (board->isValidMove(newPositions)) {
    // Sets the occupiedCells_ vector to the newPositions
    setOccupiedCellPositions(newPositions);

    for (auto &c : occupiedCellPositions_) {
      // Updates the new cells to have a reference to the current block
      board->getCell(c[0], c[1])->setBlock(currentBlock);
    }
    return true;

    // Otherwise, the move is not valid and so restore the cells to their original position
  } else {
    for (auto &c : occupiedCellPositions_) {
      board->getCell(c[0], c[1])->setBlock(currentBlock);
    }
    return false;
  }
}

// Move block left method
void Block::moveLeft() {
  moveBlock(0, -1);
  moveDownIfHeavy();
}

// Move block right method
void Block::moveRight() {
  moveBlock(0, 1);
  moveDownIfHeavy();
}

// Move block down method
void Block::moveDown() {
  moveBlock(1, 0);
  moveDownIfHeavy();
}

void Block::moveDownIfHeavy() {
  if (isHeavy_) {
    moveBlock(1, 0);
  }
}

// Drop block method
void Block::drop() {
  // Sets countsForPoints_ to be true
  // (if a block is dropped, it counts for points if it is cleared)
  countsForPoints_ = true;

  // Moves the block down until it cannot move anymore
  while (moveBlock(1, 0)) {
  }
}

// Rotate block function (true if clockwise, false if counterclockwise)
void Block::rotate(bool clockwise) {
  shared_ptr<Board> board = board_.lock();
  shared_ptr<Block> currentBlock;

  // Gets the smallest rectangle containing the block
  vector<int> boundingBox = boundingRect(occupiedCellPositions_);
  vector<int> pivot;

  // Need to adjust row and column sometimes by 1 cell
  // to preserve the position of the lower left corner of the block
  int rowAdjustment = 0;
  int columnAdjustment = 0;

  // Calculates the pivot for the current block and orientation
  if (boundingBox[1] - boundingBox[0] > boundingBox[3] - boundingBox[2]) {
    pivot = {(boundingBox[0] + boundingBox[1]) / 2, boundingBox[3]};
    if (clockwise) {
      rowAdjustment = 1;
    }
  } else {
    pivot = {boundingBox[0], (boundingBox[2] + boundingBox[3]) / 2};
    if (!clockwise) {
      columnAdjustment = -1;
    }
  }

  // Gets the current position of the cells in the block and creates a vector newPositions
  // which contains the position of the cells after the block is rotated
  vector<vector<int>> newPositions;
  for (auto &p : occupiedCellPositions_) {
    currentBlock = board->getCell(p[0], p[1])->getBlock();
    vector<int> cellRowColumn;

    // Shifts cells by moving pivot to origin
    int shiftedRow = p[0] - pivot[0];
    int shiftedColumn = p[1] - pivot[1];

    // Transposes cells and then reflects based on the respective rotation matrix
    // Shifts cells back to original pivot position
    // newRow = shiftedColumn + pivotRow
    // newColumn = shiftRow*-1 + pivotColumn
    if (clockwise) {
      cellRowColumn.push_back(shiftedColumn + pivot[0] + rowAdjustment);
      cellRowColumn.push_back((shiftedRow * -1 + columnAdjustment) + pivot[1]);
    } else {
      cellRowColumn.push_back(shiftedColumn * -1 + pivot[0] + rowAdjustment);
      cellRowColumn.push_back(shiftedRow + pivot[1] + columnAdjustment);
    }

    newPositions.push_back(cellRowColumn);
    board->getCell(p[0], p[1])->clear();
  }

  // Checks if the move is valid, if so move the block on the board
  if (board->isValidMove(newPositions)) {
    // Clears the old cell positions in the occupiedCells_ vector
    occupiedCellPositions_.clear();
    for (auto &c : newPositions) {
      // Updates the new cells to have a reference to the current block
      board->getCell(c[0], c[1])->setBlock(currentBlock);
    }
    setOccupiedCellPositions(newPositions);

    // Otherwise, the move is not valid and so restore the cells to their original position
  } else {
    for (auto &c : occupiedCellPositions_) {
      board->getCell(c[0], c[1])->setBlock(currentBlock);
    }
  }
  moveDownIfHeavy();
}

// Helper that returns the coordinates of the smallest rectangle containing the block
// Used for rotations
vector<int> Block::boundingRect(vector<vector<int>> positions) {
  // Initializes all rectangle coordinates
  int minRow = positions[0][0];
  int maxRow = positions[0][0];
  int minColumn = positions[0][1];
  int maxColumn = positions[0][1];

  // Loops through cells and updates the rectangle bounds if needed
  for (auto &p : positions) {
    minRow = min(minRow, p[0]);
    maxRow = max(maxRow, p[0]);
    minColumn = min(minColumn, p[1]);
    maxColumn = max(maxColumn, p[1]);
  }

  // Returns a vector with the bounds of the rectangle
  return {
      minRow,
      maxRow,
      minColumn,
      maxColumn};
}

////////////////////////////////////////////////////////////////////////////////
// Getters and Setters
////////////////////////////////////////////////////////////////////////////////

// Gets the vector of current cell positions that contain this block
vector<vector<int>> Block::getOccupiedCellPositions() {
  return occupiedCellPositions_;
}

// Gets the character that represents this block
char Block::getCharacter() {
  return character_;
}

// Returns whether the block is heavy or not
bool Block::isHeavy() {
  return isHeavy_;
}

// Sets the board_ weakptr
void Block::setBoard(std::shared_ptr<Board> board) {
  board_ = board;
}

// Sets the Game* weak pointer
void Block::setGame(Game *game) {
  game_ = game;
}

// Sets countsforPoints for this block
void Block::setCountsForPoints(bool counts_for_points) {
  countsForPoints_ = counts_for_points;
}

// Sets the occupiedCellPositions vector
void Block::setOccupiedCellPositions(std::vector<std::vector<int>> newPositions) {
  occupiedCellPositions_ = newPositions;
}

// Sets the heavy attribute of the block
void Block::setHeavy(bool make_heavy) {
  isHeavy_ = make_heavy;
}

////////////////////////////////////////////////////////////////////////////////
// Hint Block Movement Helpers
////////////////////////////////////////////////////////////////////////////////

void Block::makeHintBlock() {
  character_ = '?';
}

bool Block::moveBlockNoUpdate(int up_or_down, int left_or_right, bool force_move) {
  assert((abs(left_or_right) == 1) ^ (abs(up_or_down) == 1));

  shared_ptr<Board> board = board_.lock();
  vector<vector<int>> newPositions;
  for (auto &p : occupiedCellPositions_) {
    vector<int> cellRowColumn;
    cellRowColumn.push_back(p[0] + up_or_down);
    cellRowColumn.push_back(p[1] + left_or_right);
    newPositions.push_back(cellRowColumn);
  }

  if (board->isValidMove(newPositions) || force_move) {  // If the move is valid
    setOccupiedCellPositions(newPositions);
    return true;
  } else {
    return false;
  }
}

void Block::dropBlockNoUpdate(bool force_move) {
  while (moveBlockNoUpdate(1, 0, force_move)) {
  }
}
