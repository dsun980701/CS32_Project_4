#ifndef RADIXTREE_INCLUDED
#define RADIXTREE_INCLUDED

#include <string>
#include <iostream>

template <typename ValueType>
class RadixTree {
public:
    RadixTree(){m_head = new Node;} //Creates the dummy node
    ~RadixTree();
    void insert(std::string key, const ValueType& value);
    ValueType* search(std::string key) const;

private:
    struct Node{ // Node is the each Node of the Radix Tree, it will hold the string that the Node holds(m_curr), boolean expression of whether it is a end of a word(m_leaf), and a value type pointer (m_value)
        Node():m_next(),m_leaf(false),m_val(nullptr){} // Initialized Node has no edge to next node(initialize m_next to all nullptr), not a end of a word, and has no value(m_value).
        std::string m_curr;
        bool m_leaf;
        ValueType* m_val;
        Node* m_next[128];
    };
    void cleanUp(Node* a); // Recursion for deleting all allocated memory
    bool emptyNextAt(Node* a, int b); // Function to whether an edge of char "b" is non-existent at the give node "a". If there is no for edge for "b" in node "a", function returns true, else false
    bool emptyChild(Node* a); // Fcuntion to tell whether the node "a" has any edges. If there are no edges in Node "a", the function retunrs true, else false.
    bool validEntry(const std::string& key) const; // Function to verify the "key" input. If the key input is not within range of 1~127, returns false.
    
    Node* m_head; // Holds the root of the radix tree.
};

template <typename ValueType>
void RadixTree<ValueType>::insert(std::string key, const ValueType& value)
{
    if(!validEntry(key))//Not valid entry
        return;
    
    Node* p = m_head;
    Node* prev = nullptr; // will save the previous Node that was traversed.
    char prev_edge = ','; // will save the char of the edge that it took to arrive at the current node, while traversing the tree. Assigned value is place holder.
    int overlap = 0; // will count the number of overlapping chars that given "key" and the Node's m_curr has in between them.
    std::string prefix = ""; // will save the overlapped string between the "key" and the Node's m_curr.Assigned value is place holder.
    int x = 0; // Switch variable
    
    while(p!= nullptr)
    {
        overlap = 0;//when entering a new node, there is initially no overlap
        prefix = "";// and hence, no overlapping prefix
        for(int i = 0; i < (p->m_curr).size() && i < key.size(); i++)
        {//Search for overlapping prefix between the part of the passed in "key", and the string that is held by the Node in m_curr
            if(p->m_curr[i] == key[i])
            {
                overlap++;
                prefix += key[i];
            }
            else
                break;
        }
        key = key.substr(overlap);// cut down the overlapping part between the string held by Node, and the "key"
        // There are six possible cases after a current "key" string enters a prexisting node, and gets rid of the overlapping part. (For more detailed logical approach of the derivation of these cases, and there boolean expressions, please refer to the report)
        if ((key.size() == 0 && (p->m_curr).size() == 0) || (key.size() == 0 && prefix.size() == p->m_curr.size()))
        {
            x = 1; // The key is already in the tree
            break;
        }
        if (key.size() == 0 && prefix.size() == 0 && (p->m_curr).size() != 0)
        {
            x = 2; //Key became empty while following an edge
            break;
        }
        if (key.size() == 0 && prefix.size() != 0 && prefix.size() != p->m_curr.size())
        {
            x = 3;//key was exahusted while checking overlap, but there is remaining m_curr string. hence need to split p's m_curr
            break;
        }
        if((key.size() != 0 && (p->m_curr).size() == 0 && emptyNextAt(p,key[0])) ||(key.size() != 0 && emptyNextAt(p,key[0]) && prefix.size() == p->m_curr.size()))
        {
            x = 4; // add on a new Node to the current Node
            break;
        }
        if(key.size() != 0 && prefix.size() == 0 && (p->m_curr).size() != 0)
        {
            x = 5; // There is no overlap in this node, but current node has string
            break;
        }
        if(key.size() != 0 && prefix.size() != 0 && prefix.size() != p->m_curr.size())
        {
            x = 6;// There is no path for the key, but there was overlap of prefix
            break;
        }
        prev = p;//save the current Node to prepare for traversal
        prev_edge = key[0]; // save the edge that it is going to take for traversal
        p = p->m_next[key[0]]; // traverse to the next Node, taking an edge
        key = key.substr(1); // take the edge
    }
    
    switch (x){ // Each representation of case # s are specified within the report.
        case 0:
            break;
        case 1:
        {
            p->m_leaf = true;// make the node end of a word.
            ValueType* tempV = new ValueType(value); // assign a copy of the passed in value
            p->m_val = tempV; //into the Node.
            break;
        }
        case 2:
        {
            Node* temp = new Node;//make a new node.
            temp->m_leaf = true;//make the Node end of a word.
            ValueType* tempV = new ValueType(value);// assign a copy of the passed in value
            temp->m_val = tempV;
            prev->m_next[prev_edge] = temp;// Now make the same edge leading from previous Node, point to the newly made node.
            temp->m_next[p->m_curr[0]] = p;// Make a edge from the newly made Node to the current Node, by using the first char of the string that the current Node is holding.
            p->m_curr = (p->m_curr).substr(1);//reduce the first char, which is used when following the newly made edge.
            break;
        }
        case 3:
        {
            Node* temp = new Node;//new Node
            temp->m_leaf = true;//make the Node end of a word.
            temp->m_curr = prefix;//make the newly made Node hold the overlapping prefix as its string.
            ValueType* tempV = new ValueType(value);
            temp->m_val = tempV;// assign a copy of the passed in value
            prev->m_next[prev_edge] = temp; //Now make the same edge leading from previous Node, point to the newly made node.
            p->m_curr = (p->m_curr).substr(overlap);// get rid of the overlapping prefix part from the string that current Node holds.
            temp->m_next[p->m_curr[0]] = p;// make a edge from the newly made Node to the current node.
            p->m_curr = p->m_curr.substr(1);//get rid of the char used during edge.
            break;
        }
        case 4:
        {
            Node* temp = new Node;//new Node
            temp->m_leaf = true;//make the Node end of a word.
            temp->m_curr = key.substr(1);// make the newly made Node hold the key, except for the edge
            ValueType* tempV = new ValueType(value);
            temp->m_val = tempV;// assign a copy of the passed in value
            p->m_next[key[0]] = temp;// make edge from the current to the newly made Node
            break;
        }
        case 5:
        {
            Node* temp1 = new Node;// new Node1 to split, because there was no overlap between "key" and the current string held by current Node.
            prev->m_next[prev_edge] = temp1;// make the previous Node's edge point to this newly made Node1.
            temp1->m_next[p->m_curr[0]] = p;//make an edge to the current Node
            p->m_curr = p->m_curr.substr(1);
            
            Node* temp2 = new Node;//new Node 2, to hold the value and "key"
            temp2->m_leaf = true;// it is an end of a word
            ValueType* tempV = new ValueType(value);
            temp2->m_val = tempV;//assign a copy of the passed in value
            temp2->m_curr = key.substr(1);//assign "key" without the edge
            temp1->m_next[key[0]] = temp2;//make the edge from Node1 to Node2
            break;
        }
        case 6:
        {
            Node* temp1 = new Node;// new Node1; for storing the common prefix between the "key" and the string held by current Node
            temp1->m_curr = prefix;// the Node1 holds the overlapped prefix as its string
            prev->m_next[prev_edge] = temp1;// make the previous Node's edge point to this newly made Node1.
            temp1->m_next[p->m_curr[overlap]] = p;//Connect edge from Node1 to current node.
            p->m_curr = p->m_curr.substr(overlap+1);//get rid of the overlapping prefix and the edge char from the current Node.
            
            Node* temp2 = new Node;//new Node2; for storing the rest of the "key"(the part without overlap), and the "value"
            temp2->m_curr = key.substr(1); // assign the non-overlap part without the edge to the Node2
            temp2->m_leaf = true;// it is an end of a word.
            ValueType* tempV = new ValueType(value);
            temp2->m_val = tempV;//assign a copy of the passed in value.
            temp1->m_next[key[0]] = temp2;// Connect edge from Node1 to Node2
            break;
        }
        default:
            break;
    }
}

