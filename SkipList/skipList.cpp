#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

class Node{
public:
    int data;
    vector<Node*> nextElements;
    Node(int value, int level):data(value), nextElements(level+1, nullptr){}
};

class SkipList{
    int maxLevel;
    int Level;
    Node* head;
public:
    explicit SkipList(int mxLevel):maxLevel(mxLevel){
        head = new Node(0, maxLevel);
        srand(time(nullptr));
        Level = 0;
    }
    ~SkipList() {
        Node* current = head->nextElements[0];
        while (current) {
            Node* next = current->nextElements[0];
            delete current;
            current = next;
        }

        delete head;
    }
    int getRandomNumber(){
        double p = 0.5;  // Probability of promoting to next level
        int lvl = 0;

        // Use a logarithmic-like distribution
        while ((rand() / static_cast<double>(RAND_MAX)) < pow(p, lvl + 1) && lvl < maxLevel) {
            lvl++;
        }

        return lvl;

    }
    void insert(int value){
        //randomly tossing a coin to determine the level of the new node
        int lvl = getRandomNumber();
        if(lvl > Level) {
            for(int i = Level + 1; i <= lvl; i++){
                head->nextElements.push_back(nullptr);
            }
            Level = lvl;
        }

        //Keeping track of position of the new node to be added
        Node* current = head;
        vector<Node*> before(Level+1, nullptr);
        for(int i = Level; i >=0 ;i--){
            while(current->nextElements[i] != nullptr && current->nextElements[i]->data < value){
                current = current->nextElements[i];
            }
            before[i] = current;
        }

        //Actually inserting the node
        current = current->nextElements[0];
        if(current == nullptr || current->data != value){
            Node* newNode = new Node(value, lvl);
            for(int i = 0 ; i <= lvl; i++){
                newNode->nextElements[i] = before[i]->nextElements[i];
                before[i]->nextElements[i] = newNode;
            }
        }
        else cout<<"The number Provided already Exists\n";
    }

    void remove(int value){
        //Getting the position of the element required to be deleted
        Node* current = head;
        vector<Node*> before(Level+1, nullptr);
        for(int i = Level; i >= 0; i--){
            while(current->nextElements[i] != nullptr && current->nextElements[i]->data < value){
                current = current->nextElements[i];
            }
            before[i] = current;
        }

        //Deleting the node
        current = current->nextElements[0];
        if(current != nullptr && current->data == value){
            for(int i = 0; i <= Level; i++){
                if(before[i]->nextElements[i]->data != value){
                    break;
                }
                before[i]->nextElements[i] = current->nextElements[i];
            }
            delete current;
            //updating Level
            while(Level > 0 && head->nextElements[Level] == nullptr){
                head->nextElements.pop_back();
                Level--;
            }
        }
        else cout<<"The element is not in the List\n";
    }

    bool search(int val){
        Node* currentNode = head;
        for(int i = Level; i >= 0; i--){
            while(currentNode->nextElements[i] != nullptr && currentNode->nextElements[i]->data < val){
                currentNode = currentNode->nextElements[i];
            }
        }
        currentNode = currentNode->nextElements[0];
        return (currentNode && currentNode->data == val);
    }

    void print(){
        cout<<"THE WHOLE LIST:\n";
        for(int i = Level; i >= 0; i--){
            Node* currentNode = head;
            cout<< "Level "<<i<<": ";
            while(currentNode->nextElements[i] != nullptr){
                currentNode = currentNode->nextElements[i];
                cout<<currentNode->data<<" ";
            }
            cout<<"\n";
        }
    }
};

int main() {
//    SkipList list(5);
//    list.insert(10);
//    list.insert(15);
//    list.insert(2);
//    list.insert(7);
//    cout<<boolalpha<<list.search(5)<<endl;
//    cout<<boolalpha<<list.search(7)<<endl;
//    list.print();
//    list.remove(7);
//    list.remove(5);
//    list.print();
    cout << "Please Enter the Max number of levels of the skipList: ";
    int maxNumberOfLevels ;cin>>maxNumberOfLevels;
    SkipList list(maxNumberOfLevels);
    while(true){
        cout << "1 - Insert an element\n"
                "2 - Remove an element\n"
                "3 - Search for an element\n"
                "4 - Print the whole SkipList\n"
                "5 - End the Program\n"
                "Enter the number of the process you want to execute: ";
        int process;
        cin >> process;
        switch (process) {
            case 1:
                cout<<"Enter the number you want to add: ";
                int numberToAdd ; cin >> numberToAdd;
                list.insert(numberToAdd);
                break;
            case 2:
                cout<<"Enter the number you want to remove: ";
                int numberToDelete ; cin >> numberToDelete;
                list.remove(numberToDelete);
                break;
            case 3:
                cout<<"Enter the element you want to Search For: ";
                int numberToSearch;cin >> numberToSearch;
                cout<<"The element "<< numberToSearch <<" is ";
                if(list.search(numberToSearch))
                    cout<<"FOUND\n";
                else
                    cout<<"NOT FOUND\n";
                break;
            case 4:
                list.print();
                break;
            case 5:
                return 0;
            default:
                cout<<"Please Enter a correct number\n";

        }
    }
}
