#include "Game.h"

#include <cmath>
#include <iostream>
#include <memory>

#include "BlockFactory.h"
const bool DEV_TESTING = false;

// Constructor
Game::Game(bool textonly, int seed, std::string scriptfile_name, int start_level, bool enable_bonuses)
    : total_turns_(0), last_turn_with_clear_(0), current_score_(0), high_score_(0) {
  // Testing logs
  if (DEV_TESTING) {
    std::cout << "Game arguments are: " << std::endl;
    std::cout << "Textonly: " << textonly << std::endl;
    std::cout << "Seed: " << seed << std::endl;
    std::cout << "Scriptfile Name: " << scriptfile_name << std::endl;
    std::cout << "Start Level: " << start_level << std::endl;
    std::cout << "Enable Bonuses: " << enable_bonuses << std::endl;
    std::cout << std::endl;
  }

  // Saves supplied parameters for restart
  seed_ = seed;
  scriptfile_name_ = scriptfile_name;
  start_level_ = start_level;
  enable_bonuses_ = enable_bonuses;

  // Sets srand to the supplied seed
  srand(seed_);

  // Instantiates the gameBoard
  board_ = std::make_shared<Board>(this);

  // Instantiates currentLevel based on the supplied start_level
  setLevel(start_level);
}

// Destructor
Game::~Game() {}

// Subscribes an observer to both the game and all the cells in the board owned by the game
void Game::addObserver(Observer *ob) {
  // Subscribes the observer to this Game instance
  subscribe(ob);
  // Subscribes the observer to each cell in the board
  for (int i = 0; i < board_->getBoardRowSize(); i++) {
    for (int j = 0; j < board_->getBoardColumnSize(); j++) {
      board_->getCell(i, j)->subscribe(ob);
    }
  }
}

// Performs all Actions in the supplied vector of Actions
// Action is an enum that specifies all valid commands
void Game::performAction(const std::vector<Action> &actions, bool is_multiplier_command) {
  bool multipler_control_heavy = false;
  // If it is a multiplier command with a set of actions that is greater than 1
  // Then if the block is heavy it should only drop once
  if (is_multiplier_command && actions.size() >= 1 && currentBlock_->isHeavy()) {
    multipler_control_heavy = true;
    currentBlock_->setHeavy(false);
  }

  for (unsigned int i = 0; i < actions.size(); ++i) {
    // This is last command, set it back to heavy if it is a multiplied command so it will only drop once
    if (multipler_control_heavy && i == actions.size() - 1) {
      currentBlock_->setHeavy(true);
    }

    // Clears the hintblock if it exists
    if (hintBlock_) {
      if (DEV_TESTING) {
        std::cout << "TEST (Game): Hint Block Detected. Removing..." << std::endl;
      }

      clearCellsinBlock(hintBlock_);
      hintBlock_ = nullptr;
    }

    std::string fileName;

    // Switch case parses actions and calls the respective helper function
    switch (actions[i]) {
      case start:
        startNewTurn();
        break;

      case clockwise:
        currentBlock_->rotate(true);
        break;

      case counterclockwise:
        currentBlock_->rotate(false);
        break;

      case left:
        moveLeft();
        break;

      case right:
        moveRight();
        break;

      case down:
        moveDown();
        break;

      case drop:
        dropBlock();
        clearLine();
        startNewTurn();
        break;

      case hint:
        show_hint();
        break;

      case I:
        createBlock("I");
        break;

      case J:
        createBlock("J");
        break;

      case S:
        createBlock("S");
        break;

      case Z:
        createBlock("Z");
        break;

      case O:
        createBlock("O");
        break;

      case L:
        createBlock("L");
        break;

      case T:
        createBlock("T");
        break;

      case levelup:
        levelChangeUp(true);
        break;

      case leveldown:
        levelChangeUp(false);
        break;

      case norandomfile:
        std::cin >> fileName;
        if (fileName.length() != 0) currentLevel_->setNoRandomFile(fileName);
        currentLevel_->switchRandom(false);
        break;

      case restorerandom:
        currentLevel_->switchRandom(true);
        break;

      case restart:
        restartGame();
        break;

      case AItakeover:
        automated_AI_moves();
        break;

      default:
        std::cout << "Invalid Action in Game" << std::endl;
        break;
    }
  }

  // Notifies subscribed observers
  notify();
}

