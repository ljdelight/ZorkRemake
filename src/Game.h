/*
 * NOTES:
 *  - Input file itemsIn.txt is structured EXACTLY as (ignore '|')
 *       |name (single word, lowercase)
 *       |description
 *       |onInspect info
 *       |items this item depends on (single words, lowercase, separated by space)
 *       |(blank line)
 *  - I use a map for the commands. It's cool. I might have a problem when I need to return something from a command,
 *         but I think using inheritance and map the base class then use dynamic_cast to access its functions. Maybe.
 *  - I got lazy when I made the actual storyline, sorry about exit(0) at different spots
 *
 * TODO:
 *  -- catch new?
 *
 */
#ifndef _GAME_H
#define	_GAME_H

#include <map>
using std::map;
using std::make_pair;

#include "Room.h"
#include "Inventory.h"

class Game {

    // typedef for void returning function pointer, COMMAND_FUNCTION. Otherwise i would have to type: void (Game::*)(void),
    //      everytime I needed the type. Using an iterator without this would get irritating.
    typedef void (Game::*COMMAND_FUNCTION)(void);
    
public:
    Game();
    ~Game();
    void run();
    void printItemsInGame() const throw ();
    
private:
    void loadItems() throw ( FileException );
    void validateDependencies() const throw ( FileException);

    // commands used as functions
    void go();
    void kill();
    void get();
    void drop();
    void look();
    void examine();
    void help();
    void hide();
    void quit();

    Room* zergLair;
    Room* landOfDead;
    bool aboutToDie;
    bool living;
    Inventory inventory;
    Room* currentRoom;
    vector<Room *> rooms;
    vector<Item *> gameItems;
    map<string, COMMAND_FUNCTION> commands;
    string objStr;
};
#endif
