#ifndef _GAME_
#define _GAME_

#include <string>
#include <vector>

#include "Action.h"
#include "Block.h"
#include "Board.h"
#include "CommandController.h"
#include "Level.h"
#include "Subject.h"
#include "TextDisplay.h"

class CommandController;
class Board;
class Level;
class Block;

class Game : public Subject {
 public:
  // Constructor
  Game(bool textonly, int seed, std::string scriptfile_name, int start_level, bool enable_bonuses);
  // Destructor
  ~Game();

  // Performs all Actions in the supplied vector of Actions
  // Action is an enum that specifies all valid commands
  void performAction(const std::vector<Action> &actions, bool is_multiplier_command = false);

  // Drops a 1x1 block in the middle of the board
  // Used for level 4 behaviour
  void drop1by1Block(const std::shared_ptr<Block> &block);

  // Subscribes an observer to both the game and all the cells in the board owned by the game
  void addObserver(Observer *);

  // Adds the supplied int to the current score
  void addToScore(int);

  ////////////////////////////////////
  // Getters and Setters
  ////////////////////////////////////
  int get_last_turn_cleared();
  int get_total_turns();
  int get_current_score();
  int get_high_score();
  int get_current_level();

  Block *get_current_block();
  std::shared_ptr<Block> getNextBlock();

 private:
  ////////////////////////////////////
  // Private Command helpers
  ////////////////////////////////////
  void moveLeft();
  void moveRight();
  void moveDown();
  void dropBlock();

  // Helper for debug commands I,J,T,S,Z...
  void createBlock(const std::string);

  // Helper to start new turn after dropping a block
  void startNewTurn();

  // Helper to increase or decrease the current level
  void levelChangeUp(const bool);

  // Helper to restart the game
  void restartGame();

  // Sets the current level to the specified level
  void setLevel(int);

  // Helper for clear line
  void clearLine();

  // Helper method to encapsulate block spawning
  void spawnBlock(std::shared_ptr<Block>);

  // Clears the cells in the supplied block
  void clearCellsinBlock(std::shared_ptr<Block> &block);

  ////////////////////////////////////
  // Private Hint Command Helpers
  ////////////////////////////////////

  void show_hint();
  std::pair<int, int> get_best_move_despawn_current_block();

  // Calculates the hint score based on 4 heuristics
  // used for evaluating where the hint block should go
  double calculateHintScore();
  int getAggregateHeight();
  int getHoleScore();
  int getBumpinessScore();
  int getCompleteRowScore();

  // AI player that places each block where the hint block appears
  void automated_AI_moves();

  ////////////////////////////////////
  // Private Members
  ////////////////////////////////////

  // Smart pointers to the currentBlock, nextBlock
  // board, currentLevel, hintBlock
  std::shared_ptr<Block> currentBlock_;
  std::shared_ptr<Block> nextBlock_;
  std::shared_ptr<Block> hintBlock_;
  std::shared_ptr<Board> board_;
  std::shared_ptr<Level> currentLevel_;

  // Indicates the current turn number
  // (each time a block is dropped, the turn increases)
  // Used for level 4 1x1 block
  int total_turns_;

  // The last turn number in which a line was cleared
  // Used for level 4 1x1 block
  int last_turn_with_clear_;

  // Current score
  int current_score_;

  // High score
  int high_score_;

  // Saves command line arguments
  // In order to properly restart the game
  int start_level_;
  int seed_;
  std::string scriptfile_name_;
  bool enable_bonuses_;
};

#endif