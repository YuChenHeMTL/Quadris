#ifndef _LEVEL_
#define _LEVEL_
#include <fstream>
#include <memory>
#include <ostream>
#include <string>

#include "Block.h"
#include "Game.h"

class Block;
class Game;

class Level {
 protected:
  // This is needed in order to get info from the game and tell it to place blocks if needed (ex: 1x1)
  Game* Game_;

  // Stores the current level number
  int levelNum_;

  // For generate blocks from scriptfile
  static int index_;

  // If random block generation is on for level 3 and 4
  static bool isRandom_;

  // If the blocks it generates are heavy
  int isHeavy_ = false;

  // Scripfile to read blocks from
  std::string scriptFile_ = "";

  // norandom file for level 3 and 4
  static std::string norandom_file;

  virtual std::shared_ptr<Block> generateBlockFromScript(const std::string scriptFileName);
  virtual std::shared_ptr<Block> generateBlockFromRandom() = 0;

 public:
  Level();
  Level(const int levelNum, const bool isHeavy, Game* game);
  ~Level();

  virtual void endOfTurnRoutine();
  virtual std::shared_ptr<Block> generateBlock();

  //template methods
  bool getHeavy();
  int getLevelNum();
  void setScriptFile(const std::string);
  void setNoRandomFile(const std::string);
  void switchRandom(const bool);

  char getInputFromFile(std::string);
};

class Level0 : public Level {
 public:
  Level0(Game*);
  std::shared_ptr<Block> generateBlock() override;
  std::shared_ptr<Block> generateBlockFromRandom() override;
};

class Level1 : public Level {
 public:
  Level1(Game*);
  std::shared_ptr<Block> generateBlockFromRandom() override;
};

class Level2 : public Level {
 public:
  Level2(Game*);
  std::shared_ptr<Block> generateBlockFromRandom() override;
};

class Level3 : public Level {
 public:
  Level3(Game*);
  std::shared_ptr<Block> generateBlockFromRandom() override;
};
class Level4 : public Level {
 public:
  Level4(Game*);
  std::shared_ptr<Block> generateBlockFromRandom() override;
  void endOfTurnRoutine() override;
};

std::shared_ptr<Level> getLevel(const int, Game*);

#endif