#include<iostream>
using namespace std;

struct singlyNode
{
    int versionNumber;
    string fileName;
    string fileVersion;
    singlyNode * next;
};

struct doublyNode{
    int commitNumber;
    singlyNode * head;
    doublyNode * previous;
    doublyNode * next;
};

class miniGit
{
    public:
    
    doublyNode * head;
    singlyNode * traverseSLLFindFile(string filename, doublyNode * node);
    doublyNode * traverseDLLFindCommit(int commitNum);
    doublyNode * traverseDLL();
    void insertSLL(doublyNode * node, singlyNode * insert);
    void insertDLL(doublyNode * insert);
    void deleteSLL(doublyNode * node, string filename);
    void printSLL(doublyNode * node);
    void copyFileToMinigit(string file1, string file2);
    bool isChanged(string f1, string f2);
    void traverseSLLAndCheckMinigit(doublyNode * node);
    void copySLL(doublyNode * insert);
    void traverseSLLForCheckout(doublyNode * node);
};
