/*
 *  NOTES:
 *    - Should not have items of the same name that are expected to be different
 */
#ifndef _ITEM_H
#define	_ITEM_H

#include <vector>
using std::vector;

#include "BaseInformation.h"

class Item : public BaseInformation
{
public:
    Item( const string &name, const string &descr, const string &inspec, const vector<string> &depends );
    
    const vector<string>& getDependencies() const;
    virtual string toString() const;
private:
    vector<string> dependencies;
};
#endif

