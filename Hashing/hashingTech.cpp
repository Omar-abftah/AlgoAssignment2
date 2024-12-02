#include <iostream>
#include <cmath>
#include <vector>

using namespace std;
typedef long long ll;

int divisionMethod(int k, int n){
    return (k%n);
}

int multiplicationMethod(int k, int n){
    double mod = fmod(k*0.4,1.0);
    double div = n/mod;

    return floor(div);
}

ll fastPower(ll a,ll b){
    if(b == 0)return 1;
    ll sqrt = fastPower(a, b >> 1);
    return sqrt * sqrt *((b&1)?a:1);
}

int getNumberOfDigits(ll size){
    int ans = 0;
    while(size > 0){
        ans++;
        size/=10;
    }
    return ans;
}

ll midSquareMethod(int k, int size){
    int r = getNumberOfDigits(size-1);
    ll k2 = fastPower(k,2);
    int sizeOfKSquared = getNumberOfDigits(k2);
    sizeOfKSquared = (sizeOfKSquared - r)/2;
    int tens = fastPower(10,sizeOfKSquared);
    k2 /= tens;
    k2 %= tens;
    k2 %= size;
    return k2;
}

ll foldingMethod(ll n,int size){
    int digits = getNumberOfDigits(size-1);
    int tens = fastPower(10,digits);
    ll ans = 0;
    while(n/tens > 0){
        ll num = n % tens ;
        ans += num;
        n /= tens;
    }
    ans += n;
    return ans;
}



int main()
{

    return 0;
}