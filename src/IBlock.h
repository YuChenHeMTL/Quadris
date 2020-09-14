#ifndef _IBLOCK_
#define _IBLOCK_

#include "Block.h"
#include "BlockFactory.h"

class BlockFactory;
class BlockFactoryInitializer;

class IBlock : public Block {
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
  IBlock(bool isHeavy, int levelGenerated);
};

#endif