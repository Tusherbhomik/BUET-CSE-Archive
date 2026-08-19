#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstring>
#include <random>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iomanip>
using namespace std;

unsigned long hash2(unsigned char *str)
{
    unsigned long hash = 5381; // Initial seed value
    int c;

    while ((c = *str++) != 0) // Update termination condition
    {
        hash = ((hash << 5) + hash) ^ c; // Modified hash calculation
    }

    return hash;
}
unsigned long hash1(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}
unsigned long auxHash(const string &str)
{
    unsigned long hash = 5381;

    for (size_t i = 0; i < str.length(); i++)
    {
        hash = (hash << 5) + hash + static_cast<unsigned char>(str[i]) + i;
    }
    return hash + 1;
}
class HashTable
{ // chaining
    vector<list<pair<string, int>>> myHashTable1;
    unsigned long tableLength;
    unsigned int findcollides1;
    unsigned int probes;
    int hashIndentify;

public:
    HashTable(unsigned long N, int hashindentify)
    {
        tableLength = N;
        myHashTable1.resize(tableLength);
        findcollides1 = 0;
        probes = 0;
        hashIndentify = hashindentify;
    }
    double loadFactor() const
    {
        return static_cast<double>(size()) / tableLength;
    }
    bool Insert(string key, int value)
    {

        if (loadFactor() > 0.7)
        {
            rehash();
        }
        return InsertUtill(key, value);
    }
    unsigned long int getfindcollides()
    {
        return findcollides1;
    }
    unsigned long int getProbes()
    {
        return probes;
    }

    bool Find(string str)
    {
        unsigned long index;
        if (hashIndentify == 1)
        {
            index = hash1((unsigned char *)str.c_str()) % tableLength;
        }
        else
        {
            index = hash2((unsigned char *)str.c_str()) % tableLength;
        }

        if (myHashTable1[index].size() > 0)
        {
            for (auto pair : myHashTable1[index]) // why dont enter in this loop?
            {
                if (pair.first == str)
                {
                    return true;
                }
            }
        }
        return false;
    }

