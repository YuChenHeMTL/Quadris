#ifndef _LBLOCK_
#define _LBLOCK_

#include "Block.h"
#include "BlockFactory.h"

class BlockFactory;
class BlockFactoryInitializer;

class LBlock : public Block {
  friend class BlockFactoryInitializer;

  // Nested Factory Class
  class Factory : public BlockFactory {
   public:
    std::shared_ptr<Block> create(bool, int);
    friend class BlockFactoryInitializer;
  };

 private:
  // Private constructor
  LBlock(bool isHeavy, int levelGenerated);
};

#endif