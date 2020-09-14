#ifndef _TEXTDISPLAY_
#define _TEXTDISPLAY_

#include <iostream>
#include "Observer.h"

#include "Block.h"
#include "Subject.h"
#include "Cell.h"
#include "Game.h"

class Observer;
class Subject;
class TextDisplay;
class Cell;
class Game;

class TextDisplay: public Observer{

    public:

    TextDisplay();
    void update(Subject* subjectNotifying) override;
    void initGrid();
    friend std::ostream &operator<<(std::ostream & cout, const TextDisplay & display);

    private:
        
        //private helpers for update
        void updateFromCell(Cell* cell);
        void updateFromGame(Game* game);

        //need to get what block type
        char next_block_char_;
        int level_;
        int current_score_;
        int high_score_;

        static const int num_rows_ = 18;
        static const int num_columns_ = 11;
        char grid_ [num_rows_] [num_columns_];

};
#endif