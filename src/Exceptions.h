/*
 * This is a list of the exceptions the program uses.
 * All of my (recoverable) exceptions inherit from invalid_argument so I must use this LAST when catching
 *    so I can respond correctly
 */

#ifndef _EXCEPTIONS_H
#define	_EXCEPTIONS_H

#include <string>
using std::string;

#include <stdexcept>
using std::invalid_argument;
using std::runtime_error;


class FileException : public runtime_error {
public:
    FileException(const string& file) : runtime_error(file)
    {
        // what() is a derived fctn that will be used
    }
};

class BadDirection : public invalid_argument
{
public:
    BadDirection() : invalid_argument("You cannot go that way.")
    {
        
    }
};

class MissingItem : public invalid_argument
{
public:
    MissingItem() : invalid_argument("That item is not here.")
    {

    }
};

class BadRoomPointer : public runtime_error
{
public:
    BadRoomPointer( const string &s ) : runtime_error(s)
    {
        
    }
};

#endif

