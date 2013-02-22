/*
 * NOTES:
 *  - using a list to store the items in inventory so I can add/remove in O(1)
 *
 */
#ifndef _INVENTORY_H
#define	_INVENTORY_H

#include <list>
using std::list;

#include "Exceptions.h"
#include "BaseInformation.h"
#include "Item.h"

class Inventory : public BaseInformation
{
public:
    Inventory();

    virtual string toString() const;
    
    bool insert( const Item& item );
    Item drop( const string &nameToRemove ) throw (MissingItem);
    string examine( const string &itemString ) const throw (MissingItem);
    bool hasA( const string& itemStr) const;
    bool isFull() const;

private:
    const unsigned int MAX_ITEMS;
    list<Item> items;
};

#endif	

