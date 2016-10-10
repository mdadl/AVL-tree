// Original Author: Bhavana Lanjewar (2013)
// Update by: Mohd Adil Bin Mat Ti @ Mokti (2016)
// AVL Tree Project 1

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class Dictionary
{
    string      m_word;
    string      m_meaning;
    Dictionary *m_left; 
    Dictionary *m_right;
    int         m_count;

public:
    Dictionary()
    {
        m_left = m_right = NULL;
        m_count = 0;
    }

    const string& GetMeaning() { return m_meaning; }
    void DumpToFile(string& fileName);
    void DumpToFile(ofstream& fileStream);
    Dictionary* Zap();
    Dictionary* Search(string& word);
    Dictionary* Insert(string&, string&);
    Dictionary* Delete(string&);
    Dictionary* RR();
    Dictionary* LL();
    Dictionary* LR();
    Dictionary* RL();

     int Height();
    void PreOrder();
    void InOrder();

     Dictionary* RotateRight();
    Dictionary* RotateLeft();

     int BF();
};

Dictionary* CreateDictionaryFromFile(string &fileName)
{
    Dictionary* root = NULL;

    ifstream myFile(fileName.c_str());

    if (!myFile.is_open()) {
        cerr << "error: cant open file " << fileName << endl;
        return root;
    }

    root = new Dictionary; 
    while (myFile.good()) {
        string line;
        getline (myFile, line);

        if (line[0] == '#') 
            continue;

        size_t pos;
        pos = line.find(" ");
        pos = (pos != std::string::npos)? pos+1: 0;
        if (pos == 0) 
            continue;

        string word, meaning;
        word = line.substr(0, pos-1);
        meaning = line.substr (pos);

        root = root->Insert(word, meaning);
    }
    myFile.close();

    return root;
}

Dictionary* Dictionary::Insert(string& word, string& meaning)
{
    Dictionary *newRoot = this;

    if (m_count == 0) {
        m_word = word;
        m_meaning = meaning;
        m_count = 1;
    }

    if (word > m_word) {
        if (m_right == NULL) {
            m_right = new Dictionary;
        }
        m_right = m_right->Insert(word, meaning);

         if (BF() <= -2) {
            newRoot = (word > m_right->m_word)? RR(): RL();
        }
    }
    else if (word < m_word) {
        if (m_left == NULL) {
            m_left = new Dictionary;
        }

        m_left = m_left->Insert(word, meaning);
        if (BF() == 2) {
            newRoot = (word < m_left->m_word)? LL(): LR(); 
        }
    }

     return newRoot;
}

// Delete(): 
Dictionary* Dictionary::Delete(string& word)
{
    Dictionary *newRoot = this;

    if (word == m_word) {
        // We are on the node containing the word
        // If this is the leaf node, then return NULL as the new Root
        if (!m_right && !m_left) 
            newRoot = NULL;

        else if (!m_right) {
            Dictionary* t = m_left; 

            m_right = t->m_left;
            m_left = t->m_left;
            m_word = t->m_word;
            m_meaning = t->m_meaning;

            delete t;
        }
        else if (!m_left) {
            Dictionary* t = m_right; 

            m_right = t->m_left;
            m_left = t->m_left;
            m_word = t->m_word;
            m_meaning = t->m_meaning;

            delete t;
        }
        else {
            Dictionary* t;
    
            // Overwrite the current node with leaf node in the right subtree
            for (t = m_right; t->m_left; t = t->m_left);
            m_word = t->m_word;
            m_meaning = t->m_meaning;

            // Delete the leaf from the subtree
            t = m_right->Delete(word);
            if (t == NULL) 
                delete m_right;

            m_right = t;

             if (BF() >= 2)
                newRoot = (m_left->BF() >= 0)? LL(): LR();
        }
    }
    else if (m_right && word > m_word) {
        Dictionary *t;

         t = m_right->Delete(word);
        if (t == NULL)
            delete m_right;

        m_right = t;

         if (BF() >= 2) 
             newRoot = (m_left->BF() >= 0)? LL(): LR();
    }
    else if (m_left && word < m_word) {
        Dictionary *t;

         t = m_left->Delete(word);
        if (t == NULL)
             delete m_left;

        m_left = t;

         if (BF() <= -2) 
             newRoot = (m_right->BF() <= 0)? RR(): RL();
    }

     return newRoot;
}

Dictionary* Dictionary::LL()
{
    return RotateRight();
}

Dictionary* Dictionary::RR()
{
    return RotateLeft();
}

Dictionary* Dictionary::LR()
{
    if (m_left) {
         m_left = m_left->RotateLeft();
    }
    return RotateRight();
}

Dictionary* Dictionary::RL()
{
    if (m_right) {
         m_right = m_right->RotateRight();
    }
    return RotateLeft();
}

Dictionary* Dictionary::RotateRight()
{
    Dictionary *newRoot = NULL;

     newRoot = m_left;
    m_left = newRoot->m_right;
    newRoot->m_right = this;

     return newRoot;
}

Dictionary* Dictionary::RotateLeft()
{
    Dictionary *newRoot = NULL;

     newRoot = m_right;
    m_right = newRoot->m_left;
    newRoot->m_left = this;

     return newRoot;
}

