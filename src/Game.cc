/*
 * NOTES:
 *  - I use the typedef for COMMAND_FUNCTION in this file (declared in Game.h, start of class)
 *
 */

#include <cstdlib>
using std::exit;
using std::rand;
using std::srand;

#include <ctime>
using std::time;

#include <iostream>
using std::cin;
using std::cout;
using std::cerr;

using std::endl;

#include <sstream>
using std::istringstream;

#include <fstream>
using std::ifstream;

#include "Game.h"
using namespace std;
// really easy way to add some function to the map
// Avoids: commands.insert(make_pair("get", &Game::get));
#define MAKE_PAIR(func) make_pair( #func, &Game::func)

// ctor
Game::Game()
{
    // seed using time
    srand( time(NULL) );
    try
    {
        aboutToDie = false;
        living = true;
        
        // load and validate items from input file; throw could occur
        loadItems();
        validateDependencies();

        // place all command name and fctn ptr pairs into the map.
        commands.insert(MAKE_PAIR(go));
        commands.insert(MAKE_PAIR(get));
        commands.insert(MAKE_PAIR(drop));
        commands.insert(MAKE_PAIR(look));
        commands.insert(MAKE_PAIR(examine));
        commands.insert(MAKE_PAIR(help));
        commands.insert(MAKE_PAIR(quit));
        commands.insert(MAKE_PAIR(kill));
        commands.insert(MAKE_PAIR(hide));


        enum RoomNumberings { ROOM_0 = 0, ROOM_1, ROOM_2, ROOM_3, ROOM_4, ROOM_5, NUM_ROOMS };
        rooms.resize(NUM_ROOMS);

        //
        // ---- construct rooms; connections after
        //

        // living
        rooms[ROOM_0] = new Room("-Clearing",
                "All around are dead bodies. To the east is the main path to the Zerg Lair and to the north is your old house.", " " );
        rooms[ROOM_1] = new Room("-Outside Old House",
                "The house remains no longer. Sadly, you see the remains what looks to be your brother. There is a clearing to the south and the Zerg Lair is to the northeast.", " " );
        rooms[ROOM_2] = new Room("-Main Path to Zerg",
                "The path is covered in blood with human skulls guiding the way. There is a clearing is to the west and The Zerg Lair lies to the north.", " ");
        rooms[ROOM_3] = new Room( "-Zerg Lair",
                "The Zerg Lair is of a formidable form. You have been compromised! The Zerg are all around...", " ");
        
        // dead
        rooms[ROOM_4] = new Room( "-Land of the Dead",
                "Everywhere around you are ghastly figures of what was once living. Their murmuring is comforting. You feel cold. There is a path that the ghouls do not cross leading north.", " ");
        rooms[ROOM_5] = new Room( "-Path of the Condemned",
                "Behind you the path is continuously sealed by the Dead. Everything goes quiet with painful silence following. Straight ahead you can see glowing white stairs that lead up.", " ");

        zergLair = rooms[ROOM_3];
        landOfDead = rooms[ROOM_4];

        // connect rooms
        rooms[ROOM_0]->addRoom("north", rooms[ROOM_1] );
        rooms[ROOM_0]->addRoom("east",rooms[ROOM_2]);
        rooms[ROOM_1]->addRoom("south", rooms[ROOM_0]);
        rooms[ROOM_1]->addRoom("northeast", rooms[ROOM_3]);
        rooms[ROOM_2]->addRoom("north", rooms[ROOM_3]);
        rooms[ROOM_2]->addRoom("west", rooms[ROOM_0]);
        rooms[ROOM_3]->addRoom("south", rooms[ROOM_2]);
        rooms[ROOM_3]->addRoom("southwest", rooms[ROOM_1]);

        rooms[ROOM_4]->addRoom("north", rooms[ROOM_5] );
        rooms[ROOM_5]->addRoom("up", zergLair );


        // insert items
        rooms[ROOM_1]->insert(*gameItems[0]); // ring
        rooms[ROOM_5]->insert(*gameItems[1]); // orb

        currentRoom = rooms[ROOM_0]; // "clearing"
    }
    catch (const FileException& e)
    {
        // file did not load correctly. Abort.
        cerr << "ERROR: " << e.what() << ". Aborting.\n   Please review the input file\n";
        exit(1);
    }
    catch ( const BadRoomPointer &e )
    {
        // this is an internal error; cannot be saved.
        cerr << "ERROR: " << e.what() << ". Aborting.\n";
        exit(1);
    }
}
// dtor
Game::~Game()
{
    // reclaim used memory
    vector<Room*>::iterator roomItr;
    for ( roomItr = rooms.begin(); roomItr != rooms.end(); ++roomItr )
    {
        delete *roomItr;
    }
    vector<Item*>::iterator itemItr;
    for ( itemItr = gameItems.begin(); itemItr != gameItems.end(); ++itemItr )
    {
        delete *itemItr;
    }
}

