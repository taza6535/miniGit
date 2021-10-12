#include <iostream>
#include <fstream>
#include "miniGit.hpp"
using namespace std;


//Traverses the singlynode list and finds a matching file name
singlyNode * miniGit::traverseSLLFindFile(string filename, doublyNode * node)
{
    singlyNode * crawler = node->head;
    while(crawler!=NULL)
    {
        if(crawler->fileName == filename)
        {
            return crawler;
        }
        crawler = crawler->next;
    }
    return NULL;
}

//Finding a specific commit number in the double linked list
doublyNode * miniGit::traverseDLLFindCommit(int commitNum)
{
    doublyNode * curr = head;
    while(curr!=NULL){
        if(curr->commitNumber == commitNum){
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

//Finds the last DLL
doublyNode * miniGit::traverseDLL(){
    doublyNode * curr = head;
    while(curr->next != NULL){
        curr = curr->next;
    }
    return curr;
}

//Inserts a new SLL node at the end of the SLL of the most recent double node
void miniGit::insertSLL(doublyNode * node, singlyNode * newSingle)
{
    if(node->head == NULL){
        node->head = newSingle;
        newSingle->next = NULL;
        return;
    }
    singlyNode * curr = node->head;
    while(curr->next != NULL)
    {
        curr = curr->next;
    }
    curr->next = newSingle;
    newSingle->next = NULL;
}
//inserts a DLL node at the end of the DLL
void miniGit::insertDLL(doublyNode * insert){
    doublyNode * curr = head;
    while(curr->next != NULL){
        curr = curr->next;
    }
    curr->next = insert;
    insert->previous = curr;
    insert->next = NULL;
    insert->head = NULL;
    int newCommitNum = curr->commitNumber + 1;
    insert->commitNumber = newCommitNum; 
}

//use when deleting an SLL node
void miniGit::deleteSLL(doublyNode * node, string filename)
{
    singlyNode * curr = node->head;
    singlyNode * prev = NULL;

    while(curr->next!=NULL && curr->fileName != filename)
    {
        prev = curr;
        curr = curr->next;  
    }
    if(curr->next == NULL && curr->fileName != filename){ //file does not exist in the list
        cout << "The file was not found." << endl;
        return;
    }
    else if(curr == node->head && curr->fileName == filename) //if you delete the head
    {
        node->head = node->head->next;
        delete curr;
    }
    else if(curr!= node->head && curr->fileName == filename)
    {
        prev->next = curr->next;
        delete curr;
    }
    cout << "Deleting the file..." << endl;
    cout << "The function was deleted successfully." << endl;
}

//prints the filenames of each node in the SLL
void miniGit::printSLL(doublyNode * node){
    
    singlyNode * curr = node->head;
    int count=1;
    while(curr!= NULL){
        cout << count << ". " << curr->fileName << endl;
        curr = curr->next;
        count++;
    }
}

//copies the file to minigit
void miniGit::copyFileToMinigit(string file1, string file2)
{
    ifstream src;
    src.open(file1); //original file from main directory
    ofstream dest;
    dest.open(file2); //file that we're writing to in the minigit
    string line;
    while(getline(src, line ))
    {
        dest << line; //copies first file into second file which is in the minigit
    }
    src.close();
    dest.close();
}

//Returns true if changes between f1 and f2 are detected, else false because there are no changes 
bool miniGit::isChanged(string f1, string f2)
{
    ifstream one;
    one.open(f1);
    string object1;
    string line = "";

    while(getline(one, line))
    {
        object1 = object1 + line;
    }

    one.close();
    ifstream two;
    two.open(f2);
    string line2 = "";
    string object2;

    while(getline(two, line2))
    {
        object2 = object2 + line2;
    }
    two.close();
    if(object1 == object2)
    {
        return false;
    }
    return true;
}
//for each file in the SLL, checks to see if it exists in .minigit and if it has been changed
void miniGit::traverseSLLAndCheckMinigit(doublyNode * node)
{

    singlyNode * curr = node->head;
    while(curr!=NULL)
    {
        string f1 = curr->fileName;
        ifstream fileIn;
        string f2 = curr->fileName + "_" + to_string(curr->versionNumber);
        string filename = ".minigit/" + f2;
        fileIn.open(filename);
        if(fileIn.fail())
        {
            copyFileToMinigit(f1, filename);
        } 
        else
        {
            bool changed = isChanged(f1, filename);
            if(changed)
            {
                curr->versionNumber++;
                string updatedFile = curr->fileName + "_" + to_string(curr->versionNumber);
                string newminigitfile= ".minigit/" + updatedFile;
                copyFileToMinigit(f1, newminigitfile);
                curr->fileVersion = updatedFile;
            } 
        }
        curr = curr->next;
    }
}
//copies the previous double node's linked list to the current one
void miniGit::copySLL(doublyNode * newDouble){
    if(newDouble->previous == NULL || newDouble->previous->head == NULL)
    {
        return;
    }
    singlyNode * curr = newDouble->previous->head;
    while(curr != NULL){
        singlyNode * copiedSingle = new singlyNode;
        copiedSingle->versionNumber = curr->versionNumber;
        copiedSingle->fileName = curr->fileName;
        copiedSingle->fileVersion = curr->fileVersion;
        insertSLL(newDouble, copiedSingle);             
        curr = curr->next;
        copiedSingle = NULL;
    }
}
//traverses through the singly linked list of the doubly node, if the file versions are different it overwrites the current local file with the version from that commit
void miniGit::traverseSLLForCheckout(doublyNode * node)
{
    singlyNode * curr = node->head;
    while(curr != NULL){
        string file = curr->fileVersion;
        string openfile = ".minigit/" + file;
        ifstream infile;
        ofstream outfile;
        bool changed = isChanged(curr->fileName, openfile);
        if(changed)
        {
            outfile.open(curr->fileName);
            infile.open(openfile);
            string line;
            while(getline(infile, line))
            {
                outfile << line << endl;
            }
        }
        
        curr = curr->next;
    }
}