int Dictionary::BF()
{
    int lh, rh;

    lh = (m_left)? m_left->Height(): 0;
    rh = (m_right)? m_right->Height(): 0;

    return (lh-rh);
}

void Dictionary::PreOrder()
{
    if (m_count) { 
        cout << m_word << ",";
        if (m_left) m_left->PreOrder();
        if (m_right) m_right->PreOrder();
    }
}

void Dictionary::DumpToFile(string& fileName)
{
    ofstream fileStream(fileName.c_str());

    if (!fileStream.is_open()) { 
        cerr << "error: cant open file " << fileName << endl;
        return;
    }

    DumpToFile(fileStream);
}

Dictionary* Dictionary::Zap()
{
    if (m_left) { 
        m_left->Zap();
        delete m_left; 
        m_left = NULL; 
    }
    if (m_right) { 
        m_right->Zap();
        delete m_right;
        m_right = NULL;
    }
    m_word = "";
    m_meaning = "";
    m_count = 0;
}

Dictionary* Dictionary::Search(string& word)
{
    if (m_count && word == m_word) 
        return this;

    if (word > m_word)
        return (m_right)? m_right->Search(word): NULL;
    else 
        return (m_left)? m_left->Search(word): NULL;
}

void Dictionary::DumpToFile(ofstream& fileStream)
{
    if (m_count) { 
        if (m_left) 
            m_left->DumpToFile(fileStream);
        fileStream << m_word << " " << m_meaning << endl;
        if (m_right) 
            m_right->DumpToFile(fileStream);
    }
}

void Dictionary::InOrder()
{
    if (m_count) { 
        if (m_left) 
            m_left->InOrder();
        cout << m_word << ", ";
        if (m_right) 
            m_right->InOrder();
    }
}

int Dictionary::Height()
{
    int lh, rh;

    if (m_count == 0) 
        return 0;

     lh = (m_left)? m_left->Height(): 0;
    rh = (m_right)? m_right->Height(): 0;
    
    return (lh > rh)? lh+1: rh+1;
}

int main()
{
    Dictionary *root = NULL;
    
    string fileName = "data.txt";
    root = CreateDictionaryFromFile(fileName);
	
	int ch = 0;

     do {
     	cout << "AVL Tree Project 1 By Mohd Adil Bin Mat Ti @ Mokti (GP04686)" << endl;
		cout << "Data loaded from file: " << fileName << endl << endl;
     	
        cout << " 1. Insert new Word" << endl
             << " 2. Search the Word" << endl
             << " 3. Delete the Word" << endl
             << " 4. Display Word list in ascending order" << endl
             << " 5. Save Word to file" << endl
             << " 0. Exit" << endl;
        cout << "\nEnter your choice: ";
        cin >> ch;

         switch (ch) {
             case 1: {
                    if (root == NULL) { 
                        cout << "error: No dictionary created yet" << endl;
                        break;
                    }
                    
                    string word, meaning; 

                    cin.ignore();
					cout << endl << "Enter the word: ";
                    getline(cin, word);
                    cout << "Enter the meaning: ";
                    getline(cin, meaning);

                    root = root->Insert(word, meaning);
                    
                    cout << endl << "Data inserted successfully. Press ENTER to continue...." << endl;
                    cin.get();
                    system("CLS");
                    
                    }
                    break;

            case 2: { 
                    string word;
                    Dictionary* node = NULL;
                
                    cout << "Enter the word to search: "; 
                    cin  >> word;
                    
                    node = root->Search(word);
                    if (node) { 
                        cout << "The meaning of word " << word << " is " 
                             << node->GetMeaning() << endl;
                    }
                    else {
                        cout << "The word " << word 
                             << " not found in the dictionary." << endl;
                    }
                    cout << endl << "Press ENTER to continue...." << endl;
                    cin.ignore();
                    cin.get();
                    system("CLS");
                    }
                    break;

             case 3: {
                    string word;
                    Dictionary* node = NULL;

                    cout << "\n Enter the word you want to delete: ";
                    cin >> word;

                     node = root->Search(word);
                     if(node) {
                     	root = root->Delete(word);
                     	cout << endl << "Data deleted successfully. ";
					 }
                     else {
                     	cout << "The word " << word 
                             << " not found in the dictionary. ";
					 }
                     cout << "Press ENTER to continue...." << endl;
                     cin.ignore();
                     cin.get();
                     system("CLS");
                    }
                    break;

			 case 4: {
                    if (!root) { 
                        cout << "Dictionary not yet created." << endl; 
                        break;
                    }
                    cout << endl << "List of Word in Ascending Order: ";
                    root->InOrder();
                    cout << endl << endl;
                    cout << "Press ENTER to continue...." << endl;
                    cin.ignore();
                    cin.get();
                    system("CLS");
                    }
                    break;

			 case 5: { 
                    if (!root) { 
                        cout << "Dictionary not yet created" << endl; 
                        break;
                    }

                    string fileName;
                    cout << "Enter the file Name: ";
                    cin  >> fileName; 

                    root->DumpToFile(fileName);
                    
                    cout << endl << "Data saved successfully. Press ENTER to continue...." << endl;
                    cin.get();
                    system("CLS");
                    }
                    break;

             default: break;
            }
      } while (ch != 0);
      //return 0;
}