// run() -- manages the game & exits
//
// PRE: none
// POST: game is over
void Game::run()
{
    const int ZERG_PERCENT = 30;
    cout << "Times have changed.\n   You are in a world controlled by a specialized breed of human called the Zurg. Destruction, terror, "
         << "and fear of these fiends have led their long outstanding venial control. This control must stop and"
         << ", by the vicissitudes of fate, your purpose is this.\n   You are not the first to attempt this feat, however, you are special. "
         << "These horrendous montrosities will do much more than laugh at your opposition... if they know of your presence there is no hope for you. "
         << "But you know of their presence before they know of yours, hence why you are special.\n   The Zerg is not composed of individuals, "
         << "they are controlled by an unknown Force. We know nothing of this Force except that it is within the Zerg Lair. Destroy this Force "
         << "and there may be hope.\n\n";


    // print first room description and all of its items
    cout << currentRoom->toString() << endl;

    while (1)
    {
        cout << "> ";
        cin >> objStr;
        cout << endl;
        if ( cin.fail() )
        {
            string clear;
            cin.clear();
            getline(cin,clear);
        }

        if ( aboutToDie && objStr != "hide" )
        {
            cout << "\n\nYou failed the world. There is no longer a reason to hope.\n";
            return;
        }

        map<string, COMMAND_FUNCTION >::const_iterator itr;
        itr = commands.find(objStr);
        if ( itr != commands.end() )
        {
            // super weird looking. Calls the command.
            (this->*(commands[objStr]))();

            // a Zerg is nearby -> hide or die. only works while the player is living
            // rand will be on [1,100]. Gives a ZERG_PERCENT% chance of aboutToDie
            if ( living && ((rand() % 100 + 1) <= ZERG_PERCENT ) )
            {
                cout << "There is a Zerg nearby! Hide before you die!\n";
                aboutToDie = true;
            }
        }
        else
        {
            cout << "I am unsure of the command \"" << objStr << "\".\n";

            // clear cin of possible junk
            getline(cin,objStr);
        }

        if ( currentRoom == zergLair )
        {
            if ( inventory.hasA("orb"))
            {
                cout << "You act quickly and use the power of the orb to destroy the Zerg Lair. Your soul does not survive the orb's power. You died twice to save everything dear to you. Good form.\n";
            }
            else
            {
                cout << "\nThe Zerg begin chewing on your limbs. Your death is slow and painful.\nYou failed the world. There is no longer a reason to hope.\n";
            }
            return;
        }
        else if ( currentRoom == landOfDead ) // player commited suicide
        {
            // only survive if player has the ring
            if ( ! inventory.hasA("ring") )
            {
                cout << "\nYou have failed.\n";
                return; // exit
            }
            else
            {
                cout << currentRoom->toString() << endl;
            }
        }
    } // end while
}

// printItemsInGame() -- prints item names; For debugging
//
// PRE: none
// POST: prints items to cout
void Game::printItemsInGame() const throw ()
{
    vector<Item *>::const_iterator itr;
    for ( itr = gameItems.begin(); itr != gameItems.end(); ++itr )
    {
        cout << "Name: " << (**itr).toString() << '\n'
                << "Desc: " << (**itr).getDescription() << '\n'
                << "Exam: " << (**itr).examineString() << '\n'
                << "DpndC: ";
        vector<string> dep = (**itr).getDependencies();
        for ( unsigned int i = 0; i < dep.size(); i++ )
        {
            cout << dep[i] << ' ';
        }
        cout << "\n\n";
    }
} // end printItemsInGame

