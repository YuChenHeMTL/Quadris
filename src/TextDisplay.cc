#include "TextDisplay.h"
#include <cassert>

const bool DEV_TESTING = false;

TextDisplay::TextDisplay() : next_block_char_(' '), level_(0), current_score_(0), high_score_(0){
    initGrid();
}

void TextDisplay::initGrid()
{
    for (unsigned int i = 0; i < num_rows_; ++i)
    {
        for (unsigned int j = 0; j < num_columns_; ++j)
        {
            grid_[i][j] = ' ';
        }
    }
}

void TextDisplay::update(Subject *subjectNotifying)
{
    //try to dynamic cast to cell. If that doesn't work, that means it must be a game

    Cell *cell_notifying = dynamic_cast<Cell *>(subjectNotifying);

    if (cell_notifying){ //this means the dynamic cast was succesful
        if (DEV_TESTING){
            std::cout << "TEST (GD): Casted to cell. Updating." << std::endl;
        }
        updateFromCell(cell_notifying);

    }
    else{ //this means cast failed, and must be game

        if (DEV_TESTING){
            std::cout << "TEST (GD): Failed to cast to cell. Will try to cast to game." << std::endl;
        }

        Game *game_notifying = dynamic_cast<Game *>(subjectNotifying);

        if (game_notifying){
            updateFromGame(game_notifying);
        }
        else{
            std::cerr << "Failed to cast to game!" << std::endl;
        }
        std::cout << *this;
    }
}

void TextDisplay::updateFromCell(Cell* cell){

    int cellRowPos = cell->getRow();
    int cellColumnPos = cell->getColumn();

    assert((cellRowPos >= 0) && (cellRowPos < num_rows_) && (cellColumnPos >= 0) && (cellColumnPos < num_columns_));

    char cell_char = cell->getCharacter();

    if (DEV_TESTING){
        std::cout << "TEST (GD): Updating TD: Row: " << cellRowPos << " Column: " << cellColumnPos << " to \'" << cell_char << "\'" << std::endl;
    }

    grid_[cellRowPos][cellColumnPos] = cell_char;


}
void TextDisplay::updateFromGame(Game* game){
    current_score_ = game -> get_current_score();
    high_score_ = game -> get_high_score();
    next_block_char_ = game -> getNextBlock().get() -> getCharacter();
    level_ = game -> get_current_level();
}

//thought about iterator pattern and tried it, but it is not worth it
std::ostream &operator<<(std::ostream &cout, const TextDisplay &display)
{

    cout << "         Level: " << display.level_ << std::endl;
    cout << "         Score: " << display.current_score_ << std::endl;
    cout << "    High Score: " << display.high_score_ << std::endl;
    cout << "      ---------------------" << std::endl;

    //print the actual board
    for (unsigned int i = 0; i < display.num_rows_; ++i)
    {
        cout << i + 1 << "   ";
        if (i + 1 < 10)
        {
            cout << " "; //extra space so that double digits and single digit lines match up
        }
        for (unsigned int j = 0; j < (display.num_columns_ * 2) + 1; ++j)
        {
            if (j % 2 == 0)
            {
                cout << "|";
            }
            else
            {
                int real_x_value = j / 2; //due to x cursor account for '|', floor divide by 2 will get index we want
                assert((real_x_value >= 0) && (real_x_value < display.num_columns_));
                cout << display.grid_[i][real_x_value];
            }
        }
        cout << std::endl;
    }

    //print next block

    cout << "--" << std::endl;
    cout << "Next Block:" << std::endl;


    char next_block_character = display.next_block_char_;

    switch(next_block_character){

        case 'I':
            cout << "IIII" << std::endl;
            break;

        case 'J':
            cout << "J" << std::endl;
            cout << "JJJ" << std::endl;
            break;

        case 'L':
            cout << "  L" << std::endl;
            cout << "LLL" << std::endl;
            break;

        case 'O':
            cout << "OO" << std::endl;
            cout << "OO" << std::endl;
            break;

        case 'S':
            cout << " SS" << std::endl;
            cout << "SS"  << std::endl;
            break;

        case 'Z':
            cout << "ZZ" << std::endl;
            cout << " ZZ" << std::endl;
            break;

        case 'T':
            cout << "TTT" << std::endl;
            cout << " T " << std::endl;
            break;

        default:
            cout << "Invalid Character read from next block" << std::endl;
            break;
    }

    cout << "-----------------------------------" << std::endl;

    return cout;
}
