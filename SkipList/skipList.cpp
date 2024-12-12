#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <unordered_map>
using namespace std;

class Node{
public:
    int data;
    vector<Node*> nextElements;
    Node(int value, int level):data(value), nextElements(level+1, nullptr){}
    Node& operator=(const Node& other) {
        if (this == &other) {
            return *this;  // Self-assignment check
        }

        // Copy data
        data = other.data;

        // Copy the nextElements vector
        nextElements = other.nextElements;

        return *this;
    }

    virtual ~Node(){}
};

class SkipList{
protected:
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

class Player{
public:
    int score;
    string name;
    explicit Player(string playerName,int initialScore = 0):name(playerName), score(initialScore){}
};

class PlayerNode:public Node{
public:
    Player* playerData;
    PlayerNode(Player* player, int level): Node(player->score, level), playerData(player){}
    PlayerNode& operator=(const PlayerNode& other) {
        if (this == &other) {
            return *this;  // Self-assignment check
        }

        // Call the base class assignment operator
        Node::operator=(other);

        // Copy playerData (deep copy)
        if (playerData) {
            delete playerData;  // Clean up existing playerData
        }
        playerData = new Player(*other.playerData);  // Deep copy of Player object

        return *this;
    }
};

class playerSkipList:public SkipList{
private:
    unordered_map<string, PlayerNode*> playerMap;
public:
    void addPlayer(string playerName, int initialScore = 0){
        if(playerMap.find(playerName) != playerMap.end()){
            cout<<"Player Already Exist\n";
        }
        else{
            Player* newPlayer = new Player(playerName, initialScore);
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
                while(current->nextElements[i] != nullptr && current->nextElements[i]->data < newPlayer->score){
                    current = current->nextElements[i];
                }
                before[i] = current;
            }

            //Actually inserting the node
            current = current->nextElements[0];
            PlayerNode* newPlayerNode = new PlayerNode(newPlayer, lvl);
            for(int i = 0 ; i <= lvl; i++){
                newPlayerNode->nextElements[i] = before[i]->nextElements[i];
                before[i]->nextElements[i] = newPlayerNode;
            }
            playerMap[playerName] = newPlayerNode;
        }
    }

    void removePlayer(string playerName){
        auto playerIterator = playerMap.find(playerName);
        if(playerIterator == playerMap.end()){
            cout<<"Player is Not in List\n";
            return ;
        }
        PlayerNode *playerNode = playerIterator->second;
        Player *player = playerNode->playerData;
        int playerScore = player->score;
        //Getting the position of the element required to be deleted
        Node* current = head;
        vector<Node*> before(Level+1, nullptr);
        for(int i = Level; i >= 0; i--){
            while(current->nextElements[i] != nullptr && current->nextElements[i]->data < playerScore){
                current = current->nextElements[i];
            }
            before[i] = current;
        }
        current = before[0]->nextElements[0];
        if (current && current->data == playerScore) {
            for (int i = 0; i <= Level; i++) {
                if (before[i]->nextElements[i] == current) {
                    before[i]->nextElements[i] = current->nextElements[i];
                }
            }
        }
        //updating Level
        while(Level > 0 && head->nextElements[Level] == nullptr){
            head->nextElements.pop_back();
            Level--;
        }
        playerMap.erase(playerName);
        delete playerNode;
        delete player;
    }
    void updatePlayerScore(string name, int scoreChange) {
        auto it = playerMap.find(name);
        if (it == playerMap.end()) {
            cout << "Player is not Found\n";
            return;
        }
        Player* player = new Player(*it->second->playerData);
        player->score += scoreChange;

        removePlayer(name);
        addPlayer(player->name, player->score);

        delete player;
    }
    vector<Player*> getTopKElements(int k){
        vector<Player*> topKPlayers;
        PlayerNode* current = dynamic_cast<PlayerNode*>(head->nextElements[0]);
        while(current){
            topKPlayers.push_back(current->playerData);
            current = dynamic_cast<PlayerNode*>(current->nextElements[0]);
        }
        sort(topKPlayers.begin(),topKPlayers.end(),[](const Player *a, const Player *b){
            return a->score > b->score;
        });
        return vector<Player*> (topKPlayers.begin(), topKPlayers.begin()+min(k,static_cast<int>(topKPlayers.size())));
    }

    int printPlayerScore(string name){
        auto playerIterator = playerMap.find(name);
        if(playerIterator == playerMap.end()){
            cout<<"Player is not in the list\n";
            return -1;
        }
        return playerIterator->second->playerData->score;
    }

