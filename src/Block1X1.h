#ifndef _1X1BLOCK_
#define _1X1BLOCK_

#include "Block.h"
#include "BlockFactory.h"

class BlockFactory;
class BlockFactoryInitializer;

class Block1X1 : public Block {
  friend class BlockFactoryInitializer;

  // Nested Factory Class
  class Factory : public BlockFactory {
   public:
    std::shared_ptr<Block> create(bool, int);
    friend class BlockFactoryInitializer;
  };

 private:
  // Private constructor
  Block1X1(bool isHeavy, int levelGenerated);
};

#endif