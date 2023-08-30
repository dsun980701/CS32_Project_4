#include "MatchMaker.h"
#include "MemberDatabase.h"
#include "AttributeTranslator.h"
#include "PersonProfile.h"
#include "utility.h"
#include "RadixTree.h"
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;


MatchMaker::MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at)
:m_memberdatabase(&mdb), m_translator(&at)
{}

std::vector<EmailCount> MatchMaker::IdentifyRankedMatches(std::string email, int threshold) const
{
    vector<EmailCount> none;// in case of failure, return this empty vector
    unordered_map<string,int> ranks; // will hold emails to counts before changing to vector form for return value
    
    const PersonProfile* subj = m_memberdatabase->GetMemberByEmail(email);// search the personprofile of the given email
    if (subj == nullptr)// if not present, return empty vector
        return none;
    unordered_map<string,AttValPair> compatiable_ATV;//will map All non-overlapping compatialbe AttValPairs based on the AttValPair that the subject PersonProfile has
    for (int k = 0; k != subj->GetNumAttValPairs(); k++)//for all source AttValPair that subj PersonProfile has
    {
        AttValPair av;
        subj->GetAttVal(k, av);
        const vector<AttValPair>& temp = m_translator->FindCompatibleAttValPairs(av);// get the compatible AttValPair based on the source AttValPair
        for (auto pp = temp.begin(); pp != temp.end(); pp++)//Map each non-overlaping compatible AttValPair into the compatiable_ATV unordered_map
        {
            auto kk = compatiable_ATV.find(stringHash((*pp)));
            if (kk == compatiable_ATV.end())
                compatiable_ATV[stringHash(*pp)] = *pp;
        }
    }
    
    for (auto p = compatiable_ATV.begin(); p != compatiable_ATV.end();p++) // for all compatiable AttValPairs
    {
        const vector<string>& matchEmail = m_memberdatabase->FindMatchingMembers((*p).second); // get vector which has all emails that have the compatiable AttValPair
        for (auto i = matchEmail.begin(); i != matchEmail.end(); i++) // for each of the matchEmails
        {
            if (*i == email)// if the match email is identical to the caller email, continue
                continue;
            auto loc = ranks.find(*i);//else find if the unordered_map has the email already mapped
            if (loc == ranks.end()) // if there is no email in the rank map
                ranks[*i] = 1; // set count to 1, and insert the mapping
            else//if the email is already mapped
                ranks[*i] ++; //increment count
        }
    }
    
    vector<EmailCount> r;//vector to transfer all the email and counts from the ranking, if threshold is passed
    for (auto p = ranks.begin(); p != ranks.end(); p++)// for each of the mapping in the ranks unordered_map
    {
        if ((*p).second >= threshold)//if the count is above or same as threshold
        {
            EmailCount temp((*p).first,(*p).second);
            r.push_back(temp);//insert the EmailCount to the return value vector "r"
        }
    }
    
    std::sort(r.begin(),r.end(), comp); // sort the return value vector"r" using comp function specified in utility

    return r;
}