// Drops a 1x1 block in the middle of the board
// Used for level 4 behaviour
void Game::drop1by1Block(const std::shared_ptr<Block> &block) {
  if (DEV_TESTING) {
    std::cout << "TEST (Game): Dropping 1 by 1 Block in Middle" << std::endl;
  }
  spawnBlock(block);
  block->drop();
}

////////////////////////////////////
// Private Hint Command Helpers
////////////////////////////////////

std::pair<int, int> Game::get_best_move_despawn_current_block() {
  if (DEV_TESTING) {
    std::cout << "Test (Game): Temporarily Clearing Current Block" << std::endl;
  }

  clearCellsinBlock(currentBlock_);

  double best_score = -100000000;

  //this will create a block at the top that moves from left to right, rotating at the top of the board. Before dropping, a copy will be made
  std::shared_ptr<Block> top_dropper = BlockFactory::createBlock(std::string(1, currentBlock_->getCharacter()), false, 0);

  if (DEV_TESTING) {
    std::cout << "TEST (Game): Succesfully made top_dropper" << std::endl;
  }

  top_dropper->setBoard(board_);

  std::pair<int, int> best_block_movement = {0, 0};

  for (int i = 0; i < board_->getBoardColumnSize(); ++i) {
    for (int j = 0; j < 4; ++j) {
      if (DEV_TESTING) {
        std::cout << "TEST (Game): On Column: " << i << " On Rotation: " << j << std::endl;
      }

      std::shared_ptr<Block> block_to_drop = BlockFactory::createBlock(std::string(1, currentBlock_->getCharacter()), false, 0);

      block_to_drop->setBoard(board_);
      block_to_drop->setOccupiedCellPositions(top_dropper->getOccupiedCellPositions());

      for (int k = 0; k < i; k++) {
        block_to_drop->moveRight();
      }

      block_to_drop->dropBlockNoUpdate();
      block_to_drop->makeHintBlock();
      board_->spawnBlock(block_to_drop);

      double score = calculateHintScore();

      if (score > best_score) {
        best_score = score;
        best_block_movement.first = i;
        best_block_movement.second = j;
      }

      clearCellsinBlock(block_to_drop);
      block_to_drop = nullptr;

      top_dropper->rotate(true);
    }
  }

  //topdropper was never set to any cells, so dont need to clear
  top_dropper = nullptr;

  if (DEV_TESTING) {
    std::cout << "TEST (Game): " << best_score << ", " << best_block_movement.first << ", " << best_block_movement.second << std::endl;
  }

  return best_block_movement;
}

void Game::show_hint() {
  if (DEV_TESTING) {
    std::cout << "TEST (Game): Making hint block of type: " << currentBlock_->getCharacter() << std::endl;
  }

  hintBlock_ = BlockFactory::createBlock(std::string(1, currentBlock_->getCharacter()), false, 0);

  //this changes the character to "?"
  hintBlock_->makeHintBlock();

  if (DEV_TESTING) {
    std::cout << "TEST (Game): Made Hint Block" << std::endl;
  }

  hintBlock_->setBoard(board_);
  hintBlock_->setGame(this);

  Cell::visible = false;
  std::pair<int, int> hint_block_movement = get_best_move_despawn_current_block();
  Cell::visible = true;

  for (int i = 0; i < hint_block_movement.second; ++i) {
    hintBlock_->rotate(true);
  }

  for (int i = 0; i < hint_block_movement.first; ++i) {
    hintBlock_->moveRight();
  }

  hintBlock_->dropBlockNoUpdate();

  if (DEV_TESTING) {
    std::cout << "Test (Game): Spawning Current Block Again" << std::endl;
  }

  spawnBlock(currentBlock_);

  if (DEV_TESTING) {
    std::cout << "TEST (Game): position of hint block:";
    for (unsigned int i = 0; i < hintBlock_->getOccupiedCellPositions().size(); i++) {
      for (unsigned int j = 0; j < hintBlock_->getOccupiedCellPositions()[i].size(); j++) {
        std::cout << hintBlock_->getOccupiedCellPositions()[i][j] << ", ";
      }
    }
    std::cout << std::endl;
  }

  board_->spawnHintBlock(hintBlock_);
}

