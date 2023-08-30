#ifndef UTILITY_INCLUDED
#define UTILITY_INCLUDED
#include <string>

struct AttValPair;
struct EmailCount;

std::string stringHash(const AttValPair& av);//Spits out a string, given a AttValPair, to use as "key" value of a radix tree.
bool comp(const EmailCount& lhs, const EmailCount& rhs);//comparison function for sorting of the matched pair vector

#endif /* utility.h */
