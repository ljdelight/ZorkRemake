
// TODO:
//   -- remove virtual?

#ifndef _BASEINFORMATION_H
#define	_BASEINFORMATION_H

#include <string>
using std::string;

class BaseInformation
{
public:
    BaseInformation()
    {
        name = "";
        description = "";
        examineStr = "";
    }
    BaseInformation( string n, string desc, string inspec = "" )
    {
        name = n;
        description = desc;
        examineStr = inspec;
    }
    virtual ~BaseInformation() {};

    virtual string toString() const =0; // pure virtual

    // function names are self explanatory
    virtual string getName() const
    {
        return name;
    }
    virtual string getDescription() const
    {
        return description;
    }
    virtual string examineString() const
    {
        return examineStr;
    }

private:
    string name;
    string description;
    string examineStr;
};

#endif

