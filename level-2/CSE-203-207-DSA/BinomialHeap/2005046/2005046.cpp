#include <bits/stdc++.h>
#include <typeinfo>
using namespace std;
class node
{
public:
    int data, degree;
    node *child, *sibling, *parent;
};
node *newNode(int x)
{
    node *temp = new node;
    temp->data = x;
    temp->degree = 0;
    temp->child = temp->sibling = temp->parent = NULL;
    return temp;
}

class binomialHeap
{
public:
    list<node *> myHeapRootList;

    node *mergeUtill(node *a, node *b)
    {
        /*if (a->data > b->data)
        {
            node *temp = a;
            a = b;
            b = temp;
        }*/
        if (a->data > b->data)
            swap(a, b);
        b->parent = a;
        b->sibling = a->child;
        a->child = b;
        a->degree++;
        return a;
    }
    void merge()
    {
        auto prev = myHeapRootList.begin();
        auto x = myHeapRootList.begin();
        auto next = myHeapRootList.begin();
        if (myHeapRootList.size() <= 1)
            return;
        x++;
        next = x;
        next++;
        while (prev != myHeapRootList.end())
        {
            if (x == myHeapRootList.end())
            {
                prev++;
            }
            else if ((*prev)->degree < (*x)->degree)
            {
                prev++;
                x++;
                if (next != myHeapRootList.end())
                {
                    next++;
                }
                // cout<<'b'<<endl;
            }
            else if (next != myHeapRootList.end() && (*prev)->degree == (*x)->degree && (*x)->degree == (*next)->degree)
            {
                prev++;
                x++;
                next++;
                // cout<<'c'<<endl;
            }
            else if ((*prev)->degree == (*x)->degree)
            {
                // cout<<'d'<<endl;
                *prev = mergeUtill(*prev, *x);
                x = myHeapRootList.erase(x);
                if (next != myHeapRootList.end())
                {
                    next++;
                }
            }
        }
    }
    void unionHeaps(list<node *> b)
    {
        list<node *> newList;
        auto it1 = myHeapRootList.begin();
        auto it2 = b.begin();

        while (it1 != myHeapRootList.end() && it2 != b.end())
        {
            (*it1)->degree <= (*it2)->degree ? newList.push_back(*it1++) : newList.push_back(*it2++);
        }
        while (it1 != myHeapRootList.end())
        {
            newList.push_back(*it1++);
        }
        while (it2 != b.end())
        {
            newList.push_back(*it2++);
        }
        myHeapRootList.clear();
        myHeapRootList = newList;
    }
    void insert(int x)
    {
        node *temp = newNode(x);
        list<node *> pivotList;
        pivotList.push_back(temp);
        unionHeaps(pivotList);
        merge();
    }
    node *findMin()
    {
        auto it = myHeapRootList.begin();
        node *temp = *it;

        while (it != myHeapRootList.end())
        {
            if ((*it)->data < temp->data)
                temp = *it;
            it++;
        }
        return temp;
    }
    list<node *> removeMin(node *tree)
    {
        list<node *> heap;
        node *temp = tree->child;
        node *lo;

        while (temp)
        {
            lo = temp;
            temp = temp->sibling;
            lo->sibling = NULL;
            heap.push_front(lo);
        }
        return heap;
    }
    int extractMin()
    {
        list<node *> newHeapRootList, tempList;
        node *temp;
        int data;

        temp = findMin();
        data = temp->data;

        auto it = myHeapRootList.begin();
        while (it != myHeapRootList.end())
        {
            if (*it != temp)
            {
                newHeapRootList.push_back((*it));
            }
            it++;
        }
        myHeapRootList.clear();
        myHeapRootList = newHeapRootList;
        newHeapRootList.clear();
        tempList = removeMin(temp);
        unionHeaps(tempList);
        merge();
        return data;
    }
    void print()
    {
        // cout << "My heap root list  size() :" << myHeapRootList.size() << endl;
        cout << "Printing Binomial Heap..." << endl;
        int treeCount = 0;
        // if(myHeapRootList.size()==0) cout<<"yes"<<endl;
        for (auto treeRoot : myHeapRootList)
        {
            cout << "Binomial Tree, B" << treeRoot->degree;

            queue<node *> q;
            q.push(treeRoot);
            node *temp = treeRoot;
            int level = -1;

            while (!q.empty())
            {
                node *current = q.front();
                q.pop();
                if (current == temp)
                {
                    level++;
                    temp = current->child;
                    cout << "\nLevel " << level << " : ";
                }
                cout << current->data << " ";
                node *child = current->child;
                while (child)
                {
                    q.push(child);
                    child = child->sibling;
                }
            }
            cout << endl;
            treeCount++;
        }
    }
};

int main()
{
    // freopen("in.txt", "r", stdin);
    // freopen("out.txt", "w", stdout);
    char ch;
    int x;
    bool flag = 0;
    binomialHeap b1;
    while (true)
    {
        if(flag==0) {
            if(!(cin>>ch))break;
            
        }
        flag=0;
        switch (ch)
        {
        case 'I':
        {
            cin >> x;
            b1.insert(x);
            break;
        }
        case 'F': // find-min
        {
            node *temp = b1.findMin();
            cout << "Find-Min returned " << temp->data << endl;
            break;
        }
        case 'E': // Extract-min
        {
            cout << "Extract-Min returned " << b1.extractMin() << endl;
            break;
        }
        case 'U': // Union
        {
            flag = 1;
            binomialHeap b2;
            string str;
            while (cin >> str)
            {
                //<<str<<endl;
                if (str[0] >= 'A')
                {
                    ch = str[0];
                    break;
                }
                else
                {
                    b2.insert(stoi(str));
                }
            }
            b1.unionHeaps(b2.myHeapRootList);
            b1.merge();
            // if (str[0] = 'P')
            // {
            //     b1.print();
            // }
            break;
        }
        case 'P': // Union
        {
            b1.print();
            break;
        }
        default:
        {
            cout << "Invalid command" << ch << endl;
            break;
        }
        }
    }

    return 0;
}
