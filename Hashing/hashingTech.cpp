#include <iostream>
#include <cmath>
#include <vector>
using namespace std;
typedef long long ll;

class hashingTechniques{
public:
    virtual int hash(int number,int size) = 0;
protected:
    int value;
};

class DivisionMethod : public hashingTechniques{
public:
    int hash(int number,int size) override{
        value = (number%size);
        return value;
    }
};

class MultiplicationMethod : public hashingTechniques{
public:
    int hash(int number,int size) override{
        double mod = fmod(number*0.4,1.0);
        value = floor(size * mod);
        return value;
    }
};

class MidSquareMethod: public hashingTechniques{
public:

    static int getNumberOfDigits(int size){
        int ans = 0;
        while(size > 0){
            ans++;
            size/=10;
        }
        return ans;
    }
    int hash(int number, int size)override{
        int r = getNumberOfDigits(size-1);
        int k2 = number * number;
        int sizeOfKSquared = getNumberOfDigits(k2);
        sizeOfKSquared = (sizeOfKSquared - r)/2;
        int midDigits = (k2 / (int)pow(10,sizeOfKSquared)) % (int)pow(10,r);
        value = midDigits % size;
        return value;
    }
};

class FoldingMethod : public hashingTechniques{
public:
    int fastPower(int a,int b){
        if(b == 0)return 1;
        int sqrt = fastPower(a, b >> 1);
        return sqrt * sqrt *((b&1)?a:1);
    }

    static int getNumberOfDigits(int size){
        int ans = 0;
        while(size > 0){
            ans++;
            size/=10;
        }
        return ans;
    }
    int hash(int number,int size) override{
        int digits = getNumberOfDigits(size-1);
        int tens = fastPower(10,digits);
        int ans = 0;
        while(number/tens > 0){
            int num = number % tens ;
            ans += num;
            number /= tens;
        }
        ans += number;
        value = ans % size;
        return value;
    }
};
class HashMap{
private:
    hashingTechniques *hq;
    int size;
    vector<vector<int>>map;
    int collisionResolutionTechnique;
public:
    static int getLeastPrime(int number){
        vector<bool> arr(1e5,true);
        vector<int> answers;
        arr[0] = arr[1] = false;
        for(int i = 2 ; i < 1e5; i++){
            if(arr[i]){
                answers.push_back(i);
                for(int j = 2*i; j < 1e5; j += i){
                    arr[j] = false;
                }
            }
        }
        int index = lower_bound(answers.begin(), answers.end(), number)-answers.begin();
        if(index > 0){
            return answers[index-1];
        }
        else return answers[index];
    }
    void insert(int number){
        if(collisionResolutionTechnique == 1){
            int idx = hq->hash(number,size);
            map[idx].push_back(number);
        }
        else if(collisionResolutionTechnique == 2){
            int idx = hq->hash(number,size);
            for(int i = 0; i < size*2; i++){
                if(map[(idx+i)%size].empty()){
                    map[(idx+i)%size].push_back(number);
                    break;
                }
            }
        }
        else if(collisionResolutionTechnique == 3){
            int idx = hq->hash(number,size);
            for(int i = 0; i < size*2; i++){
                if(map[(idx+(i*i))%size].empty()){
                    map[(idx+(i*i))%size].push_back(number);
                    break;
                }
            }
        }
        else if(collisionResolutionTechnique == 4){
            int index = hq->hash(number,size);
            int primeLessThanSize = getLeastPrime(size);
            int secondHash = hq->hash(number,primeLessThanSize);
            for(int i = 0; i < size*2; i++){
                int newIndex = (index + i*secondHash)%size;
                if(map[newIndex].empty()){
                    map[newIndex].push_back(number);
                    return;
                }
            }

        }
    }
    void print(){
        for(int i = 0; i < size; i++){
            cout<< i <<": ";
            for(auto j : map[i]){
                cout<<j<<" ";
            }
            cout<<endl;
        }
    }
    bool search(int num){
        int idx = hq->hash(num,size);
        for(auto i : map[idx]){
            if(i == num)return true;
        }
        return false;
    }

    HashMap(int sz, hashingTechniques *h, int collisionTech){
        this->size = sz;
        this->hq = h;
        vector<vector<int>>v(size,vector<int>(0,-INT_MAX));
        map = v;
        this->collisionResolutionTechnique = collisionTech;
    }
};


int main()
{
    hashingTechniques *hashingTech;
    cout<<""
          "1- Division Method\n"
          "2- Multiplication Method\n"
          "3- Mid Square Method\n"
          "4- Folding Method\n"
          "Please enter the number Corresponding to the Hashing technique you want to use: ";
    int numberOfTech;cin>>numberOfTech;
    cout<<""
          "1- Chaining Method\n"
          "2- Open Addressing Method(Linear Probing)\n"
          "3- Open Addressing Method(Quadratic Probing)\n"
          "4- Double Hashing Method\n"
          "Please enter the number Corresponding to the Collision Resolution technique you want to use: ";
    int numberOfCollisionTech;cin>>numberOfCollisionTech;

    switch (numberOfTech) {
        case 1:
            hashingTech = new DivisionMethod();
            break;
        case 2:
            hashingTech = new MultiplicationMethod();
            break;
        case 3:
            hashingTech = new MidSquareMethod();
            break;
        case 4:
            hashingTech = new FoldingMethod();
            break;
        default:
            cout<<"NOT A VALID CHOICE! \n";
            return 0;
    }
    cout<<""
          "Enter the size of the map: ";
    int size;cin>>size;
    auto *hashmap = new HashMap(size,hashingTech, numberOfCollisionTech);
    while(true){
        cout<<"1- Add new Number\n"
              "2- Search For an Element\n"
              "3- Print the Whole Map\n"
              "4- Exit\n"
              "Please enter the number of the function you want to do: ";
        int op; cin>>op;
        if(op == 1){
            cout<<"Please enter the number you want to add: ";
            int number;cin>>number;
            hashmap->insert(number);
        }
        else if(op == 2){
            cout<<"Please enter the number you want to search for: ";
            int number;cin>>number;
            bool isFound = hashmap->search(number);
            cout<<"The number "<<number<<" is "<<(isFound ? "Found\n":"Not Found\n");
        }
        else if(op == 3){
            hashmap->print();
        }
        else if(op == 4){
            cout<<"EXITING.....";
            break;
        }
        else cout<<"PLEASE ENTER A VALID CHOICE!";
    }
    delete hashingTech;
    delete hashmap;

    return 0;
}