// kill ojb; removes it from game unless it's self
void Game::kill()
{
    cin >> objStr;
    if ( currentRoom->hasItem(objStr))
    {
        currentRoom->remove(objStr);
        cout << "You killed it! A little too energetically...\n";
    }
    else if ( objStr == "self" || objStr == "me" || objStr == "myself" || objStr == "yourself" )
    {
        cout << "\nIn an effort to shuffle off this mortal coil, you tightly grasp both hands around "
                << "your scrawny neck and suffocate yourself. A sea of blackness envelops your vision, "
                << "and all noise is replaced by an eerie silence. The cadences of absonence swirl about "
                << "your weakening mind as your heart turns to stone. Nothingness... You awaken to find "
                << "yourself in a world unknown, a place of a foreign origin. This is what is commonly referred to as 'Purgatory'.\n";
        currentRoom = landOfDead;
        living = false;
    }
    else
    {
        cout << "There is no " << objStr << " in the room to kill.\n";
    }
}
// drops item
void Game::drop()
{
    cin >> objStr;
    if ( inventory.hasA(objStr) )
    {
        currentRoom->insert(inventory.drop(objStr));
        cout << "Dropped.\n";
    }
    else
    {
        cout << "You do not have that!\n";
    }
}
// shows commands
void Game::help()
{
    cout << "The commands are:\n\t";
    map<string,COMMAND_FUNCTION>::const_iterator itr = commands.begin();
    
    // it could not at the end, but let's check anyway
    if ( itr != commands.end())
    {
        // print the first command
        cout << itr->first;
        ++itr;
    }
    // print the remaining commands
    while( itr != commands.end() )
    {
        cout << ", " << itr->first;
        ++itr;
    }
    cout << endl;
}
// picks something up
void Game::get()
{
    cin >> objStr;
    if ( currentRoom->hasItem(objStr) )
    {
        if ( inventory.insert(currentRoom->remove(objStr)) )
        {
            cout << "Taken.\n";
        }
        else
        {
            cout << "You are carrying too much.\n";
        }
    }
    else
    {
        cout << "You cannot see any " << objStr << " here!\n";
    }
}
// go dir; move in that direction
void Game::go()
{
    cin >> objStr;
    
    if ( currentRoom->hasDirection(objStr))
    {
        currentRoom = currentRoom->getNextRoom(objStr);
        cout << currentRoom->toString() << endl;
    }
    else
    {
        cout << "You cannot go that way.\n";
    }
}
// shows what is in the current room
void Game::look()
{
    cout << currentRoom->toString() << endl;
}
// changes about to die to false, so the player doesn't die by the Zerg.
void Game::hide()
{
    cout << "Using your clandestine skills, you successfully hide from the Zerg and continue on your way.\n";
    aboutToDie = false;
}
//examine ojb; shows info about the item
void Game::examine() 
{

    cin >> objStr;

    if ( objStr == "inventory" )
    {
        cout << inventory.toString() << endl;;
    }
    else if ( objStr == "room" || objStr == currentRoom->getName() )
    {
        cout << currentRoom->toString() << endl;
    }
    else if ( inventory.hasA(objStr) )
    {
        cout << inventory.examine(objStr) << endl;
    }
    else if ( currentRoom->hasItem(objStr) )
    {
        cout << currentRoom->examine(objStr) << endl;
    }
    else
    {
        cout << "You cannot see any " << objStr << " here!\n";
    }
}
// like exit...
void Game::quit()
{
    cout << "Thanks for playing!!\n";
    exit(0);
}

