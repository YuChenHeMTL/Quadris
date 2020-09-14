#include "Level.h"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

#include "BlockFactory.h"

using namespace std;

const bool DEV_TESTING = false;

Level::Level() {
  levelNum_ = 0;
}

Level::Level(const int levelNum, const bool isHeavy, Game* game) {
  levelNum_ = levelNum;
  isHeavy_ = isHeavy;
  Game_ = game;
};

Level::~Level() {}

bool Level::getHeavy() {
  return isHeavy_;
}

int Level::getLevelNum() {
  return levelNum_;
}

void Level::setScriptFile(const string file) {
  scriptFile_ = file;
}

void Level::setNoRandomFile(string file) {
  norandom_file = file;
}

void Level::switchRandom(const bool random) {
  index_ = 0;
  isRandom_ = random;
}

//intentionally left blank
void Level::endOfTurnRoutine() {}

//template method
shared_ptr<Block> Level::generateBlock() {
  if (levelNum_ == 1 || levelNum_ == 2 || isRandom_) {
    return generateBlockFromRandom();
  }

  try {
    string noRandomFileName = norandom_file;
    ifstream scriptFile(noRandomFileName);
    if (scriptFile.good()) {
      return generateBlockFromScript(noRandomFileName);
    } else {
      throw runtime_error("Unable to open script file: " + noRandomFileName);
    }

  } catch (exception& e) {
    std::cerr << e.what() << std::endl;
    throw logic_error("Error in Level");
  }
  return nullptr;
}