    void printPlayers(){
        for(int i = Level; i >= 0 ;i--){
            PlayerNode* current = dynamic_cast<PlayerNode*>(head->nextElements[i]);
            cout<<"Level "<< i<<": ";
            while(current){
                cout<<"("<<current->playerData->name <<": "<< current->playerData->score<<") ";
                current = dynamic_cast<PlayerNode*>(current->nextElements[i]);
            }
            cout<<endl;
        }
    }

    playerSkipList(int i) : SkipList(i) {
        maxLevel = i;
    }
};

int main() {
    cout<<"1- Normal SkipList\n";
    cout<<"2- Game\n";
    cout<<"Please choose the number of the SkipList you want to try: ";
    int skipListType, maxLevel;cin>>skipListType;
    skipListType--;
    cout<<"Enter the max Level: ";cin>>maxLevel;
    if(skipListType == 0){
        int op;
        SkipList skipList(maxLevel);
        while(true){
            cout<<"1- Insert New Element\n";
            cout<<"2- Delete an Element\n";
            cout<<"3- Search for an Element\n";
            cout<<"4- Print the Whole List\n";
            cout<<"5- Exit\n";
            cout<<"Enter the number of the function you want to do: ";
            cin>>op;
            if(op == 1){
                cout<<"Enter the element you wan0t to add: ";
                int val;cin>>val;
                skipList.insert(val);
                cout<<"The element "<<val<<" is inserted\n";
            }
            else if(op == 2){
                cout<<"Enter the element you want to delete: ";
                int val;cin>>val;
                skipList.remove(val);
                cout<<"The element "<<val<<" is Removed\n";
            }
            else if(op == 3){
                cout<<"Enter the element you want to search for: ";
                int val;cin>>val;
                bool isFound = skipList.search(val);
                cout<<"The element "<<val<<(isFound ? " is Found\n":"Is Not Found\n");
            }
            else if(op == 4){
                skipList.print();
            }
            else if(op == 5){
                cout<<"Exiting...";
                break;
            }
            else{
                cout<<"Please enter a valid choice\n";
            }
        }
    }
    else if(skipListType == 1){
        int op;
        playerSkipList playerManger(maxLevel);
        while(true){
            cout<<"1- Add New Player\n";
            cout<<"2- Remove a Player\n";
            cout<<"3- Get a player's score\n";
            cout<<"4- Update a player's score\n";
            cout<<"5- Get Top Players\n";
            cout<<"6- Print the Whole Scoreboard\n";
            cout<<"7- Exit\n";
            cout<<"Enter the number of the function you want to do: ";
            cin>>op;
            if(op == 1){
                cout<<"Enter the Player Name: ";
                string name;cin>>name;
                cout<<"Enter the Player InitialScore: ";
                int score;cin>>score;
                playerManger.addPlayer(name,score);
                cout<<"The Player "<<name<<" is inserted with score "<<score<<"\n";
            }
            else if(op == 2){
                cout<<"Enter the name of the player you want to delete: ";
                string name;cin>>name;
                playerManger.removePlayer(name);
                cout<<"The Player "<<name<<" is Removed\n";
            }
            else if(op == 3){
                cout<<"Enter the name of the Player: ";
                string name;cin>>name;
                int score = playerManger.printPlayerScore(name);
                cout<<"The Player "<<name<<"'s score is "<<score <<"\n";
            }
            else if(op == 4){
                cout<<"Enter the name of the Player: ";
                string name;cin>>name;
                cout<<"Enter the value u want to add or subtract from his score(Positive or Negative): ";
                int increment;cin>>increment;
                playerManger.updatePlayerScore(name, increment);
                cout<<"The Player "<<name<<"'s score after Updating is "<<playerManger.printPlayerScore(name)<<"\n";
            }
            else if(op == 5){
                cout<<"Enter the Number of the top player's you want to have: ";
                int k;cin>>k;
                vector<Player*> topKElements = playerManger.getTopKElements(k);
                for(auto player: topKElements){
                    cout<<player->name<<": "<<player->score<<endl;
                }
            }
            else if(op == 6){
                playerManger.printPlayers();
            }
            else if(op == 7){
                cout<<"Exiting...";
                break;
            }
            else{
                cout<<"Please enter a valid Choice\n";
            }
        }
    }
    else{
        cout<<"NOT A VALID CHOICE\n";

    }
    return 0;
}
