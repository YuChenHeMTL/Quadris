#ifndef _Action_
#define _Action_

enum Action
{
    left,
    right,
    down,
    clockwise,
    counterclockwise,
    drop,
    levelup,
    leveldown,
    norandomfile,
    restorerandom, //this is actually command random, but random is keyword so had to rename
    I,
    J,
    L,
    S,
    Z,
    O,
    T,
    restart,
    hint,
    endgame, //this is not a command, will try to detect EOF to end game
    start,
    AItakeover,
    ACTION_SIZE
};

#endif
