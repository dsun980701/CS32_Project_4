#include "utility.h"
#include "provided.h"
#include <string>

std::string stringHash(const AttValPair& av)
{
    return av.attribute +"DI@UH(,P9h"+ av.value;//Since key in the Radix tree is "string" type, sortta hash the key by directly using 'randomly' included 10 chars so that identical attr and value, for instance "a"-"aa" can't be confused with "aa"-"a". Hence this method would yield about 9.91504E-20 % chance of overlapping key, because at least the "key" must be identical to those 'randomly' included 10 chars to confuse key and value. Also ',' is included. 
}

bool comp(const EmailCount& lhs, const EmailCount& rhs)
{
    if (lhs.count > rhs.count)//First sort with descending Counts
        return true;
    if (lhs.count < rhs.count)
        return false;
    //Then sort with alphabetical order of the email. Because the emails have upper and lower cases, in order to compare them strictly alphabetically, their cases have to be all lower (or upper) case.
    std::string lhs_c = lhs.email;
    std::string rhs_c = rhs.email;
    for(int i = 0; i < lhs_c.size(); i++)
        lhs_c[i] = tolower(lhs_c[i]);
    for(int i = 0; i < rhs_c.size(); i++)
        rhs_c[i] = tolower(rhs_c[i]);
    return (lhs_c) < (rhs_c);
}

