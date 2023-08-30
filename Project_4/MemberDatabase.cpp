#include "MemberDatabase.h"
#include <string>
#include <fstream>
#include "provided.h"
#include "PersonProfile.h"
#include "utility.h"

using namespace std;

MemberDatabase::~MemberDatabase()
{
    auto p = deletion.begin();
    while(p != deletion.end())
    {
        delete *p;
        p = deletion.erase(p);
    }
}

bool MemberDatabase::LoadDatabase(std::string filename)
{
    ifstream infile(filename);
    if(!infile)
        return false;
    string line;
    string name,email;
    int num_pairs;
    while(getline(infile,line))
    {
        if(line == "")
            continue;
        name = line;
        if(!getline(infile,line))
            return false;
        email = line;
        if(!getline(infile,line))
            return false;
        PersonProfile* profile_v = new PersonProfile(name,email); // Explained in Header
        num_pairs = stoi(line);//Number of attribute val pairs that the person has
        for(int i = 0; i < num_pairs;i++)
        {
            if(!getline(infile,line))
                return false;
            size_t a = line.find(',');//Attribute and Value seperated by comma
            if (a == string::npos)
                return false;
            AttValPair att_val(line.substr(0,a),line.substr(a+1));
            profile_v->AddAttValPair(att_val);//Add the AttValPair in the profile
            string keyForR_2 = stringHash(att_val);//Get key for the RadixTree which maps AttValPair to emails
            vector<string>* emailsWATV = m_AVPtoEmail.search(keyForR_2);// search if it is in the radix tree
            if (emailsWATV == nullptr)//if not mapped in radix-tree
            {
                vector<string> input;
                input.push_back(email);
                m_AVPtoEmail.insert(keyForR_2, input);
            }
            else//if already mapped in radix-tree
            {
                bool overlapping = false;
                for(auto p = emailsWATV->begin(); p != emailsWATV->end();p++)//Linear search for overlapping email, as time complexity for insertion was not specified. Use of vector will induce longer insertion time, but faster access time when searching compared to unordered_set.
                {
                    if (*p == email)
                    {
                        overlapping = true;
                        break;
                    }
                }
                if(!overlapping)
                {
                    emailsWATV->push_back(email);
                }
            }
        }

        PersonProfile** radix_value = m_database.search(email);// Search for email
        if (radix_value == nullptr)//if not already mapped in the radix-tree
        {
            m_database.insert(email, profile_v);
            deletion.push_back(profile_v);
        }
        else//if there is overlapping email
        {
            delete profile_v;
            return false;
        }
    }
    return true;
}

std::vector<std::string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const
{
    string key = stringHash(input);
    vector<string>* output = m_AVPtoEmail.search(key);
    if (output == nullptr)
    {
        vector<string> none;
        return none;
    }
    else
        return *output;
}

const PersonProfile* MemberDatabase::GetMemberByEmail(std::string email) const
{
    PersonProfile** temp = m_database.search(email);
    if (temp == nullptr)
        return nullptr;
    else
        return *(temp);
}
