#ifndef _OBLOCK_
#define _OBLOCK_

#include "Block.h"
#include "BlockFactory.h"

class BlockFactory;
class BlockFactoryInitializer;

class OBlock : public Block {
  void rotate(bool) override;

  friend class BlockFactoryInitializer;

  // Nested Factory Class
  class Factory : public BlockFactory {
   public:
    std::shared_ptr<Block> create(bool, int);
    friend class BlockFactoryInitializer;
  };

 private:
  // Private constructor
  OBlock(bool isHeavy, int levelGenerated);
};

#endif