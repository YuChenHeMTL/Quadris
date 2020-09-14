#ifndef _JBLOCK_
#define _JBLOCK_

#include "Block.h"
#include "BlockFactory.h"

class BlockFactory;
class BlockFactoryInitializer;

class JBlock : public Block {
  friend class BlockFactoryInitializer;

  // Nested Factory Class
  class Factory : public BlockFactory {
   public:
    std::shared_ptr<Block> create(bool, int);
    friend class BlockFactoryInitializer;
  };

 private:
  // Private constructor
  JBlock(bool isHeavy, int levelGenerated);
};

#endif