#include "GraphicDisplay.h"
#include <cassert>

const bool DEV_TESTING = false;

GraphicDisplay::GraphicDisplay() : next_block_char_(' '), level_(-1), current_score_(-1), high_score_(-1) {

}


void GraphicDisplay::update(Subject *subjectNotifying)
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
    }
}

void GraphicDisplay::updateFromCell(Cell* cell){

    int cellRowPos = cell->getRow();
    int cellColumnPos = cell->getColumn();

    assert((cellRowPos >= 0) && (cellRowPos < num_rows_) && (cellColumnPos >= 0) && (cellColumnPos < num_columns_));

    char cell_char = cell->getCharacter();
    
    std::string cell_colour = getCellColour(cell_char);

    if (!cell_colour.empty()){
        updateCellColour(cellRowPos, cellColumnPos, cell_colour);
    }

}

std::string GraphicDisplay::getCellColour(char cell_char){

    std::string cell_colour = "";

    switch(cell_char){

        case ' ':
            cell_colour = "White";
            break;
        case 'I':
            cell_colour = "Cyan";
            break;
        case 'J':
            cell_colour = "Dark Blue";
            break;
        case 'L':
            cell_colour = "Dark Orange";
            break;
        case 'O':
            cell_colour = "Yellow";
            break;
        case 'S':
            cell_colour = "Green";
            break;
        case 'Z':
            cell_colour = "Red";
            break;
        case 'T':
            cell_colour = "Purple";
            break;
        case '*':
            cell_colour = "Sienna";
            break;
        case '?':
            cell_colour = "Black";
            break;
        default:
            std::cerr << "Unable to decipher cell of char: " << cell_char << std::endl;
            break;
    }

    return cell_colour;


}

void GraphicDisplay::updateFromGame(Game* game){

    int get_current_score = game -> get_current_score();
    int get_high_score = game -> get_high_score();
    int get_current_level = game -> get_current_level();
    char get_next_block_char = game -> getNextBlock().get() -> getCharacter();


    // Only updates when the game data has changed, otherwise there is no need to redraw
    if (get_current_score != current_score_){
        current_score_ = get_current_score;
        updateCurrentScore(current_score_);
    }
    
    if (get_high_score != high_score_){
        high_score_ = get_high_score;
        updateHighScore(high_score_);
    }
    
    if (get_current_level != level_){
        level_ = get_current_level;
        updateCurrentLevel(level_);
    }

    if (get_next_block_char != next_block_char_){
        next_block_char_ = get_next_block_char;
        updateNextBlock(next_block_char_);
    }

    window_.drawGridLines();
}

void GraphicDisplay::updateCurrentScore(int current_score){
    window_.drawCurrentScore(current_score);
}
void GraphicDisplay::updateHighScore(int high_score){
    window_.drawHighScore(high_score);
}
void GraphicDisplay::updateCurrentLevel(int current_level){
    window_.drawCurrentLevel(current_level);
}

void GraphicDisplay::updateNextBlock(char next_block_char){
    window_.drawNextBlock(next_block_char);
}


void GraphicDisplay::updateCellColour(int row, int column, std::string colour){
    
    assert((row >= 0) && (row < num_rows_) && (column >= 0) && (column < num_columns_));

    if (DEV_TESTING){
        std::cout << "TEST (GD): Updating Cell: Row: " << row << " Column: " << column << " to colour: " << colour << std::endl;
    }

    window_.drawCell(row,column,colour);
}
