
#include "Item.h"

// default constructor
Item::Item(const string &name, const string &descr, const string &inspec, const vector<string> &depends)
    : BaseInformation(name, descr, inspec)
{
    dependencies = depends;
}

// getDependencies() -- returns the dependencies of this item as a vector<string>
//
// PRE: none
// POST: const vector ref is returned (potentially empty).
const vector<string>& Item::getDependencies() const
{
    return dependencies;
}

// toString() -- returns name (same as getName)
//
// PRE: none
// POST: none
string Item::toString() const
{
    return getName();
}


