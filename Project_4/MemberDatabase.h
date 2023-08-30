#ifndef MEMBERDATABASE_INCLUDED
#define MEMBERDATABASE_INCLUDED
#include <vector>
#include <string>
#include "RadixTree.h"
#include "provided.h"

class PersonProfile;

class MemberDatabase
{
public:
    ~MemberDatabase();
    bool LoadDatabase(std::string filename);
    std::vector<std::string> FindMatchingMembers(const AttValPair& input) const;
    const PersonProfile* GetMemberByEmail(std::string email) const;
    
private:
    RadixTree<PersonProfile*> m_database;//Maps email-address(key) TO a pointer to PersonProfile(value). Instead of PersonProfile, a pointer to the PersonProfile is used as the "value" of radixtree because the RadixTree and PersonProfile both does not have a copy constructor, and the RadixTree is required to have a Copy of the passed in value. If a PersonProfile is copy constructed using the default copy constructor, all its data member is simply copied. As a result, the copy version of the Personprofile(within THIS radixtree) will have a default-copy-constructed Radixtree(which will simply copy the m_head of the radixtree within the PersonProfile). As a result, the copy of the PersonProfile within THIS radixtree will have an identical radix tree(since their m_head is identical), and when this class is destructred, there will be error because the identical radix-tree(holding m_head pointer to the identical root Node) was already destroyed at the end of the LoadDatabase() function: 1. At the end of the LoadDatabase() function call, temporary Radixtree made during creation of temporary Personprofile used within the LoadDatabase() function, will be destroyed after the function call, destroying the radixtree within the temporary Personprofile, and hence the Radixtree with the copied Personprofile in m_database radixtree. 2. When this class is destructed, the default destructor of the Personprofile will try to eliminate the data member RadixTree, resulting in initation of RadixTree's destructor. Hence dynamic memory for personprofile is made, and m_database Radixtree will have value of pointer to the Personprofile.
    RadixTree<std::vector<std::string>> m_AVPtoEmail; // Maps AttValPair to email addresses. Uses key derived from AttValPair, and has value of vector of emails.
    std::vector<PersonProfile*> deletion;// For deletion of allocated PersonProfile
    
};

#endif /* MemberDatabase_hpp */
