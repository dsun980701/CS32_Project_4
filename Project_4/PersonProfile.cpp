#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include "utility.h"
#include "PersonProfile.h"
#include "RadixTree.h"
#include "provided.h"

using namespace std;

PersonProfile::PersonProfile(std::string name, std::string email)
:m_name(name), m_email(email),m_AV_pairs(0)
{}

std::string PersonProfile::GetName() const
{
    return m_name;
}

std::string PersonProfile::GetEmail() const
{
    return m_email;
}

void PersonProfile::AddAttValPair(const AttValPair& attval)
{
    string key = attval.attribute; // key to the radixtree
    unordered_set<string>* a = m_data.search(key);// search whether the attribute is already mapped within the Radixtree
    if (a == nullptr) //If it is not mapped,
    {
        KeyAndIndex temp(0,key);//first save the key, and the index(which is 0 since the unordered_set is now getting initialized for the first time for the given key)
        m_keys.push_back(temp);
        unordered_set<string> radix_value;//make a new unordered_set
        radix_value.insert(attval.value);//insert the value
        m_data.insert(key,radix_value);//insert the key and value into radixtree
        m_AV_pairs++;
        return;
    }
    else//if the key is already present in the Radixtree
    {
        if((a->insert(attval.value)).second)//insert the "value" into the unordered_set. If the insertion was successful(meaning no overlap), do the following.
        {
            int index = int(a->size()); // find the size of the unordered set(after the newly inserted value)
            KeyAndIndex temp(index-1,key);// create a KeyAndIndex for saving
            m_keys.push_back(temp);
            m_AV_pairs++;
        }
    }
}

int PersonProfile::GetNumAttValPairs() const
{
    return m_AV_pairs;
}

bool PersonProfile::GetAttVal(int attribute_num, AttValPair& attval) const
{
    if (attribute_num < m_keys.size())//if the passed-in "attribute_num" is valid
    {
        KeyAndIndex k_I = m_keys[attribute_num];//find the Key[for radixtree] and and index[for unordered_set] in the vector.
        unordered_set<string>* a = m_data.search(k_I.m_keys);//find the unordered_set in the Radixtree using the Key
        if (a != nullptr)//If the Key is mapped in the Radixtree
        {
            string val;
            int i = 0;
            auto p = (*a).begin();
            for(; i < k_I.m_index; p++)//using iterator, traverse through the unordered_set x times; where x is the number specfied by the index stored in k_I.
            {
                i++;
            }
            attval.value = (*p);// after traversing the unordered_set, for a specified number, assign the value in the current node of the unordered_set to the returning value.
            attval.attribute = k_I.m_keys;
            return true;
        }
        
    }
    return false;
}
