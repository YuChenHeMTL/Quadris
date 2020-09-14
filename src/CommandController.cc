#include "CommandController.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

const bool DEV_TESTING = false;

CommandController::CommandController(Game* game) {
  game_ = game;
  initMap();
}

void CommandController::get_input_forever() {
  std::string input = "";
  execute_action(std::vector<Action>{start});
  while (std::cin >> input) {
    if (input == "sequence") {
      if (std::cin >> input) {  //this will get the file name
        execute_sequence(input);
      } else {
        std::cout << "Could not get file name" << std::endl;
      }
    } else if (input == "rename") {
      std::string original_val = "";
      std::string new_val = "";

      if (!(std::cin >> original_val)) {
        std::cout << "Could not get original command name" << std::endl;
        break;
      }

      if (!(std::cin >> new_val)) {
        std::cout << "Could not get new command name" << std::endl;
        break;
      }

      if (commandMap_.find(original_val) != commandMap_.end()) {
        rename_command(original_val, new_val);
      } else {
        std::cout << "Could not find command " << original_val << " in command list" << std::endl;
      }

    } else {
      bool is_multiplying_command = false;
      const std::vector<Action> commandActions = getActions(input, is_multiplying_command);
      if (!commandActions.empty()) {
        execute_action(commandActions, is_multiplying_command);
      }
    }
  }
}

void CommandController::rename_command(std::string original_val, std::string new_val) {
  auto erase_val = commandMap_.find(original_val);

  assert(commandMap_.find(original_val) != commandMap_.end());

  if (commandMap_.find(new_val) != commandMap_.end()) {
    std::cout << "Cannot rename value \"" << new_val << "\", it is already a command" << std::endl;
    return;
  }

  std::vector<Action> original_val_action = erase_val->second;

  commandMap_.erase(erase_val);

  commandMap_.insert({new_val, original_val_action});

  if (DEV_TESTING) {
    std::cout << "Successfully Renamed Value from " << original_val << " to " << new_val << std::endl;
  }
}

const std::vector<Action> CommandController::getActions(std::string command, bool & is_multiplier_action) {
  assert(command.size() > 0);

  if (commandMap_.find(command) != commandMap_.end()) {
    return commandMap_[command];
  } else if (isdigit(command[0])) {
    is_multiplier_action = true;
    return getMultipliedAction(command);
  } else {
    return scanForPrefixes(command);
  }
}

//this running guarantees the first element is a digit
const std::vector<Action> CommandController::getMultipliedAction(std::string command) {
  assert(isdigit(command[0]));

  // int command_suffix_index = command.length() - 1;
  int multiplier = 0;
  std::string new_command = "a";

  std::stringstream ss(command);

  if (!(ss >> multiplier)) {
    std::cout << "Failed to get multiplier, Doing Nothing" << std::endl;
    return empty_vector;
  }

  if (!(ss >> new_command)) {
    std::cout << "Failed To Get Command After Multiplier, Doing Nothing" << std::endl;
    return empty_vector;
  }

  if (DEV_TESTING) {
    std::cout << "Multiplier: " << multiplier << " New Command: " << new_command << std::endl;
  }

  std::vector<Action> tempcommandActions;
  std::vector<Action> commandActions;

  if (commandMap_.find(command) != commandMap_.end()) {
    tempcommandActions = commandMap_[command];
  } else {
    tempcommandActions = scanForPrefixes(new_command);
  }

  for (int i = 0; i < multiplier; ++i) {
    commandActions.insert(commandActions.end(), tempcommandActions.begin(), tempcommandActions.end());
  }

  return commandActions;
}