    void FindProb(string str)
    {
        unsigned long index;
        if (hashIndentify == 1)
        {
            index = hash1((unsigned char *)str.c_str()) % tableLength;
        }
        else
        {
            index = hash2((unsigned char *)str.c_str()) % tableLength;
        }
        if (myHashTable1[index].size() > 0)
        {

            for (auto pair : myHashTable1[index])
            {
                if (pair.first == str)
                {
                    probes++;
                    return;
                }
                probes++;
            }
        }
    }
    bool InsertUtill(string key, int value)
    {
        if (Find(key))
        {
            return false;
        }
        unsigned long index;
        if (hashIndentify == 1)
        {
            index = hash1((unsigned char *)key.c_str()) % tableLength;
        }
        else
        {
            index = hash2((unsigned char *)key.c_str()) % tableLength;
        }
        findcollides1 += (myHashTable1[index].size()) ? 1 : 0;
        myHashTable1[index].push_back({key, value});
        return true;
    }
    bool Delete(string str)
    {
        unsigned long index;
        if (hashIndentify == 1)
        {
            index = hash1((unsigned char *)str.c_str()) % tableLength;
        }
        else
        {
            index = hash2((unsigned char *)str.c_str()) % tableLength;
        }

        for (auto it = myHashTable1[index].begin(); it != myHashTable1[index].end(); it++)
        {
            if (it->first == str)
            {
                myHashTable1[index].erase(it);
                return true;
            }
        }
        return false;
    }
    void rehash()
    {
        unsigned long newTableLength = tableLength * 2; // Double the table size for rehashing
        vector<list<pair<string, int>>> newHashTable(newTableLength);
        for (const auto &bucket : myHashTable1)
        {
            for (const auto &entry : bucket)
            {
                unsigned long newIndex;
                if (hashIndentify == 1)
                {
                    newIndex = hash1((unsigned char *)entry.first.c_str()) % newTableLength;
                }
                else
                {
                    newIndex = hash2((unsigned char *)entry.first.c_str()) % newTableLength;
                }
                newHashTable[newIndex].push_back(entry);
                myHashTable1 = std::move(newHashTable);
                tableLength = newTableLength;
            }
        }
    }
};
class HashTable1
{
    unsigned long tableLength;
    vector<pair<string, int>> myHashTable;
    int hashIndentify;
    unsigned int findcollides1;
    unsigned int probes;

public:
    HashTable1(unsigned long N, int hashindentify)
    {
        tableLength = N;
        myHashTable.resize(tableLength, {"", -1});
        hashIndentify = hashindentify;
        findcollides1 = 0;
        probes = 0;
    }
    unsigned long int getfindcollides()
    {
        return findcollides1;
    }
    unsigned long int getProbes()
    {
        return probes;
    }
    bool Find(string str)
    {
        int i = 0;
        unsigned long hash;
        if (hashIndentify == 1)
        {
            hash = hash1((unsigned char *)str.c_str()) % tableLength;
        }
        else
        {
            hash = hash2((unsigned char *)str.c_str()) % tableLength;
        }
        unsigned long auxHashValue = auxHash(str);
        while (true)
        {
            unsigned long finalHash = (hash + i * auxHashValue) % tableLength;
            if (myHashTable[finalHash].first == str)
            {
                return true;
            }
            else if (myHashTable[finalHash].first == "")
            {
                return false;
            }
            i++;
        }
    }
    void FindProb(string str)
    {
        int i = 0;
        unsigned long hash;
        if (hashIndentify == 1)
        {
            hash = hash1((unsigned char *)str.c_str()) % tableLength;
        }
        else
        {
            hash = hash2((unsigned char *)str.c_str()) % tableLength;
        }
        unsigned long auxHashValue = auxHash(str);
        while (true)
        {
            probes++;
            unsigned long finalHash = (hash + i * auxHashValue) % tableLength;
            if (myHashTable[finalHash].first == str)
            {
                return;
            }
            i++;
        }
    }
    bool InsertUtill(string key, int value)
    {
        if (Find(key))
        {
            return false; // for ensuring no duplicate paisi so insert korbe na
        }
        unsigned long int i = 0;
        unsigned long hash;
        if (hashIndentify == 1)
        {
            hash = hash1((unsigned char *)key.c_str()) % tableLength;
        }
        else
        {
            hash = hash2((unsigned char *)key.c_str()) % tableLength;
        }
        unsigned long auxHashValue = auxHash(key);
        unsigned long finalHash;
        while (true)
        {
            finalHash = (hash + i * auxHashValue) % tableLength;
            if (myHashTable[finalHash].first == "" || myHashTable[finalHash].first == "D")
            {
                myHashTable[finalHash] = {key, value};
                return true;
            }
            findcollides1++;
            i++;
        }
    }
    bool Delete(string str)
    {
        bool flag = Find(str); // pele delete ,delete korle true return ,na hoy false return

        if (flag)
        {
            int i = 0;
            unsigned long hash;
            if (hashIndentify == 1)
            {
                hash = hash1((unsigned char *)str.c_str()) % tableLength;
            }
            else
            {
                hash = hash2((unsigned char *)str.c_str()) % tableLength;
            }
            unsigned long auxHashValue = auxHash(str);
            while (true)
            {
                unsigned long finalHash = (hash + i * auxHashValue) % tableLength;
                if (myHashTable[finalHash].first == str)
                {
                    myHashTable[finalHash] = {"D", -1};
                    return true;
                }
                i++;
            }
        }
        else
            return false;
    }
    void rehash()
    {
        unsigned long newTableLength = tableLength * 2; // Double the table size for rehashing
        vector<pair<string, int>> newHashTable(newTableLength, {"", -1});
        for (const auto &entry : myHashTable)
        {
            if (entry.first != "" && entry.first != "D")
            {
                unsigned long hash;
                if (hashIndentify == 1)
                {
                    hash = hash1((unsigned char *)entry.first.c_str()) % newTableLength;
                }
                else
                {
                    hash = hash2((unsigned char *)entry.first.c_str()) % newTableLength;
                }
                unsigned long auxHashValue = auxHash(entry.first);
                unsigned long finalHash;
                unsigned long i = 0;
                while (true)
                {
                    finalHash = (hash + i * auxHashValue) % newTableLength;
                    if (newHashTable[finalHash].first == "" || newHashTable[finalHash].first == "D")
                    {
                        newHashTable[finalHash] = entry;
                        break;
                    }
                    i++;
                }
            }
        }
        myHashTable = newHashTable;
        tableLength = newTableLength;
    }
};
class HashTable2
{
    unsigned long tableLength;
    vector<pair<string, int>> myHashTable;
    int hashIndentify;
    unsigned int findcollides1;
    unsigned int probes;

public:
    HashTable2(unsigned long N, int hashindentify)
    {
        tableLength = N;
        myHashTable.resize(tableLength, {"", -1});
        hashIndentify = hashindentify;
        findcollides1 = 0;
        probes = 0;
    }
    unsigned long int getfindcollides()
    {
        return findcollides1;
    }
    unsigned long int getProbes()
    {
        return probes;
    }
    bool Find(string str)
    {
        // c1=13,c2=83
        //  customHash(k, i) = (Hash(k) + C1 × i × auxHash(k) + C2 × i) % N
        unsigned long int c1 = 13, c2 = 83;
        int i = 0;
        unsigned long hash;
        if (hashIndentify == 1)
        {
            hash = hash1((unsigned char *)str.c_str()) % tableLength;
        }
        else
        {
            hash = hash2((unsigned char *)str.c_str()) % tableLength;
        }
        unsigned long auxHashValue = auxHash(str);
        while (true)
        {
            unsigned long finalHash = (hash + c1 * i * auxHashValue + c2 * i * i) % tableLength;
            ;
            if (myHashTable[finalHash].first == str)
            {
                return true;
            }
            else if (myHashTable[finalHash].first == "")
            {
                return false;
            }
            i++;
        }
    }
    void FindProb(string str)
    {
        int i = 0;
        unsigned long c1 = 13, c2 = 83;
        unsigned long hash;
        if (hashIndentify == 1)
        {
            hash = hash1((unsigned char *)str.c_str()) % tableLength;
        }
        else
        {
            hash = hash2((unsigned char *)str.c_str()) % tableLength;
        }
        unsigned long auxHashValue = auxHash(str);
        while (true)
        {
            probes++;
            unsigned long finalHash = (hash + c1 * i * auxHashValue + c2 * i * i) % tableLength;
            if (myHashTable[finalHash].first == str)
            {
                return;
            }
            i++;
        }
    }
    bool InsertUtill(string key, int value)
    {
        if (Find(key))
        {
            return false; // for ensuring no duplicate paisi so insert korbe na
        }
        unsigned long int i = 0;
        unsigned long int c1 = 13, c2 = 83;
        unsigned long hash;
        if (hashIndentify == 1)
        {
            hash = hash1((unsigned char *)key.c_str()) % tableLength;
        }
        else
        {
            hash = hash2((unsigned char *)key.c_str()) % tableLength;
        }
        unsigned long auxHashValue = auxHash(key);
        while (true)
        {
            unsigned long finalHash = (hash + c1 * i * auxHashValue + c2 * i * i) % tableLength;
            if (myHashTable[finalHash].first == "" || myHashTable[finalHash].first == "D")
            {
                myHashTable[finalHash] = {key, value};
                return true;
            }
            findcollides1++;
            i++;
        }
    }
    bool Delete(string str)
    {
        bool flag = Find(str); // pele delete ,delete korle true return ,na hoy false return

        if (flag)
        {
            unsigned long int i = 0;
            unsigned long int c1 = 13, c2 = 83;
            unsigned long hash;
            if (hashIndentify == 1)
            {
                hash = hash1((unsigned char *)str.c_str()) % tableLength;
            }
            else
            {
                hash = hash2((unsigned char *)str.c_str()) % tableLength;
            }
            unsigned long auxHashValue = auxHash(str);
            while (true)
            {
                unsigned long finalHash = (hash + c1 * i * auxHashValue + c2 * i * i) % tableLength;
                if (myHashTable[finalHash].first == str)
                {
                    myHashTable[finalHash] = {"D", -1};
                    return true;
                }
                i++;
            }
        }
        else
            return false;
    }
    void rehash()
    {
        unsigned long newTableLength = tableLength * 2; // Double the table size for rehashing
        vector<pair<string, int>> newHashTable(newTableLength, {"", -1});
        unsigned long c1 = 13, c2 = 83;

        for (const auto &entry : myHashTable)
        {
            if (entry.first != "" && entry.first != "D")
            {
                unsigned long hash;
                if (hashIndentify == 1)
                {
                    hash = hash1((unsigned char *)entry.first.c_str()) % newTableLength;
                }
                else
                {
                    hash = hash2((unsigned char *)entry.first.c_str()) % newTableLength;
                }
                unsigned long auxHashValue = auxHash(entry.first);
                unsigned long finalHash;
                unsigned long i = 0;
                while (true)
                {
                    finalHash = (hash + c1 * i * auxHashValue + c2 * i * i) % newTableLength;
                    if (newHashTable[finalHash].first == "" || newHashTable[finalHash].first == "D")
                    {
                        newHashTable[finalHash] = entry;
                        break;
                    }
                    i++;
                }
            }
        }
        myHashTable = newHashTable;
        tableLength = newTableLength;
    }

};
string generateRandomWord(int length)
{
    static const char charset[] = "abcdefghijklmnopqrstuvwxyz";
    int charsetSize = sizeof(charset) - 1;

    string randomWord;
    for (int i = 0; i < length; ++i)
    {
        randomWord += charset[rand() % charsetSize];
    }
    return randomWord;
}
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
vector<int> findUnique(int n)
{
    vector<int> unique_integers;
    for (int i = 0; i < n; ++i)
    {
        unique_integers.push_back(i);
    }
    random_device rd;
    mt19937 gen(rd());
    shuffle(unique_integers.begin(), unique_integers.end(), gen);
    unique_integers.resize(1000);
    return unique_integers;
}
int main()
{
    int i = 0;
    int arr[3] = {5000, 10000, 20000};
    while (i < 3)
    {
        srand(2005046);
        unsigned long N, noOfWords = 10000;
        // cin >> N;
        // int input = N;
        N = arr[i];
        int valueCount[6];
        memset(valueCount, 0, sizeof(valueCount));

        if (noOfWords > N)
        {
            noOfWords = N;
        }
        N = giveDesiredPrime(N);

        vector<string> str;
        HashTable h1(N, 1);
        HashTable h2(N, 2);
        HashTable1 h3(N, 1);
        HashTable1 h4(N, 2);
        HashTable2 h5(N, 1);
        HashTable2 h6(N, 2);
        for (int i = 0; valueCount[0] < noOfWords; i++)
        {
            int wordLength = 5 + rand() % 6;
            string randomWord = generateRandomWord(wordLength);
            if (h1.InsertUtill(randomWord, valueCount[0]))
            {
                valueCount[0]++;
                str.push_back(randomWord);
            }
            if (h2.InsertUtill(randomWord, valueCount[1]))
            {
                valueCount[1]++;
            }
            if (h3.InsertUtill(randomWord, valueCount[2]))
            {
                valueCount[2]++;
            }
            if (h4.InsertUtill(randomWord, valueCount[3]))
            {
                valueCount[3]++;
            }
            if (h5.InsertUtill(randomWord, valueCount[4]))
            {
                valueCount[4]++;
            }
            if (h6.InsertUtill(randomWord, valueCount[5]))
            {
                valueCount[5]++;
            }
        }

        vector<int> generatedVector = findUnique(noOfWords);
        for (int i = 0; i < 1000; i++)
        {
            h1.FindProb(str[generatedVector[i]]);
            h2.FindProb(str[generatedVector[i]]);
            h3.FindProb(str[generatedVector[i]]);
            h4.FindProb(str[generatedVector[i]]);
            h5.FindProb(str[generatedVector[i]]);
            h6.FindProb(str[generatedVector[i]]);
        }
        std::cout << setw(30) << left << "Hash Table Size"
                  << setw(30) << left << "Collision Resolution Method"
                  << setw(15) << left << " "
                  << setw(30) << left << "Hash1"
                  << setw(60) << left << "Hash2" << endl;

        std::cout
            << setw(30) << left << arr[i]
            << setw(30) << left << " "
            << setw(20) << left << "#no of collision"
            << setw(20) << left << "#no of probes"
            << setw(20) << left << "#no of collision"
            << setw(30) << left << "Avg probe" << endl;
        std::cout
            << setw(30) << left << " "
            << setw(30) << left << "Seperate Chaining"
            << setw(30) << left << h1.getfindcollides()
            << fixed << setprecision(2) << setw(20) << left << h1.getProbes() / 1000.0
            << setw(10) << left << h2.getfindcollides()
            << fixed << setprecision(2) << setw(30) << left << h2.getProbes() / 1000.0 << endl;
        std::cout
            << setw(30) << left << " "
            << setw(30) << left << "Double Hashing"
            << setw(30) << left << h3.getfindcollides()
            << fixed << setprecision(2) << setw(20) << left << h3.getProbes() / 1000.0
            << setw(10) << left << h4.getfindcollides()
            << fixed << setprecision(2) << setw(30) << left << h4.getProbes() / 1000.0 << endl;
        std::cout
            << setw(30) << left << " "
            << setw(30) << left << "Custom Probing"
            << setw(30) << left << h5.getfindcollides()
            << fixed << setprecision(2) << setw(20) << left << h5.getProbes() / 1000.0
            << setw(10) << left << h6.getfindcollides()
            << fixed << setprecision(2) << setw(30) << left << h6.getProbes() / 1000.0 << endl;
        std::cout << endl;
        i++;
    }
    return 0;
}