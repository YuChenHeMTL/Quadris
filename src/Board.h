#ifndef _BOARD_
#define _BOARD_

#include <iostream>
#include <memory>
#include <vector>

#include "Block.h"
#include "Cell.h"
#include "Game.h"

// Forward Declarations
class Cell;
class Block;
class Game;

class Board {
  friend class Game;

 public:
  // Constructor
  Board(Game *game);

  // Destructor
  ~Board();

  // Clears any full lines and returns the number of lines cleared
  int clearLine();

  // Given a block, it attempts to spawn it on the current gameBoard
  void spawnBlock(std::shared_ptr<Block>);

  // Spawns the hint block on the board
  void spawnHintBlock(std::shared_ptr<Block>);

  // Takes in a vector of cell positions and checks if the positions are valid
  // That those cells are in bounds and currently empty in the board
  // Used for movement methods
  bool isValidMove(std::vector<std::vector<int>>);

  ////////////////////////////////////////////////////////////////////////////////
  // Getters and Setters
  ////////////////////////////////////////////////////////////////////////////////

  int getBoardColumnSize();
  int getBoardRowSize();

  // Returns the cell at (row,column) in the board
  std::shared_ptr<Cell> getCell(int, int);

  /////////////////////////////////////////////////////////////////////////////////

 private:
  // Weak pointer to Game used to get the current block
  Game *game_;

  // Static board sizes
  static const int boardRowSize_ = 18;
  static const int boardColumnSize_ = 11;

  // 2D array of Cells that represent the game board
  std::shared_ptr<Cell> grid_[boardRowSize_][boardColumnSize_];

  /////////////////////////////////////
  // Private helpers
  /////////////////////////////////////

  // Clears the board (used for restart)
  void clearBoard();

  // Moves all cells in the specified row down by one row (used for clearLine)
  void moveRowDown(int);
};

#endif