#include <bits/stdc++.h>
using namespace std;

#define ALPHABET_SIZE 26

// trie node
struct TrieNode
{
    struct TrieNode *Dothraki[ALPHABET_SIZE];
    string English;
    // isEndOfWord is true if the node represents a english value
    bool isEndOfWord;
};

// Returns new trie node which initialized to NULLs
struct TrieNode *getNode(void)
{
    struct TrieNode *pNode = new TrieNode;
    pNode->English = '\0';
    pNode->isEndOfWord = false;

    for (int i = 0; i < ALPHABET_SIZE; i++)
        pNode->Dothraki[i] = NULL;

    return pNode;
}

// inserts to tree with key and value parameters
void insert(struct TrieNode *root, string key, string value, ofstream &fw)
{
    struct TrieNode *iter = root;
    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] - 'a';
        if (!iter->Dothraki[index])
            iter->Dothraki[index] = getNode();

        iter = iter->Dothraki[index];
    }

    if (iter->English == value)
        fw << "\"" << key << "\"" << " already exist\n";
    else if (iter->English.length() > 1)
    {
        iter->English = value;
        fw << "\"" << key << "\"" << " was updated\n";
    }
    else
    {
        iter->English = value;
        fw << "\"" << key << "\"" << " was added\n";
    }

    // mark last letter
    iter->isEndOfWord = true;
}

// search tree for key parameter
void search(struct TrieNode *root, string key, ofstream &fw)
{
    struct TrieNode *iter = root;       // for iterations
    int i, index = key[0] - 'a';
    if (!iter->Dothraki[index])     // checking first node if it has a record
    {
        fw << "\"no record\"\n";
        return;
    }
    iter = iter->Dothraki[index];
    for ( i = 1; i < key.length(); i++)
    {
        index = key[i] - 'a';
        if (!iter->Dothraki[index])     // key path doesn't exits
        {
            fw << "\"incorrect Dothraki word\"\n";
            return;
        }
        iter = iter->Dothraki[index];
    }

    if (!(iter->isEndOfWord))       // if key doesn't have english meaning
        fw << "\"not enough Dothraki word\"\n";

    else
    {
        // exits
        fw << "\"The English equivalent is " << iter->English << "\"" << endl;
    }
}

// delete from tree with key parameter
void Delete(struct TrieNode *root, string key, ofstream &fw)
{
    struct TrieNode *iter = root;       // for iterations
    struct TrieNode *del;               // if we need to delete we save what we will delete in del
    int j, i, Count, Count2=-1, index = key[0] - 'a';
    if (!iter->Dothraki[index])     // checking first node if it has a record
    {
        fw << "\"no record\"\n";
        return;
    }
    iter = iter->Dothraki[index];
    for ( i = 1; i < key.length(); i++)
    {
        // *****
        // process for del
        Count = 0;
        for ( j = 0; j < ALPHABET_SIZE; j++)
            if (iter->Dothraki[j])
                Count ++;

        if (Count > 1 && i != key.length()-1)
        {
            Count2 = i;
            del = iter;
        }
        // *****
        index = key[i] - 'a';
        if (!iter->Dothraki[index])     // key path doesn't exits
        {
            fw << "\"incorrect Dothraki word\"\n";
            return;
        }
        iter = iter->Dothraki[index];
    }

    if (!(iter->isEndOfWord))       // if key doesn't have english meaning
        fw << "\"not enough Dothraki word\"\n";

    else
    {
        // deletion part
        if (Count2 != -1)
            del->Dothraki[key[Count2] - 'a'] = NULL;

        iter->isEndOfWord = false;
        fw << "\"" << key << "\"" << " deletion is successful\n";
    }
}

// list tree as asked
// this function is recursive and with preorder traversal
void liste(struct TrieNode* node, string word, ofstream &fw, int tab)
{
    int i, Count=0;
    vector<int> indexs;
    for ( i = 0; i < ALPHABET_SIZE; i++)
        if (node->Dothraki[i])
        {
            indexs.push_back(i);
            Count ++;
        }

    if (Count > 1)
    {
        for ( i = 0; i < tab; i++)
            fw << "\t";

        fw << "-" << word;
        if (node->isEndOfWord)
            fw << "(" <<node->English << ")";
        fw << endl;
        tab ++;
    }

    else if (node->isEndOfWord)
    {
        for ( i = 0; i < tab; i++)
            fw << "\t";

        fw << "-" << word << "(" <<node->English << ")" << endl;
    }

    vector<string> words;
    for ( i = 0; i < indexs.size(); i++)
        words.push_back(word);

    for ( i = 0; i < indexs.size(); i++)
    {
        words[i].push_back(indexs[i]+'a');
        liste(node->Dothraki[indexs[i]], words[i], fw, tab);
    }

}

// driver
int main(int argc, char** argv)
{
    vector<string> all_list;
    char line[50];
    string input_file, output_file, key, value, li;

    // command line parameters
    input_file = argv[1];
    output_file = argv[2];

    struct TrieNode *root = getNode();    // init

    // read line by line and send for process
    ifstream f(input_file);
    ofstream fw(output_file);
    while (f.getline(line, 50))
    {
        string li(line);
        if (li[0] == 'i')
        {
            li = li.substr(7,li.length());
            li.erase(remove(li.begin(), li.end(), '\r'), li.end());
            li.erase(remove(li.begin(), li.end(), ')'), li.end());
            stringstream s(li);
            vector<string> v;
            while (s.good())
            {
                string substr;
                getline(s, substr, ',');
                v.push_back(substr);
            }
            key = v[0];
            value = v[1];
            insert(root, key, value, fw);
        }
        else if(li[0] == 's')
        {
            li = li.substr(7,li.length());
            li.erase(remove(li.begin(), li.end(), '\r'), li.end());
            li.erase(remove(li.begin(), li.end(), ')'), li.end());
            key = li;
            search(root, key, fw);
        }
        else if(li[0] == 'd')
        {
            li = li.substr(7,li.length());
            li.erase(remove(li.begin(), li.end(), '\r'), li.end());
            li.erase(remove(li.begin(), li.end(), ')'), li.end());
            key = li;
            Delete(root, key, fw);
        }
        else
        {
            string word;
            int i;
            for ( i = 0; i < ALPHABET_SIZE; i++)
                if (root->Dothraki[i])
                {
                    word = i+'a';
                    liste(root->Dothraki[i], word, fw, 0);
                }
        }
    }

    cout << "\nProgram successed !\n";

    return 0;
}