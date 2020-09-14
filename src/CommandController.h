#ifndef _CommandController_
#define _CommandController_

#include "Action.h"
#include "Game.h"

#include <unordered_map>
#include <vector>
#include <memory>

class Game;

class CommandController
{

public:
    CommandController(Game* g);
    void get_input_forever();
    void execute_action(const std::vector<Action> &a, bool is_multiplier_command = false);
    void execute_sequence(std::string filename);
    const std::vector<Action> getActions(std::string command, bool & is_multiplier_action);
    const std::vector<Action> getMultipliedAction(std::string command);
    const std::vector<Action> scanForPrefixes(std::string command);
    void rename_command(std::string original_val, std::string new_val);

    //this is used for testing only
    std::string get_action_string_from_enum(Action a);

private:
    void initMap();
    std::unordered_map<std::string, std::vector<Action>> commandMap_;
    Game* game_;

    //this is used if cannot lookup action in commandMap_
    const std::vector<Action> empty_vector{};
};
#endif