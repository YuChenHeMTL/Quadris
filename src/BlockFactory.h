#ifndef _BLOCKFACTORY_
#define _BLOCKFACTORY_

#include <map>
#include <memory>

#include "Block.h"

class Block;

// Class to initiliaze the Block Factory
class BlockFactoryInitializer {
  // Static intializer
  static BlockFactoryInitializer si;
  BlockFactoryInitializer();
};

class BlockFactory {
  // Pure virtual create() function for each Block to implement
  virtual std::shared_ptr<Block> create(bool, int) = 0;

  // Static map of BlockFactories for each block type
  static std::map<std::string, std::shared_ptr<BlockFactory>> factories_;

 public:
  friend class BlockFactoryInitializer;

  // Template Method
  // Factory Method to create blocks (isHeavy, levelGenerated)
  static std::shared_ptr<Block> createBlock(const std::string&, bool, int);
};

#endif