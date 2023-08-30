#ifndef MATCHMAKER_INCLUDED
#define MATCHMAKER_INCLUDED
#include <vector>
#include <string>
#include "provided.h"

class MemberDatabase;
class AttributeTranslator;
struct EmailCount;

class MatchMaker{
public:
    MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at);
    std::vector<EmailCount> IdentifyRankedMatches(std::string email, int threshold) const;
    
private:
    const MemberDatabase* m_memberdatabase;
    const AttributeTranslator* m_translator;
};


#endif /* MatchMaker.h */
