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
            cout << "player is not Found\n";
            return;
        }

        PlayerNode* playerNode = it->second;
        Player* player = playerNode->playerData;

        removePlayer(name);
        player->score += scoreChange;
        addPlayer(player->name, player->score);
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
                cout<<current->playerData->score<<" ";
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
    playerSkipList gameLeaderboard(5);

    while (true) {
        cout << "\n--- Multiplayer Game Management ---\n"
             << "1 - Add a Player\n"
             << "2 - Remove a Player\n"
             << "3 - Update Player Score\n"
             << "4 - Get Player Score\n"
             << "5 - Get Top N Players\n"
             << "6 - Print All Players\n"
             << "7 - Exit\n"
             << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                string name;
                int initialScore;
                cout << "Enter player name: ";
                cin >> name;
                cout << "Enter initial score: ";
                cin >> initialScore;
                gameLeaderboard.addPlayer(name, initialScore);
                break;
            }
            case 2: {
                string name;
                cout << "Enter player name to remove: ";
                cin >> name;
                gameLeaderboard.removePlayer(name);
                break;
            }
            case 3: {
                string name;
                int scoreChange;
                cout << "Enter player name: ";
                cin >> name;
                cout << "Enter score change (positive or negative): ";
                cin >> scoreChange;
                gameLeaderboard.updatePlayerScore(name, scoreChange);
                break;
            }
            case 4: {
                string name;
                cout << "Enter player name to check score: ";
                cin >> name;
                int score = gameLeaderboard.printPlayerScore(name);
                if (score != -1) {
                    cout << name << "'s score: " << score << endl;
                }
                break;
            }
            case 5: {
                int n;
                cout << "Enter number of top players to retrieve: ";
                cin >> n;
                vector<Player*> topPlayers = gameLeaderboard.getTopKElements(n);
                cout << "Top " << n << " Players:\n";
                for (auto player : topPlayers) {
                    cout << player->name << ": " << player->score << endl;
                }
                break;
            }
            case 6: {
                gameLeaderboard.printPlayers();
                break;
            }
            case 7: {
                cout << "Exiting...\n";
                return 0;
            }
            default: {
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }
}
