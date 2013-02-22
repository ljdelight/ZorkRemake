
#include "Room.h"

Room::Room( const string &tempName, const string &descr, const string &inspec )
: BaseInformation(tempName, descr, inspec)
{
    // do nothing
}

// addRoom() -- links this room to another
//
// PRE: must be a valid pointer!
// POST: nextRooms and dirNames have more items
void Room::addRoom( const string &dirName, Room* const nextRoom)
{
    if ( nextRoom == NULL )
    {
        string e = "Room \"" + this->getName() + "\" given null pointer for direction \"" + dirName + "\"";
        throw BadRoomPointer(e);
    }
    nextRooms.push_back( nextRoom );
    directionNames.push_back( dirName );
}

// getNextRoom() -- returns Room* for room given direction
//
// PRE: dirName needs to be in the direction names
// POST: returns a const ptr to next room
Room* const Room::getNextRoom( const string &dirName ) const throw ( BadDirection )
{
    // directionNames and nextRooms are parallel arrays so we need to know where this value is
    size_t i;
    for ( i = 0; i < directionNames.size() && directionNames[i] != dirName; i++ )
    {
        ; // do nothing
    }

    if ( i != directionNames.size() )
    {
        return nextRooms[i];
    }
    else
    {
        throw BadDirection();
    }
}

// insert() -- puts an item into the room
//
// PRE: none
// POST: item is now in room
void Room::insert( const Item &item ) throw ()
{
    items.push_back(item);
}

// hasA() -- does the room have this item?
//
// PRE: none
// POST: returns T of F
bool Room::hasItem(const string& itemStr) const
{
    list<Item>::const_iterator itr;
    for ( itr = items.begin(); (itr != items.end()) && (itr->toString() != itemStr); ++itr )
    {
        // nothing
    }

    if ( itr != items.end() )
    {
        return true;
    }
    return false;
}

// hasDirection() -- does this room have direction dir?
//
// PRE: none
// POST: none
bool Room::hasDirection( const string &dir) const
{
    vector<string>::const_iterator itr;
    for ( itr = directionNames.begin(); itr != directionNames.end(); ++itr )
    {
        // the room does have an exit at direction dir! return true;
        if ( dir == *itr)
        {
            return true;
        }
    }
    return false;
}

// examine(str) -- returns the Item's examination
//
// PRE: none
// POST: finds the item, then returns examineString(). If it isn't located, throws exception
string Room::examine(const string& itemString) const throw (MissingItem)
{
    // keep going until the end or we find the right Item
    list<Item>::const_iterator itr;
    for ( itr = items.begin(); (itr != items.end()) && (itr->toString() != itemString); ++itr )
    {
        // nothing
    }

    if ( itr != items.end() )
    {
        return itr->examineString();
    }
    else
    {
        throw MissingItem();
    }
}

// remove() -- removes item from room
//
// PRE: item should be in the room
// POST: item is removed from room
Item Room::remove( string itemName ) throw (MissingItem)
{
    // look until end is found or names match
    list<Item>::iterator itr;
    for ( itr = items.begin(); (itr != items.end()) && (itr->getName() != itemName); ++itr )
    {
        ; // do nothing
    }

    if ( itr != items.end() )
    {
        Item temp = *itr;
        items.erase(itr);
        return temp;
    }
    else
    {
        throw MissingItem();
    }
} // end remove()

// toString() -- returns string of room description and the items within the room
//
// PRE: none
// POST: string is returned
string Room::toString() const
{
    // return this
    string ret;

    ret = getName() + "\n" + getDescription();

    // append items if room has items
    if ( !items.empty() )
    {
        ret += "\n You see:";

        // loop through stuff in room
        list<Item>::const_iterator itr;
        for ( itr = items.begin(); itr != items.end(); ++itr )
        {
            ret += "\n    a " + itr->toString();
        }
    }

    return ret;
} // end toString()
