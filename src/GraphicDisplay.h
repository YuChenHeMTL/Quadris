#ifndef _GRAPHICDISPLAY_
#define _GRAPHICDISPLAY_

#include <iostream>
#include <string>

#include "Cell.h"
#include "Game.h"
#include "GraphicsWindow.h"
#include "Observer.h"
#include "Subject.h"

class Observer;
class Subject;
class GraphicDisplay;
class Cell;
class Game;

class GraphicDisplay : public Observer {
 public:
  GraphicDisplay();
  void update(Subject* subjectNotifying) override;
  void initCells();

  void updateCellColour(int row, int column, std::string colour = "White");

 private:
  //private helpers for update
  void updateFromCell(Cell* cell);
  void updateFromGame(Game* game);
  void updateNextBlock(char next_block_char);
  void updateCurrentScore(int current_score);
  void updateHighScore(int high_score);
  void updateCurrentLevel(int current_level);
  std::string getCellColour(char cell_char);

  //need to get what block type
  char next_block_char_;
  int level_;
  int current_score_;
  int high_score_;
  GraphicsWindow window_;

  static const int num_rows_ = 18;
  static const int num_columns_ = 11;
};
#endif