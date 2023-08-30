#ifndef ATTRIBUTETRANSLATOR_INCLUDED
#define ATTRIBUTETRANSLATOR_INCLUDED
#include <string>
#include <vector>
#include "RadixTree.h"

struct AttValPair;

class AttributeTranslator
{
public:
    AttributeTranslator();
    bool Load(std::string filename);
    std::vector<AttValPair> FindCompatibleAttValPairs(const AttValPair& source) const;
    
private:
    RadixTree<std::vector<AttValPair>> m_translator; // Stores all the translations. The radix tree maps source AttValPair to a vector of Compatiable AttValPairs specific to the source AttValPair. Key is dervided from the source AttValPair, and value is a vector of compatiable AttValPiar.
    
};
#endif /* AttributeTranslator.h */
