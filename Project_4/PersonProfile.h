#ifndef PERSONPROFILE_INCLUDED
#define PERSONPROFILE_INCLUDED
#include <string>
#include <vector>
#include <unordered_set>
#include "RadixTree.h"

struct AttValPair;

class PersonProfile {
public:
    PersonProfile(std::string name, std::string email);
    std::string GetName() const;
    std::string GetEmail() const;
    void AddAttValPair(const AttValPair& attval);
    int GetNumAttValPairs() const;
    bool GetAttVal(int attribute_num, AttValPair& attval) const;
    
private:
    std::string m_name; // holds name of the Person
    std::string m_email; //hold email of the person
    int m_AV_pairs;// holds number of AttValParis the person has
    RadixTree<std::unordered_set<std::string>> m_data; // Radix Tree for AttValParis that a person has, which maps key to strings.
    struct KeyAndIndex{//this holds the key and indext(within the unrordered_set) for GetAttVal function.
        KeyAndIndex(int index, std::string key):m_index(index),m_keys(key){}
        int m_index;
        std::string m_keys;
    };
    std::vector<KeyAndIndex> m_keys;// vector of the KeyAndIndex for keeping track of the KeyAndIndex present in the unordered_set within the RadixTree.
};


#endif /* PersonProfile_hpp */
