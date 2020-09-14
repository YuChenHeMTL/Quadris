#include "Board.h"

#include <cassert>
#include <memory>

#include "BlockFactory.h"

using namespace std;

const bool DEV_TESTING = false;

// Constructor intializes all Cells in the board
Board::Board(Game *game) : game_(game) {
  for (int i = 0; i < boardRowSize_; i++) {
    for (int j = 0; j < boardColumnSize_; j++) {
      grid_[i][j] = make_shared<Cell>(i, j);
    }
  }
}

// Destructor doesn't need to do anything
// since all ownership pointers are smart pointers
Board::~Board() {}

// Takes in a vector of cell positions and checks if the positions are valid
// That those cells are in bounds and currently empty in the board
// Used for movement methods
bool Board::isValidMove(vector<vector<int>> newPositions) {
  for (auto &v : newPositions) {  // if it is out of bounds, or the cell is not empty and it is not the hint block
    if (v[0] < 0 || v[0] >= boardRowSize_ || v[1] < 0 || v[1] >= boardColumnSize_ || getCell(v[0], v[1])->isNotEmpty()) {
      return false;
    }
  }
  return true;
}

// Spawns the block in the board if there is space, otherwise it is game over
void Board::spawnBlock(shared_ptr<Block> block) {
  vector<vector<int>> positions = block->getOccupiedCellPositions();

  // If there is no space for the block to spawn, the game is over
  if (!isValidMove(positions)) {
    cout << "Game Over!" << endl;
    throw std::logic_error("game_over");
  }

  // If there is space, spawn the block in the board
  for (auto &p : positions) {
    if (DEV_TESTING) {
      std::cout << "TEST (Board): Spawning Block in Cell " << p[0] << " " << p[1] << std::endl;
    }
    getCell(p[0], p[1])->setBlock(block);
  }
}

// Spawns the hint block on the board
void Board::spawnHintBlock(shared_ptr<Block> block) {
  vector<vector<int>> positions = block->getOccupiedCellPositions();
  vector<vector<int>> new_positions;

  vector<vector<int>> current_block_positions = game_->get_current_block()->getOccupiedCellPositions();
  // If there is space, spawn the block in the board
  for (auto &p : positions) {
    if (DEV_TESTING) {
      std::cout << "TEST (Board): Attempting to Spawn Hint in Cell " << p[0] << " " << p[1] << std::endl;
    }

    bool hintBlockInterfereswithCurrent = false;

    for (unsigned int i = 0; i < current_block_positions.size(); ++i) {
      if (p[0] == current_block_positions[i][0] && p[1] == current_block_positions[i][1]) {
        hintBlockInterfereswithCurrent = true;

        if (DEV_TESTING) {
          std::cout << "TEST (Board): Not Spawning Hint in Cell " << p[0] << " " << p[1] << std::endl;
        }

        break;
      }
    }

    if (!hintBlockInterfereswithCurrent) {
      getCell(p[0], p[1])->setBlock(block);
      new_positions.push_back({p[0], p[1]});
    }
  }

  block->setOccupiedCellPositions(new_positions);
}

// Clears any full lines and returns the number of lines cleared
int Board::clearLine() {
  vector<int> fullRows;
  // Loop through board, finds which rows need to be cleared (are full)
  for (int row = 0; row < boardRowSize_; row++) {
    int numCellsEmpty = 0;
    for (int column = 0; column < boardColumnSize_; column++) {
      if (!getCell(row, column)->isNotEmpty()) {
        numCellsEmpty++;
      }
    }
    if (numCellsEmpty == 0) {
      fullRows.push_back(row);
    }
  }

  // Clears all the cells in each row
  for (auto &row : fullRows) {
    for (int column = 0; column < boardColumnSize_; column++) {
      getCell(row, column)->clear();
    }
  }

  // For every row that has been cleared
  for (auto &row : fullRows) {
    // For every row above the cleared row, shift it down by one row
    for (int rowAbove = row - 1; rowAbove >= 0; rowAbove--) {
      moveRowDown(rowAbove);
    }
  }

  // Returns the number of full rows that have been cleared (used for scoring)
  return fullRows.size();
}

/////////////////////////////////////
// Private helpers
/////////////////////////////////////

// Clears the board (used for restart)
void Board::clearBoard() {
  for (int row = 0; row < boardRowSize_; row++) {
    for (int column = 0; column < boardColumnSize_; column++) {
      std::shared_ptr<Block> cellBlock = getCell(row, column)->getBlock();
      if (cellBlock) {
        cellBlock->setCountsForPoints(false);
      }
      getCell(row, column)->clear();
    }
  }
}

// Moves all the cells in the specified row down by one row
void Board::moveRowDown(int row) {
  for (int column = 0; column < boardColumnSize_; column++) {
    shared_ptr<Block> curBlock = getCell(row, column)->getBlock();
    getCell(row, column)->clear();

    // Assert that the cell below is empty
    assert(!getCell(row + 1, column)->isNotEmpty());

    getCell(row + 1, column)->setBlock(curBlock);
  }
}

////////////////////////////////////////////////////////////////////////////////
// Getters and Setters
////////////////////////////////////////////////////////////////////////////////

int Board::getBoardColumnSize() {
  return boardColumnSize_;
}

int Board::getBoardRowSize() {
  return boardRowSize_;
}

// Returns the cell at (row,column) in the board
shared_ptr<Cell> Board::getCell(int row, int column) {
  assert((row >= 0) && (row < boardRowSize_) && (column >= 0) && (column < boardColumnSize_));
  return grid_[row][column];
}