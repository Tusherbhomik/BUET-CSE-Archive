#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

template <typename T>
class SecondLayerHashTable
{
public:
    void insert(const string &key2, const T &value)
    {
        data[key2] = value;
    }

    T search(const string &key2)
    {
        if (data.find(key2) != data.end())
        {
            return data[key2];
        }
        return T(); // Return a default-constructed value if not found
    }

    vector<pair<string, T>> getAllEntries()
    {
        vector<pair<string, T>> entries;
        for (const auto &entry : data)
        {
            entries.emplace_back(entry.first, entry.second);
        }
        return entries;
    }

private:
    unordered_map<string, T> data;
};

template <typename T>
class TwoLayeredHashTable
{
private:
    unordered_map<string, SecondLayerHashTable<T>> firstLayer;

public:
    TwoLayeredHashTable(int N) : firstLayer(N) {}

    void insert(const string &key1, const string &key2, const T &value)
    {
        if (firstLayer.find(key1) == firstLayer.end())
        {
            firstLayer[key1] = SecondLayerHashTable<T>();
        }
        firstLayer[key1].insert(key2, value);
    }

    T search(const string &key1, const string &key2)
    {
        if (firstLayer.find(key1) != firstLayer.end())
        {
            return firstLayer[key1].search(key2);
        }
        return T(); // Return a default-constructed value if not found
    }

    vector<pair<string, T>> search(const string &key1)
    {
        if (firstLayer.find(key1) != firstLayer.end())
        {
            return firstLayer[key1].getAllEntries();
        }
        return vector<pair<string, T>>();
    }
};

int main()
{
    TwoLayeredHashTable<int> table(100);

    table.insert("bd", "dhaka", 500);
    table.insert("bd", "ctg", 300);
    table.insert("uk", "london", 800);

    cout << table.search("bd", "dhaka") << endl;
    cout << table.search("bd", "syl") << endl;

    vector<pair<string, int>> bdEntries = table.search("bd");
    for (const auto &entry : bdEntries)
    {
        cout << "(" << entry.first << ", " << entry.second << ")" << endl;
    }

    // cout << table.search("china") << endl;

    return 0;
}