// validateDependencies() -- ensures that each dependency is an actual Item in gameItems
//
// PRE: none
// POST: if no exception is thrown, the gameItems are valid.
void Game::validateDependencies() const throw ( FileException )
{
    bool found;
    vector<string> currItemDependencies;
    vector<Item*>::const_iterator iItr; // TODO: better names?
    vector<Item*>::const_iterator jItr;

    // for each Item, get its dependency vector, then ensure each dependency is an item.
    for ( iItr = gameItems.begin(); iItr != gameItems.end(); ++iItr )
    {
        found = false;
        currItemDependencies = (*iItr)->getDependencies();

        // for each dependency, check that it is in the gameItems vector
        for ( unsigned int i = 0; i < currItemDependencies.size(); i++ )
        {
            // look at all game Items and find matching name
            for ( jItr = gameItems.begin(); jItr != gameItems.end() && !found; ++jItr )
            {
                if ( currItemDependencies[i] == (*jItr)->getName() )
                {
                    found = true;
                }
            } // end innermost for

            // error if we do not find the Item
            if ( !found )
            {
                string error = "item " + (*iItr)->getName() + " has dependency \"" + currItemDependencies[i]
                        + "\" that was not loaded";
                throw FileException(error);
            }
        } // end for
    } // end outermost for
} // end validateDepencies()

// loadItems() -- loads the items that the game uses. Reads them from the file itemsIn.txt
// BEWARE: this looks nasty but it's not too bad. stringstreams are required to further parse the input after getline()
//
// PRE: itemsIn.txt must exist, an item cannot have a dependency on itself, dependencies must also be an inputted Item
// POST: Item* are added to gameItems so the rooms may use them.
void Game::loadItems() throw ( FileException )
{
    const string FILE_NAME = "itemsIn.txt";

    //
    // !!!!!!! FOUR LINES PER ITEM IN INPUT FILE PLUS ENDING EMPTY LINE !!!!!!!!!!!!
    //
    ifstream in(FILE_NAME.c_str());
    if ( !in )
    {
        string error = "could not locate file: " + FILE_NAME;
        throw FileException(error);
    }

    // initialization of things i use
    const int NAME_INDEX = 0, DESCRIPTION_INDEX = 1, INSPECT_INDEX = 2;
    const int REQUIRED_PARAMS_FOR_ITEM = 4;
    vector<string> data;
    vector<string> dependenciesOfItem;

    string temp;
    int parametersRead = 0;
    while (getline(in, temp))
    {
        // we have read one line and each line has a parameter: increase it
        parametersRead++;

        // if we have read all the required parameters, we're ready to make an item
        if ( parametersRead == REQUIRED_PARAMS_FOR_ITEM )
        {
            // gets the dependencies and puts them into vector (even if it's empty)
            istringstream itemDepends(temp);
            while (itemDepends >> temp)
            {
                dependenciesOfItem.push_back(temp);
            }

            // verify that an item does Not depend on itself
            for ( unsigned int i = 0; i < dependenciesOfItem.size(); i++ )
            {
                if ( dependenciesOfItem[i] == data[NAME_INDEX] )
                {
                    string error = "input file for item \"" + data[NAME_INDEX] + "\" has dependency on itself";
                    throw FileException(error);
                }
            } // end for

            // create item and let the game manage it
            gameItems.push_back(new Item(data[NAME_INDEX], data[DESCRIPTION_INDEX], data[INSPECT_INDEX], dependenciesOfItem));

            // gets the blank line between Item objects in file
            getline(in, temp);

            // set up for next loop iteration
            data.clear();
            dependenciesOfItem.clear();
            parametersRead = 0;
            temp = "";
        } // end outer if
        // pushes the parameters: Item Name, then Description, then Inspection str
        else
        {
            // getline() left the '\n' on the temp string. Weird that door!=door b/c of 5char and 4char lengths.
            //
            // if the current input parameter is Name
            if ( parametersRead - 1 == NAME_INDEX )
            {
                // Removes the nasty extra character
                istringstream oStrS(temp);
                oStrS >> temp;
                data.push_back(temp);
            }
            else
            {
                data.push_back(temp);
            }
        } // end else
    } // end while

    in.close();
} // end loadItems
