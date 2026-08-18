#include <bits/stdc++.h>
using namespace std;

class HashTable{
    public:
    int BUCKET;
    vector<vector<int>> table;;  
    
    HashTable(int N){
       vector<vector<int>> table(N,vector<int>(N, 0));
    }
    void insertItem(int key){
        int i = hashFunction(key);
        table[i].push_back(key);
    }
    void deleteItem(int key){
        int i = hashFunction(key);
        table[i].remove(key);
    }
    int hashFunction(int x){
        return (x % BUCKET);
    }
    void displayHash(){
        for(int i = 0; i < BUCKET; i++){
            cout << i;
            for(auto x : table[i])
                cout << " --> " << x;
            cout << endl;
        }
    }
};
int countBySieveOfEratosthenes(int n)
{
    vector<bool> isPrime(n + 1, true);
    int count = 0;
    for (int p = 2; p * p <= n; ++p)
    {
        if (isPrime[p])
        {
            for (int i = p * p; i <= n; i += p)
            {
                isPrime[i] = false;
            }
        }
    }
    for (int p = 2; p <= n; p++)
    {
        if (isPrime[p])
        {
            count++;
        }
    }
    return count;
}
int findNthPrime(int n)
{
    const int limit = 1000000;
    vector<bool> isPrime(limit + 1, true);
    vector<int> primes;
    for (int num = 2; num <= limit && primes.size() < n; ++num)
    {
        if (isPrime[num])
        {
            primes.push_back(num);
            for (int multiple = num * num; multiple <= limit; multiple += num)
                isPrime[multiple] = false;
        }
    }
    return primes.back();
}
int giveDesiredPrime(unsigned long int N)
{
    unsigned long noOfPrime, desiredPrime;
    noOfPrime = countBySieveOfEratosthenes(N);
    if (findNthPrime(noOfPrime) == N)
    {
        desiredPrime = N;
    }
    else
    {
        desiredPrime = findNthPrime(noOfPrime + 1);
    }
    return desiredPrime;
}
int main()
{
    int N;
    cin>>N;
    N = giveDesiredPrime(N);
    cout<<N<<endl;
    vector<vector<int>> matrix(N,vector<int>(N, 0));

    return 0;
}