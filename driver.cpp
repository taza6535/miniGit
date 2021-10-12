#include <iostream>
#include "miniGit.cpp"
#include <fstream>

using namespace std;

#include <filesystem>
namespace fs = filesystem;

void printRepository(doublyNode * start)
{
    doublyNode * curr = start;
    while(curr!=NULL)
    {
        cout << curr->commitNumber << "-->";
        singlyNode* node = curr->head;
        while(node!=NULL)
        {
            cout << node->fileName << "-->";
            node = node->next;
        }
        curr = curr->next;
        cout << endl;
    }
}

void displayMenu()
{
    cout << "======Menu=====" << endl;
    cout << "1. Add a file" << endl;
    cout << "2. Remove file" << endl;
    cout << "3. Commit" << endl;
    cout << "4. Checkout" << endl;
    cout << "5. Quit" << endl;
}          

int main(){
    cout << "Do you want to initialize a new repository? " << endl;
    cout << "1. Yes" << endl;
    cout << "2. No" << endl;
    int yesOrNo;
    cin >> yesOrNo;
    if(yesOrNo == 2){
        return 0;
    }
    else if(yesOrNo >2 || yesOrNo < 1)
    {
        cout << "Invalid option.";
        return 0;
    }
    fs::remove_all(".minigit");
    fs::create_directory(".minigit"); 
    miniGit repo;
    doublyNode * start = new doublyNode; //creating the first node of the doubly linked list
    start->commitNumber = 0;
    start->previous = NULL;
    start->next = NULL;
    start->head = NULL;
    repo.head = start;
    displayMenu();

   int num;
   cin >> num;
    while (num != 5){
    switch(num){
        case 1:
        {
            string filename;
            cout << "Enter a file name:" << endl;
            cin >> filename;
            ifstream infile;
            infile.open(filename);
            while(infile.fail())
            {
                cout << "That file could not be opened. Enter another file name:" << endl; //prompt user to enter valid filename
                cin >> filename;
                infile.open(filename);
            }
            doublyNode * lastNode = repo.traverseDLL();
            if(repo.traverseSLLFindFile(filename,lastNode) != NULL){
                cout << "The file already exists!" << endl;
                displayMenu();
                cin >> num;
                break;
            } else {
                cout << "Adding file..." << endl;
            }

            singlyNode * newfile = new singlyNode;
            newfile->fileName = filename;
            newfile->fileVersion = filename + "_" + to_string(newfile->versionNumber); //will the filenames print f1.txt01?
            newfile->next = NULL;
            doublyNode * result = repo.traverseDLL(); //find last DLL node
            repo.insertSLL(result, newfile); //inserting newfile into SLL at last DLL node

            cout << "Added successfully!" << endl;
            displayMenu();
            cin>>num;
            break;
        }
        break;
        case 2: 
        {
            string filename;
            cout << "Enter a file name:" << endl;
            cin >> filename;
            doublyNode * result = repo.traverseDLL();
            while(repo.traverseSLLFindFile(filename, result) == NULL){
                cout << "The file does not exist. Enter another file name." << endl;
                cin >> filename;
            }
            repo.deleteSLL(result, filename);
            displayMenu();
            cin >> num;
            break;
        } 
        break;
        case 3:
        {
            doublyNode * lastDouble = repo.traverseDLL(); //find the last DLL node
            repo.traverseSLLAndCheckMinigit(lastDouble); //check to see if files exist in minigit and checks to see if they have been changed
            doublyNode * newDouble = new doublyNode; //create a new DLL node to insert
            repo.insertDLL(newDouble); //insert new node
            repo.copySLL(newDouble); //copies the previous node's linked list to the new node
            cout << "Commit successful" << endl;
            displayMenu();
            cin >> num;
            break;

        }
        break;
        case 4:
        {
            int commit;
            cout << "Enter the version number you want to checkout:" << endl;
            cin >> commit;
            doublyNode * commitNode = repo.traverseDLLFindCommit(commit);
            if(commitNode==NULL)
            {
                cout << "This commit number does not exist" << endl;
                break;
            }
            cout << "The files for commit number " << commitNode->commitNumber << " are:" << endl;
            repo.printSLL(commitNode);
            cout << endl;
            cout << "Any uncommitted changes will be lost. Do you you want to continue?" << endl;
            cout << "1. No" << endl;
            cout << "2. Yes" << endl;
            int answer;
            cin>>answer;
            if(answer==1)
            {
                displayMenu();
                break;
            }
            repo.traverseSLLForCheckout(commitNode);
            cout << "Checkout successful" << endl;
            displayMenu();
            cin >> num;
            break;
        }
        default:
        {
            cout << "Invalid option. Please enter another option" << endl;
            displayMenu();
            cin >> num;
            break;            
        }
        break;
        
    } 
}
}