void Game::automated_AI_moves() {
  unsigned long long num_blocks_placed = 0;

  while (true) {
    Cell::visible = false;
    std::pair<int, int> best_block_movement = get_best_move_despawn_current_block();
    Cell::visible = true;

    spawnBlock(currentBlock_);

    for (int i = 0; i < best_block_movement.second; ++i) {
      performAction(std::vector<Action>{clockwise});
    }

    for (int i = 0; i < best_block_movement.first; ++i) {
      performAction(std::vector<Action>{right});
    }

    performAction(std::vector<Action>{drop});

    num_blocks_placed++;

    std::cout << "Placed Block # " << num_blocks_placed << std::endl;
  }
}

int Game::getAggregateHeight() {
  if (DEV_TESTING) {
    std::cout << "TEST (Game): Calculating Height Score" << std::endl;
  }

  int rowSize = board_->getBoardRowSize();        // == 18 = number of rows
  int columnSize = board_->getBoardColumnSize();  // == 11 = number of columns

  std::vector<int> columnScore(columnSize, 0);

  //initialization of height of columns
  for (int i = 0; i < columnSize; i++) {
    for (int j = 0; j < rowSize; j++) {
      if (board_->getCell(j, i)->getCharacter() != ' ') {
        columnScore[i] = rowSize - j;
        break;
      }
    }
  }

  if (DEV_TESTING) {
    std::cout << "TEST (Game): Aggregate Height Score: ";
    for (auto ite : columnScore) {
      std::cout << ite << " ";
    }
    std::cout << std::endl;
  }

  //total aggregate height
  int aggregateHeight = 0;
  for (auto ite : columnScore) {
    aggregateHeight += ite;
  }

  return aggregateHeight;
}

int Game::getHoleScore() {
  if (DEV_TESTING) {
    std::cout << "TEST (Game): Calculating Hole Score" << std::endl;
  }

  int rowSize = board_->getBoardRowSize();        // == 18 = number of rows
  int columnSize = board_->getBoardColumnSize();  // == 11 = number of columns

  std::vector<int> holeScore(board_->getBoardColumnSize(), 0);

  // initialization of # of holes
  for (int i = 0; i < columnSize; i++) {
    bool firstCell = false;
    for (int j = 0; j < rowSize; j++) {
      if (board_->getCell(j, i)->getCharacter() == ' ') {
        if (firstCell) {
          holeScore[i]++;
        }
      } else {
        firstCell = true;
      }
    }
  }

  if (DEV_TESTING) {
    std::cout << "TEST (Game): Hole Score: ";
    for (auto ite : holeScore) {
      std::cout << ite << " ";
    }
    std::cout << std::endl;
  }

  //total hole number
  int holeNumber = 0;
  for (auto ite : holeScore) {
    holeNumber += ite;
  }

  return holeNumber;
}

