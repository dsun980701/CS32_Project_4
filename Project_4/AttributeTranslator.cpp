#include "AttributeTranslator.h"
#include "provided.h"
#include "utility.h"
#include <fstream>
#include <string>

using namespace std;

AttributeTranslator::AttributeTranslator()
{}

bool AttributeTranslator::Load(std::string filename)
{
    ifstream infile(filename);
    if(!infile)
        return false;
    string line;
    string save[3];// save[0] holds Source_attr, save[1] holds Source_val, save[2] holds Comp_attr
    while(getline(infile,line))
    {
        if(line == "")
            continue;
        size_t a;
        for(int i = 0; i < 3; i++)
        {
            a = line.find(',');
            if (a == string::npos)
                return false;
            save[i] = line.substr(0,a);
            line = line.substr(a+1);
        }
        AttValPair source(save[0],save[1]);//first two strings(seperated by comma) is the source AttValPair
        AttValPair comp(save[2],line);//last two strings(seperated by comma) is the compatiable AttValPair of the source AttvalPair.
        string radix_key = stringHash(source);//use function to make Key for RadixTree relating to the source AttValPair
        std::vector<AttValPair>* radix_value = m_translator.search(radix_key); //find the value(vector of AttValPair) mapped by the key(related to the source AttValPair)
        if (radix_value == nullptr)// if not mapped, insert the compatible AttValPair
        {
            std::vector<AttValPair> temp;
            AttValPair comp(save[2],line);
            temp.push_back(comp);
            m_translator.insert(radix_key, temp);
        }
        else//if already mapped,
        {
            bool overlap = false;
            for(auto p = radix_value->begin(); p != radix_value->end();p++) // see if the current compatiable AttValPair is already in the vector. (For complexity analysis, since it stated in the spec that we can assume there to be a fixed number of Compatible AttValPair given a source AttValPair. This linear search can be deemed a constant time operation).
            {
                if (*p == comp)
                {
                    overlap = true;
                    break;
                }
            }
            if(!overlap)
                radix_value->push_back(comp);
        }
    }
    return true;
}

std::vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair& source) const
{
    string radix_key = stringHash(source);//use the same convertor as insertion for Radixtree Key related to source AttValPair
    std::vector<AttValPair>* radix_value = m_translator.search(radix_key);//search
    if (radix_value == nullptr) // if not found, return empty vector
    {
        std::vector<AttValPair> a;
        return a;
    }
    else
        return *radix_value;//if found, return that vector which stores all the compatible AttValPair
}
