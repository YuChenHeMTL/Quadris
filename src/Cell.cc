#include "Cell.h"

#include "Block.h"

using namespace std;

// Constructor intializes Cell position
Cell::Cell(int row, int column) {
  rowPosition_ = row;
  columnPosition_ = column;
}

// No raw pointers for ownership so doesn't need to do anything
Cell::~Cell() {
}

// Returns true is the cell is not empty (has a block stored)
bool Cell::isNotEmpty() {
  if (block_) {
    return true;
  } else {
    return false;
  }
};

// Clears the stored block in this cell
void Cell::clear() {
  // Sets the block to nullptr
  block_ = nullptr;

  // Notifies all observers to update
  if (visible) {
    notify();
  }
}

////////////////////////////////////////////////////////////////////////////////
// Getters and Setters
////////////////////////////////////////////////////////////////////////////////

// Gets the block stored in this cell
shared_ptr<Block> Cell::getBlock() {
  return block_;
}

// Sets the stored block in this cell
void Cell::setBlock(std::shared_ptr<Block> block) {
  // Sets the block
  block_ = block;

  // Notifies all observers to update
  if (visible) {
    notify();
  }
}

// Returns the columnPosition of this cell
int Cell::getColumn() {
  return columnPosition_;
}

// Returns the rowPosition of this cell
int Cell::getRow() {
  return rowPosition_;
}

// Returns the character of the stored block
char Cell::getCharacter() {
  // If there is a stored block, return that character of the block
  if (block_) {
    return block_->getCharacter();
  }
  // Otherwise return ' ' which indicates an empty cell
  return ' ';
}

// Static variable Initializer
// Indicates if cell updates should notify observers
bool Cell::visible = true;