#ifndef _TBLOCK_
#define _TBLOCK_

#include "Block.h"
#include "BlockFactory.h"

class BlockFactory;
class BlockFactoryInitializer;

class TBlock : public Block {
  friend class BlockFactoryInitializer;

  // Nested Factory Class
  class Factory : public BlockFactory {
   public:
    std::shared_ptr<Block> create(bool, int);
    friend class BlockFactoryInitializer;
  };

 private:
  // Private constructor
  TBlock(bool isHeavy, int levelGenerated);
};

#endif