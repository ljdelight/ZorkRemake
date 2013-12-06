/* 
 * Lucas Burson
 *
 * December 6, 2008, 10:45 PM
 * Estimated time: unknown
 * Total time: ~14hrs (missing some below)
 *   Dec6
 *    - 11pm-12: made class prototype outline/UML on paper
 *    - 12am-2: coded class fctns
 *   Dec9
 *    - 12am-2 : added stuff to Game.h: commands and item input items from file
 *   Dec10
 *    - 8pm-10: destructors, separated .h and .cc files, something else
 *   Dec11
 *    - 8pm-12: made story and created the rooms for it. Modified commands to use map instead
 *
 * NOTES:
 *  - You might not like it, but I use "using std::xxx;" because it helps me memorize what I'm using. (used in .h and .cc files)
 *  - If you enter "go" or something that requires an object, type "go north" so there isn't a pause.
 *        I was going to implement something to say "Which direction?" if nothing was given but I never started that task.
 *  - You may want to try killing yourself (In the game silly!)
 *
 * TODO:
 *  -- make rooms... (files?)
 *         could read items in, then rooms. Seems cool too.
 *
 */
#include <cstdlib>
#include <iostream>

#include "Game.h"

using namespace std;

int main()
{
    Game game;

    // run the game
    game.run();
    exit(0);
}

