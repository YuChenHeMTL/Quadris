#ifndef _BLOCK_
#define _BLOCK_

#include <memory>
#include <vector>

#include "Board.h"
#include "Cell.h"
#include "Game.h"

class Cell;
class Board;
class Game;

class Block {
 public:
  // Constructor
  Block(bool isHeavy, int levelGenerated, char character);
  // Destructor
  ~Block();

  // Getters
  char getCharacter();
  std::vector<std::vector<int>> getOccupiedCellPositions();
  bool isHeavy();

  // Setters
  void setBoard(std::shared_ptr<Board>);
  void setGame(Game*);
  void setCountsForPoints(bool counts_for_points);
  void setOccupiedCellPositions(std::vector<std::vector<int>> newPositions);
  void setHeavy(bool make_heavy);

  // Block movement functions
  void moveLeft();
  void moveRight();
  void moveDown();
  void drop();
  virtual void rotate(bool);

  // Hint functions
  void hint();
  void makeHintBlock();

  //THESE ARE USED FOR NAVIGATING HINT ONLY, DO NOT USE THEM TO MOVE CURRENT BLOCK
  bool moveBlockNoUpdate(int up_or_down = 0, int left_or_right = 0, bool force_move = false);
  void dropBlockNoUpdate(bool force_move = false);

 protected:
  // Vector of the positions of the cells which currently contain this block
  std::vector<std::vector<int>> occupiedCellPositions_;

  // Whether or not the block is heavy
  bool isHeavy_;

  // The level at which the block was generated, used for scoring
  int levelGenerated_;

  // The character denoted by the block
  char character_;

  // Determines whether or not destruction of block counts for points (E.g. hint block does not count)
  bool countsForPoints_;

  // Holds a weak_ptr to the board to be able to access the grid for block movements
  std::weak_ptr<Board> board_;

  // Holds a weak pointer in the form of a raw pointer to Game, to be able to update scoring once the block is destroyed
  Game* game_;

  /////////////////////////////////////
  // Private helpers for movement
  /////////////////////////////////////

  // Moves the block down if it is heavy
  void moveDownIfHeavy();

  // Moves the block by the specified row and column values
  bool moveBlock(int, int);

  // Returns the smallest rectangle containing the block
  // Used for rotations
  std::vector<int> boundingRect(std::vector<std::vector<int>>);
};

#endif