//the way we scan for prefixes allow for prefixes to be added for macros, or renaming
const std::vector<Action> CommandController::scanForPrefixes(std::string commandPrefix) {
  if (commandPrefix.length() == 0) {
    return empty_vector;
  }

  auto it = commandMap_.begin();
  std::unordered_set<std::string> potentialMatches;

  while (it != commandMap_.end()) {
    std::string command_in_map = it->first;

    if (commandPrefix.length() <= command_in_map.length()) {
      auto res = std::mismatch(commandPrefix.begin(), commandPrefix.end(), command_in_map.begin());

      if (res.first == commandPrefix.end()) {  //this means that it is a prefix
        potentialMatches.insert(command_in_map);
      }
    }

    ++it;
  }

  if (potentialMatches.size() == 1) {  //return first and only element in the set
    return commandMap_[*potentialMatches.begin()];
  } else {  //this means nothing matched, or more than 1 matched.
    std::cout << "Could not find command: \"" << commandPrefix << "\"! It may be a conflicting prefix of another command or DNE!" << std::endl;
    return empty_vector;
  }
}

void CommandController::execute_action(const std::vector<Action>& a, bool is_multiplier_command) {
  if (DEV_TESTING) {
    if (a.empty()) {
      std::cout << "TEST (CC): Action is empty!" << std::endl;
    }
    for (unsigned int i = 0; i < a.size(); ++i) {
      std::cout << "TEST (CC): Sending Action #: " << i + 1 << " Action: " << get_action_string_from_enum(a[i]) << std::endl;
    }
  }

  if (!a.empty()) {
    game_->performAction(a, is_multiplier_command);
  }
}

void CommandController::execute_sequence(std::string filename) {
  std::string command;
  std::ifstream myfile(filename);

  try {
    if (myfile.is_open()) {
      while (myfile >> command) {
        bool is_multiplying_command = false;
        const std::vector<Action> commandActions = getActions(command, is_multiplying_command);
        if (!commandActions.empty()) {
          execute_action(commandActions, is_multiplying_command);
        }
      }
      //it will close stream automatically
    } else {
      throw std::runtime_error("Unable to open file");
    }
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }

  if (myfile.eof()) {
    throw std::logic_error("game_over");
  }
}

void CommandController::initMap() {
  commandMap_["left"] = std::vector<Action>{left};
  commandMap_["right"] = std::vector<Action>{right};
  commandMap_["down"] = std::vector<Action>{down};
  commandMap_["clockwise"] = std::vector<Action>{clockwise};
  commandMap_["counterclockwise"] = std::vector<Action>{counterclockwise};
  commandMap_["drop"] = std::vector<Action>{drop};
  commandMap_["levelup"] = std::vector<Action>{levelup};
  commandMap_["leveldown"] = std::vector<Action>{leveldown};
  commandMap_["norandomfile"] = std::vector<Action>{norandomfile};
  commandMap_["random"] = std::vector<Action>{restorerandom};
  commandMap_["I"] = std::vector<Action>{I};
  commandMap_["J"] = std::vector<Action>{J};
  commandMap_["L"] = std::vector<Action>{L};
  commandMap_["S"] = std::vector<Action>{S};
  commandMap_["Z"] = std::vector<Action>{Z};
  commandMap_["O"] = std::vector<Action>{O};
  commandMap_["T"] = std::vector<Action>{T};
  commandMap_["restart"] = std::vector<Action>{restart};
  commandMap_["hint"] = std::vector<Action>{hint};
  commandMap_["AItakeover"] = std::vector<Action>{AItakeover};
}

std::string CommandController::get_action_string_from_enum(Action a) {
  switch (a) {
    case left:
      return "left";
    case right:
      return "right";
    case down:
      return "down";
    case clockwise:
      return "clockwise";
    case counterclockwise:
      return "counterclockwise";
    case drop:
      return "drop";
    case levelup:
      return "levelup";
    case leveldown:
      return "leveldown";
    case norandomfile:
      return "norandomfile";
    case restorerandom:
      return "random";
    case I:
      return "I";
    case J:
      return "J";
    case L:
      return "L";
    case S:
      return "S";
    case Z:
      return "Z";
    case O:
      return "O";
    case T:
      return "T";
    case restart:
      return "restart";
    case hint:
      return "hint";
    case endgame:
      return "endgame";
    case start:
      return "start";
    case AItakeover:
      return "AItakover";
    default:
      return "Invalid Action";
  }
}