int Game::getBumpinessScore() {
  if (DEV_TESTING) {
    std::cout << "TEST (Game): Calculating Bumpiness Score" << std::endl;
  }

  int rowSize = board_->getBoardRowSize();        // == 18 = number of rows
  int columnSize = board_->getBoardColumnSize();  // == 11 = number of columns

  std::vector<int> columnScore(columnSize, 0);

  //initialization of height of columns
  for (int i = 0; i < columnSize; i++) {
    for (int j = 0; j < rowSize; j++) {
      if (board_->getCell(j, i)->getCharacter() != ' ') {
        columnScore[i] = rowSize - j;
        break;
      }
    }
  }

  //bumpiness
  int bumpiness = 0;
  for (unsigned int i = 0; i < columnScore.size() - 1; i++) {
    int absColumnDiff = (columnScore[i] - columnScore[i + 1] > 0) ? columnScore[i] - columnScore[i + 1] : columnScore[i + 1] - columnScore[i];
    bumpiness += absColumnDiff;
  }

  return bumpiness;
}
int Game::getCompleteRowScore() {
  if (DEV_TESTING) {
    std::cout << "TEST (Game): Calculating Row Score" << std::endl;
  }

  int rowSize = board_->getBoardRowSize();        // == 18 = number of rows
  int columnSize = board_->getBoardColumnSize();  // == 11 = number of columns

  int num_complete_rows = 0;

  for (int i = 0; i < rowSize; ++i) {
    int num_blocks = 0;
    for (int j = 0; j < columnSize; ++j) {
      if (board_->getCell(i, j)->isNotEmpty()) {
        num_blocks++;
      }
    }

    if (num_blocks == columnSize) {
      num_complete_rows++;
    }
  }

  if (DEV_TESTING) {
    std::cout << "TEST (Game): Row Score: " << num_complete_rows << std::endl;
  }

  return num_complete_rows;
}

//https://codemyroad.wordpress.com/2013/04/14/tetris-ai-the-near-perfect-player/
// The hint score calculation is taken from this source linked above. The calculation and parameter values must be credited to the source above.
double Game::calculateHintScore() {
  int aggregateHeight = getAggregateHeight();
  int holeNumber = getHoleScore();
  int bumpiness = getBumpinessScore();
  int RowScore = getCompleteRowScore();

  double aggregateHeightParam = -0.510066;
  double rowScoreParam = 0.760666;
  double holeParam = -0.35663;
  double bumpinessParam = -0.184483;

  double totalScore = (double)aggregateHeight * aggregateHeightParam + (double)holeNumber * holeParam + (double)bumpiness * bumpinessParam + rowScoreParam * RowScore;

  if (DEV_TESTING) {
    std::cout << "TEST (Game): Total score for current board: " << totalScore << std::endl;
  }

  return totalScore;
}

////////////////////////////////////
// Private Command helpers
////////////////////////////////////

// Helper to restart the game
void Game::restartGame() {
  // Clears the board
  board_->clearBoard();

  // Reset the score and the current level
  current_score_ = 0;
  setLevel(start_level_);
  currentLevel_->switchRandom(true);

  // Resets current turn
  last_turn_with_clear_ = 0;
  total_turns_ = 0;

  // Sets the current and next blocks to nullptr in order to generate new blocks
  currentBlock_ = nullptr;
  nextBlock_ = nullptr;

  startNewTurn();
}

// Helper to start new turn after dropping a block
void Game::startNewTurn() {
  // Calls the current levels endOfTurnRoutine (e.g. Level 4 1x1 Block)
  currentLevel_->endOfTurnRoutine();

  // If this is the first turn, then we must generate an extra block (one for the currentBlock and one for the nextBlock)
  if (!nextBlock_) {
    nextBlock_ = currentLevel_->generateBlock();
    // Notify Observers
    notify();
  }

  // Set the nextBlock as the currentBlock
  currentBlock_ = nextBlock_;

  // Generates the nextBlock using level
  nextBlock_ = currentLevel_->generateBlock();

  // Spawn the currentBlock
  spawnBlock(currentBlock_);

  // Increment total_turn counter
  total_turns_++;
}

// Adds the supplied int to the current score
void Game::addToScore(int points) {
  // Adds points to the current_score
  current_score_ += points;

  // If the current_score exceeds the high score, update it
  if (current_score_ > high_score_) {
    high_score_ = current_score_;
  }
}