char Level::getInputFromFile(string fileName) {
  char currentBlock;
  string nextBlock;
  try {
    ifstream blockFile(fileName);
    if (blockFile.is_open()) {
      int currentIndex = -1;
      while (currentIndex < index_ && !blockFile.eof()) {
        blockFile >> currentBlock;
        currentIndex++;
      }
      if (!blockFile.eof()) blockFile >> nextBlock;

      //if no nextBlock is available, reset index to 0 for next generateBlock
      if (nextBlock.length() == 0) {
        index_ = 0;
      } else {
        index_++;
      }

    } else {
      throw runtime_error("Unable to open block file: " + fileName);
    }

  } catch (exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return currentBlock;
}

shared_ptr<Block> Level::generateBlockFromScript(const std::string fileName) {
  char currentBlock = getInputFromFile(fileName);
  shared_ptr<Block> returnBlock;

  //processing current block in script file
  switch (currentBlock) {
    case ('I'):
      returnBlock = BlockFactory::createBlock("I", isHeavy_, levelNum_);
      break;

    case ('J'):
      returnBlock = BlockFactory::createBlock("J", isHeavy_, levelNum_);
      break;

    case ('L'):
      returnBlock = BlockFactory::createBlock("L", isHeavy_, levelNum_);
      break;

    case ('S'):
      returnBlock = BlockFactory::createBlock("S", isHeavy_, levelNum_);
      break;

    case ('Z'):
      returnBlock = BlockFactory::createBlock("Z", isHeavy_, levelNum_);
      break;

    case ('O'):
      returnBlock = BlockFactory::createBlock("O", isHeavy_, levelNum_);
      break;

    case ('T'):
      returnBlock = BlockFactory::createBlock("T", isHeavy_, levelNum_);
      break;

    default:
      returnBlock = nullptr;
      break;
  }

  return returnBlock;
};

// ----------------------------------------------------
Level0::Level0(Game* game) : Level(0, false, game) {}
shared_ptr<Block> Level0::generateBlock() {
  if (scriptFile_ == "") scriptFile_ = "./sequence.txt";
  string scriptFileName = scriptFile_;
  try {
    ifstream scriptFile(scriptFileName);
    if (scriptFile.good()) {
      return generateBlockFromScript(scriptFileName);
    } else {
      throw runtime_error("Unable to open script file: " + scriptFileName);
    }

  } catch (exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return nullptr;
}

shared_ptr<Block> Level0::generateBlockFromRandom() { return nullptr; }

// ----------------------------------------------------

Level1::Level1(Game* game) : Level(1, false, game) {}
shared_ptr<Block> Level1::generateBlockFromRandom() {
  int totalNum = 12;
  int randomNum = rand() % totalNum;
  shared_ptr<Block> generatedBlock;
  if (randomNum == 0) {
    generatedBlock = BlockFactory::createBlock("S", isHeavy_, levelNum_);
  } else if (randomNum == 1) {
    generatedBlock = BlockFactory::createBlock("Z", isHeavy_, levelNum_);
  } else if (randomNum == 2 || randomNum == 3) {
    generatedBlock = BlockFactory::createBlock("I", isHeavy_, levelNum_);
  } else if (randomNum == 4 || randomNum == 5) {
    generatedBlock = BlockFactory::createBlock("J", isHeavy_, levelNum_);
  } else if (randomNum == 6 || randomNum == 7) {
    generatedBlock = BlockFactory::createBlock("L", isHeavy_, levelNum_);
  } else if (randomNum == 8 || randomNum == 9) {
    generatedBlock = BlockFactory::createBlock("O", isHeavy_, levelNum_);
  } else if (randomNum == 10 || randomNum == 11) {
    generatedBlock = BlockFactory::createBlock("T", isHeavy_, levelNum_);
  } else {
    cerr << "Random Num Gen in Level 0 failed!" << endl;
    throw logic_error("Random Num Gen failed");
  }

  return generatedBlock;
}
// ----------------------------------------------------
Level2::Level2(Game* game) : Level(2, false, game) {}
shared_ptr<Block> Level2::generateBlockFromRandom() {
  int totalNum = 7;
  int randomNum = rand() % totalNum;
  shared_ptr<Block> generatedBlock;
  if (randomNum == 0) {
    generatedBlock = BlockFactory::createBlock("S", isHeavy_, levelNum_);
  } else if (randomNum == 1) {
    generatedBlock = BlockFactory::createBlock("Z", isHeavy_, levelNum_);
  } else if (randomNum == 2) {
    generatedBlock = BlockFactory::createBlock("I", isHeavy_, levelNum_);
  } else if (randomNum == 3) {
    generatedBlock = BlockFactory::createBlock("J", isHeavy_, levelNum_);
  } else if (randomNum == 4) {
    generatedBlock = BlockFactory::createBlock("L", isHeavy_, levelNum_);
  } else if (randomNum == 5) {
    generatedBlock = BlockFactory::createBlock("O", isHeavy_, levelNum_);
  } else if (randomNum == 6) {
    generatedBlock = BlockFactory::createBlock("T", isHeavy_, levelNum_);
  } else {
    cerr << "Random Num Gen in Level 1 failed!" << endl;
    throw logic_error("Random Num Gen failed");
  }

  return generatedBlock;
}
// ----------------------------------------------------
Level3::Level3(Game* game) : Level(3, true, game){};
shared_ptr<Block> Level3::generateBlockFromRandom() {
  int totalNum = 9;
  int randomNum = rand() % totalNum;
  shared_ptr<Block> generatedBlock;
  if (randomNum == 0 || randomNum == 1) {
    generatedBlock = BlockFactory::createBlock("S", isHeavy_, levelNum_);
  } else if (randomNum == 2 || randomNum == 3) {
    generatedBlock = BlockFactory::createBlock("Z", isHeavy_, levelNum_);
  } else if (randomNum == 4) {
    generatedBlock = BlockFactory::createBlock("I", isHeavy_, levelNum_);
  } else if (randomNum == 5) {
    generatedBlock = BlockFactory::createBlock("J", isHeavy_, levelNum_);
  } else if (randomNum == 6) {
    generatedBlock = BlockFactory::createBlock("L", isHeavy_, levelNum_);
  } else if (randomNum == 7) {
    generatedBlock = BlockFactory::createBlock("O", isHeavy_, levelNum_);
  } else if (randomNum == 8) {
    generatedBlock = BlockFactory::createBlock("T", isHeavy_, levelNum_);
  } else {
    cerr << "Random Num Gen in Level 3 failed!" << endl;
    throw logic_error("Random Num Gen failed");
  }

  return generatedBlock;
}

// ----------------------------------------------------
Level4::Level4(Game* game) : Level(4, true, game) {}

shared_ptr<Block> Level4::generateBlockFromRandom() {
  int totalNum = 9;
  int randomNum = rand() % totalNum;
  shared_ptr<Block> generatedBlock;
  if (randomNum == 0 || randomNum == 1) {
    generatedBlock = BlockFactory::createBlock("S", isHeavy_, levelNum_);
  } else if (randomNum == 2 || randomNum == 3) {
    generatedBlock = BlockFactory::createBlock("Z", isHeavy_, levelNum_);
  } else if (randomNum == 4) {
    generatedBlock = BlockFactory::createBlock("I", isHeavy_, levelNum_);
  } else if (randomNum == 5) {
    generatedBlock = BlockFactory::createBlock("J", isHeavy_, levelNum_);
  } else if (randomNum == 6) {
    generatedBlock = BlockFactory::createBlock("L", isHeavy_, levelNum_);
  } else if (randomNum == 7) {
    generatedBlock = BlockFactory::createBlock("O", isHeavy_, levelNum_);
  } else if (randomNum == 8) {
    generatedBlock = BlockFactory::createBlock("T", isHeavy_, levelNum_);
  } else {
    cerr << "Random Num Gen in Level 3 failed!" << endl;
    throw logic_error("Random Num Gen failed");
  }

  return generatedBlock;
}

void Level4::endOfTurnRoutine() {
  if (Game_) {  // If Game_ pointer is not null
    int last_turn_with_clear = Game_->get_last_turn_cleared();
    int total_turns = Game_->get_total_turns();
    if ((total_turns - last_turn_with_clear > 0) && ((total_turns - last_turn_with_clear) % 5 == 0)) {
      if (DEV_TESTING) {
        std::cout << "TEST (Level): Spawning 1x1 Block" << std::endl;
      }

      Game_->drop1by1Block(BlockFactory::createBlock("*", isHeavy_, levelNum_));

      return;

    } else {  //Do nothing
      if (DEV_TESTING) {
        std::cout << "TEST (Level): Not Spawning 1x1 Block: " << (total_turns - last_turn_with_clear) << "/5 in a row without clear" << std::endl;
      }
    }
  } else {
    std::cerr << "Game Pointer is nullptr" << std::endl;
  }
}

shared_ptr<Level> getLevel(const int levelNum, Game* game) {
  int verifiedLevelNum = levelNum;
  if (levelNum < 0) verifiedLevelNum = 0;
  if (levelNum > 4) verifiedLevelNum = 4;

  shared_ptr<Level> returnLevel;

  switch (verifiedLevelNum) {
    case 1:
      returnLevel = std::make_shared<Level1>(game);
      break;
    case 2:
      returnLevel = std::make_shared<Level2>(game);
      break;
    case 3:
      returnLevel = std::make_shared<Level3>(game);
      break;
    case 4:
      returnLevel = std::make_shared<Level4>(game);
      break;
    case 0:
    default:
      returnLevel = std::make_shared<Level0>(game);
      break;
  }

  return returnLevel;
}

// Static initialiaztion
int Level::index_ = 0;
bool Level::isRandom_ = true;
std::string Level::norandom_file = "";