template <typename ValueType>
ValueType* RadixTree<ValueType>::search(std::string key) const
{
    if (!validEntry(key))//If Invalid key
        return nullptr;
    
    Node* p = m_head;
    int overlap;
    while(p!= nullptr)//traverse the tree, untill p is nullptr, OR until a boolean expression within the loop "breaks" the loop when it detects that word is in the tree
    {
        overlap = 0;
        for(int i = 0; i < (p->m_curr).size() && i < key.size(); i++)
        {// find overlapping part between string held by the current Node, and the "key"
            if(p->m_curr[i] == key[i])
            {
                overlap++;
            }
            else
                break;
        }
        key = key.substr(overlap);//get rid of the overlap
        if (p->m_leaf && key.size() == 0 && ((p->m_curr).size() == 0 || overlap == p->m_curr.size()))// if the word was found to be in the Tree
            break;
        if(key.size() == 0 || overlap != p->m_curr.size())// if traversal has reached a point where futher traversal is no appropriate
            return nullptr; // return nullptr
        p = p->m_next[key[0]];//else traverse the tree, following an edge
        key = key.substr(1);
    }
    if (p== nullptr)
        return nullptr;

    return p->m_val;//If and only if the loop is "broken" during tree traversal(indicating the word is present in the tree), will p != nullptr.
}

template <typename ValueType>
RadixTree<ValueType>::~RadixTree()
{
    cleanUp(m_head);//initiate a recursive function to rid of all the dynamically allocated ememory
}

template <typename ValueType>
void RadixTree<ValueType>::cleanUp(Node* a)
{
    if(a->m_val != nullptr)
        delete a->m_val;
    if (emptyChild(a))
    {
        delete a;
        return;
    }
    for(int i = 0; i < 128; i++)
    {
        if(a->m_next[i] != nullptr)
            cleanUp(a->m_next[i]);
    }
    delete a;
}

template <typename ValueType>
bool RadixTree<ValueType>::emptyNextAt(Node* a, int b)
{
    if (a-> m_next[b] != nullptr)
        return false;
    return true;
}

template <typename ValueType>
bool RadixTree<ValueType>::emptyChild(Node* a)
{
    for(int i = 0; i < 128; i++)
    {
        if(a->m_next[i] != nullptr)
            return false;
    }
    return true;
}


template <typename ValueType>
bool RadixTree<ValueType>::validEntry(const std::string& key) const
{
    if (key == "")
        return false;

    for(int i =0; i < key.size(); i++)
    {
        if (1 <= key[i] && key[i] <= 127)
            continue;
        else
            return false;
    }
    return true;
}
#endif /* RadixTree.h */
