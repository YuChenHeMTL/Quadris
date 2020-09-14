#include "BlockFactory.h"

#include "Block1X1.h"
#include "IBlock.h"
#include "JBlock.h"
#include "LBlock.h"
#include "OBlock.h"
#include "SBlock.h"
#include "TBlock.h"
#include "ZBlock.h"

// Static Member
std::map<std::string, std::shared_ptr<BlockFactory>> BlockFactory::factories_;

std::shared_ptr<Block> BlockFactory::createBlock(const std::string& id, bool isHeavy, int levelGenerated) {
  // If the block type is found in the map, then create an instance and return it
  if (factories_.find(id) != factories_.end()) {
    // create() is the hole
    return factories_[id]->create(isHeavy, levelGenerated);
  } else {
    std::cout << "Invalid Block" << std::endl;
    throw "Invalid Block!";
  }
}

// Intializes Block Factory
BlockFactoryInitializer::BlockFactoryInitializer() {
  BlockFactory::factories_["O"] = std::make_shared<OBlock::Factory>();
  BlockFactory::factories_["J"] = std::make_shared<JBlock::Factory>();
  BlockFactory::factories_["L"] = std::make_shared<LBlock::Factory>();
  BlockFactory::factories_["S"] = std::make_shared<SBlock::Factory>();
  BlockFactory::factories_["T"] = std::make_shared<TBlock::Factory>();
  BlockFactory::factories_["Z"] = std::make_shared<ZBlock::Factory>();
  BlockFactory::factories_["I"] = std::make_shared<IBlock::Factory>();
  BlockFactory::factories_["*"] = std::make_shared<Block1X1::Factory>();
}

// Static Member
BlockFactoryInitializer BlockFactoryInitializer::si;