// Helper method to encapsulate block spawning
void Game::spawnBlock(std::shared_ptr<Block> block) {
  // Sets the board in the block
  block->setBoard(board_);

  // Sets the weak game pointer in the block
  block->setGame(this);

  // Spawns the block on the board
  board_->spawnBlock(block);
}

// Increases or decreases the current level
void Game::levelChangeUp(const bool changeUp) {
  // Gets the current level and increments of decrements based on the supplied bool
  int currentLevelNum = currentLevel_->getLevelNum();
  int nextLevelNum = (changeUp == true) ? currentLevelNum + 1 : currentLevelNum - 1;

  // Testing logs
  if (DEV_TESTING) {
    std::cout << "TEST (Game): Trying to Change Level from " << currentLevelNum << " to " << nextLevelNum << std::endl;
  }

  // If the current level is the same then do nothing
  if (nextLevelNum == currentLevelNum) return;

  // Sets the level to the specified level
  setLevel(nextLevelNum);
}

// Sets the current level to the specified level
void Game::setLevel(int level) {
  currentLevel_ = getLevel(level, this);
  if (scriptfile_name_.length() != 0) currentLevel_->setScriptFile(scriptfile_name_);
}

// Method to clear any full lines in the board
void Game::clearLine() {
  // Determines the number of lines which were cleared by calling the clearLine helper function
  int clearedLines = board_->clearLine();

  // Calculates the points to be added to the score as a result
  if (clearedLines > 0) {
    last_turn_with_clear_ = total_turns_;
    int level = currentLevel_->getLevelNum();
    int scoredPoints = (clearedLines + level) * (clearedLines + level);
    addToScore(scoredPoints);
  }
}

// Clears the cells in the supplied block
void Game::clearCellsinBlock(std::shared_ptr<Block> &block) {
  // This means block pointer is null
  if (!block) {
    return;
  }

  std::vector<std::vector<int>> cells_occupied_by_block = block->getOccupiedCellPositions();

  // For all the cells occupied by the block, clear them.
  for (unsigned int i = 0; i < cells_occupied_by_block.size(); ++i) {
    board_->getCell(cells_occupied_by_block[i][0], cells_occupied_by_block[i][1])->clear();
  }
}

// Block Movement Helper function
void Game::moveLeft() {
  currentBlock_->moveLeft();
}

void Game::moveRight() {
  currentBlock_->moveRight();
}
void Game::moveDown() {
  currentBlock_->moveDown();
}
void Game::dropBlock() {
  currentBlock_->drop();
}

// Function to replace the current block with the supplied block type
// Used for I,J,S,T... commands
void Game::createBlock(const std::string c) {
  // Testing logs
  if (DEV_TESTING) {
    std::cout << "TEST (Game): Creating a " << c << " Block by User Command" << std::endl;
  }

  // Creates the newBlock
  std::shared_ptr<Block> CharBlock = BlockFactory::createBlock(c, currentLevel_->getHeavy(), currentLevel_->getLevelNum());

  // Removes current block
  clearCellsinBlock(currentBlock_);
  currentBlock_ = nullptr;

  // Sets the current block to the new block and spawns it
  currentBlock_ = CharBlock;
  spawnBlock(currentBlock_);
}

/////////////////////////////////////////////////
// Getter Methods
/////////////////////////////////////////////////

Block *Game::get_current_block() {
  return currentBlock_.get();
}

int Game::get_last_turn_cleared() {
  return last_turn_with_clear_;
}
int Game::get_total_turns() {
  return total_turns_;
}

int Game::get_current_score() {
  return current_score_;
}
int Game::get_high_score() {
  return high_score_;
}

std::shared_ptr<Block> Game::getNextBlock() {
  return nextBlock_;
}

int Game::get_current_level() {
  return currentLevel_->getLevelNum();
}

/////////////////////////////////////////////////
