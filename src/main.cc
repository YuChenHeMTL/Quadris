#include <string.h>

#include <fstream>
#include <iostream>

#include "CommandController.h"
#include "Game.h"
#include "GraphicDisplay.h"
#include "TextDisplay.h"

class CommandController;
class Game;

int main(int argc, char *argv[]) {
  // Instantiates all game parameters to default values
  bool textonly = false;
  int seed = 0;
  std::string scriptfile_name = "sequence.txt";
  int start_level = 0;
  bool enable_bonuses = false;
  bool valid_flags = true;

  int i = 1;

  // Parses all the supplied command line arguments
  while (i < argc) {
    std::string flag(argv[i]);

    if (flag == "-text") {
      textonly = true;
      ++i;
    } else if (flag == "-seed") {
      try {
        if (i + 1 >= argc) {
          throw std::runtime_error("No seed argument!");
        }

        seed = std::stoi(argv[i + 1]);
        i += 2;
      } catch (std::exception &e) {
        valid_flags = false;
        std::cerr << "Failed to get seed value: " << e.what() << std::endl;
        break;
      }
    } else if (flag == "-scriptfile") {
      try {
        if (i + 1 >= argc) {
          throw std::runtime_error("No scriptfile argument!");
        }

        scriptfile_name = argv[i + 1];
        std::ifstream testopen(scriptfile_name);

        if (!testopen.is_open()) {
          throw std::runtime_error("Failed to open script file!");
        }

        i += 2;
      } catch (std::exception &e) {
        valid_flags = false;
        std::cerr << "Failed to get scriptfile: " << e.what() << std::endl;
        break;
      }
    } else if (flag == "-startlevel") {
      try {
        if (i + 1 >= argc) {
          throw std::runtime_error("No level argument!");
        }

        start_level = std::stoi(argv[i + 1]);
        i += 2;
      } catch (std::exception &e) {
        valid_flags = false;
        std::cerr << "Failed to get level: " << e.what() << std::endl;
        break;
      }
    } else if (flag == "-enablebonus") {
      enable_bonuses = true;
      i++;
    } else {
      std::cerr << "Invalid Argument " << argv[i] << std::endl;
      valid_flags = false;
      break;
    }
  }

  if (valid_flags) {
    // Creates the game instance
    std::shared_ptr<Game> game = std::make_shared<Game>(textonly, seed, scriptfile_name, start_level, enable_bonuses);

    // Instantiates and subscribes TextDisplay to the game
    TextDisplay td;
    game->addObserver(&td);

    // Instantiates GraphicDisplay and subscribes TextDisplay to the game
    // (if textonly mode is not on)
    std::shared_ptr<GraphicDisplay> gd;
    if (!textonly) {
      gd = std::make_shared<GraphicDisplay>();
      game->addObserver(gd.get());
    }

    // Instantiates the CommandController
    CommandController c(game.get());

    // Enters the main game loop in CommandController
    try {
      c.get_input_forever();
    } catch (const std::exception &e) {
      // If the exception "game_over" is thrown
      // Then terminate the program by returning 0;
      if (strcmp(e.what(), "game_over") == 0) {
        return 0;
      }
    }
  }
  return 0;
}