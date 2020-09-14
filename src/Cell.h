#ifndef _CELL_
#define _CELL_

#include <memory>

#include "Block.h"
#include "Subject.h"

class Subject;
class Block;

class Cell : public Subject {
 public:
  friend class Game;
  // Constructor (row,column)
  Cell(int, int);
  // Destructor
  ~Cell();

  // Returns true is the cell is not empty (has a block stored)
  bool isNotEmpty();

  // Clears the stored block in this cell
  void clear();

  ////////////////////////////////////////////////////////////////////////////////
  // Getters and Setters
  ////////////////////////////////////////////////////////////////////////////////

  // Sets the stored block in this cell
  void setBlock(std::shared_ptr<Block>);

  // Gets the block stored in this cell
  std::shared_ptr<Block> getBlock();

  // Returns the character of the stored block
  char getCharacter();

  // Returns the columnPosition and rowPosition of this cell
  int getColumn();
  int getRow();

 private:
  // Stores the row and column position of this cell
  int rowPosition_;
  int columnPosition_;

  // Static bool that makes any new cell updates invisible
  static bool visible;

  // Stores a pointer to the current block at this cell position
  std::shared_ptr<Block> block_;
};

#endif