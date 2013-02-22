/*
 * NOTES:
 *  - using a list for the items so I can add/remove in O(1)
 *
 */
#ifndef _ROOM_H
#define	_ROOM_H

#include <list>
using std::list;

#include "BaseInformation.h"
#include "Exceptions.h"
#include "Item.h"

class Room : public BaseInformation
{
public:

    Room( const string &tempName, const string &descr, const string &inspec );

    void addRoom( const string &dirName, Room* const nextRoom);
    void insert( const Item &item ) throw ();
    Item remove( string itemName ) throw (MissingItem);
    Room* const getNextRoom( const string &dirName ) const throw ( BadDirection );
    
    bool hasItem(const string& itemStr) const;
    bool hasDirection( const string &dir) const;
    string examine(const string& itemString) const throw (MissingItem);
    
    virtual string toString() const;
    
private:
    list<Item> items;
    vector<Room*> nextRooms;
    vector<string> directionNames;
};

#endif

