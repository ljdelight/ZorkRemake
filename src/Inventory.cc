
#include "Inventory.h"

Inventory::Inventory()
    : BaseInformation("inventory", "this contains your items"), MAX_ITEMS(10)
{
    
}

// toString() -- returns list of items as a string
//
// PRE: none
// POST: returns list of your items
string Inventory::toString() const
{
    if ( items.empty() )
    {
        return "You have nothing.";
    }
    else
    {
        // return this
        string ret = "You are carrying:";

        // loop through inventory
        list<Item>::const_iterator itr;
        for ( itr = items.begin(); itr != items.end(); ++itr )
        {
            ret += "\n\ta " + itr->toString();
        }

        return ret;
    }
} // end toString()

// examine() -- returns examineString
//
// PRE: item must be in list
// POST: returns string
string Inventory::examine(const string& itemString) const throw (MissingItem)
{
    // keep going until the end or we find the right Item
    list<Item>::const_iterator itr;
    for ( itr = items.begin(); (itr!=items.end())&&(itr->toString()!=itemString); ++itr )
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

// hasA() -- does the inventory contain "item"?
//
// PRE: none
// POST: none
bool Inventory::hasA( const string& itemStr) const
{
    list<Item>::const_iterator itr;
    for ( itr = items.begin(); (itr!=items.end())&&(itr->toString()!=itemStr); ++itr )
    {
        // nothing
    }
    // item is in list if we're not at the end
    if ( itr != items.end() )
    {
        return true;
    }
    return false;
}

// insert() -- inserts an item if invent is not full
//
// PRE: inventory is not Full.
// POST: item is added
bool Inventory::insert(const Item& item)
{
    if ( !isFull() )
    {
        items.push_back(item);
        return true;
    }
    else
    {
        return false;
    }
}

// remove() -- removes item, if present
//
// PRE: item in inventory
// POST: item is removed from inventory
Item Inventory::drop( const string &nameToRemove ) throw (MissingItem)
{
    // look until end is found or names match
    list<Item>::iterator itr;
    for ( itr = items.begin(); (itr != items.end())&&(itr->getName() != nameToRemove); ++itr )
    {
        ; // do nothing
    }

    // indicates that the names matched
    if ( itr != items.end() )
    {
        Item temp = *itr;

        // remove the item
        items.erase( itr );

        return temp;
    }
    else
    {
        throw MissingItem();
    }
}

// isFull() -- is the inventory full?
bool Inventory::isFull() const
{
    return items.size() >= MAX_ITEMS;
}
