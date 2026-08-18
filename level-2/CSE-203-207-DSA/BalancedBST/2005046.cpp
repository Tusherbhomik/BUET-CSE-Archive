#include <bits/stdc++.h>
using namespace std;

class avl_tree
{
public:
    class Node
    {
    public:
        int data;
        Node *l_next;
        Node *r_next;
        int height;
        Node(int x)
        {
            data = x;
            height = 1;
            l_next = NULL;
            r_next = NULL;
        }
    };
    int t_height;
    Node *root = NULL;

    void insert(int x)
    {
        root = insertUtil(root, x);
        printAVL(root);
        cout << endl;
    }
    bool find(int x)
    {
        Node *temp = findUtill(x, root);
        if (temp == NULL)
            return false;
        else
        {
            return true;
        }
    }
    Node *d_elete(int x)
    {
        root = d_eleteUtill(root, x);
        printAVL(root);
    }
    void traverse()
    {
        traverseUtil(root);
    }

private:
    int getHeight(Node *node)
    {
        if (node == NULL)
            return 0;
        return node->height;
    }
    // void updateHeight(Node *parent)
    // {
    //     int l_height = getHeight(parent->l_next);
    //     int r_height = getHeight(parent->r_next);
    //     if (l_height > r_height)
    //     {
    //         parent->height = l_height + 1;
    //     }
    //     else
    //     {
    //         parent->height = r_height + 1;
    //     }
    // }
    Node *r_rotation(Node *parent)
    {
        Node *newParent = parent->l_next;
        parent->l_next = newParent->r_next;
        newParent->r_next = parent;
        parent->height = 1 + max(getHeight(parent->l_next), getHeight(parent->r_next));
        newParent->height = 1 + max(getHeight(newParent->l_next), getHeight(newParent->r_next));
        return newParent;
    }
    Node *l_rotation(Node *parent)
    {
        Node *newParent = parent->r_next;
        parent->l_next = newParent->l_next;
        newParent->l_next = parent;
        parent->height = 1 + max(getHeight(parent->l_next), getHeight(parent->r_next));
        newParent->height = 1 + max(getHeight(newParent->l_next), getHeight(newParent->r_next));
        return newParent;
    }
    Node *insertUtil(Node *parent, int x)
    {

        if (parent == NULL)
        {
            t_height++;
            Node *newNode = new Node(x);
            return newNode;
        }

        else if (x < parent->data)
        {
            parent->l_next = insertUtil(parent->l_next, x);
        }
        else
        {
            parent->r_next = insertUtil(parent->r_next, x);
        }
        // updateHeight(parent);

        parent->height = 1 + max(getHeight(parent->l_next), getHeight(parent->r_next));
        int b_factor = getHeight(parent->l_next) - getHeight(parent->r_next);

        if (b_factor > 1)
        {
            if (x < parent->l_next->data)
            {
                return r_rotation(parent);
            }
            else
            {
                parent->l_next = l_rotation(parent->l_next);
                return r_rotation(parent);
            }
        }
        else if (b_factor < -1)
        {
            if (x > parent->r_next->data)
            {
                return l_rotation(parent);
            }
            else
            {
                parent->r_next = r_rotation(parent->r_next);
                return l_rotation(parent);
            }
        }
        // cout<<"ok InsertUTill "<<x<<endl;

        return parent;
    }
    Node *findUtill(int x, Node *parent)
    {
        cout << "Ok found" << endl;
        if (root == NULL)
        {
            return NULL;
        }
        if (x == parent->data)
            return root;
        if (x > parent->data)
            return findUtill(x, parent->r_next);
        if (x < parent->data)
            return findUtill(x, parent->l_next);
    }
    Node *d_eleteUtill(Node *parent, int x)
    {
        if (parent == NULL)
            return NULL;
        if (x < parent->data)
        {
            parent->l_next = d_eleteUtill(parent->l_next, x);
        }
        if (x > parent->data)
        {
            parent->r_next = d_eleteUtill(parent->r_next, x);
        }
        else
        {

            Node *tempR = parent->r_next;
            if (parent->r_next == NULL)
            {
                Node *tempL = parent->l_next;
                delete (parent);
                parent = tempL;
            }
            else if (parent->l_next == NULL)
            {

                delete (parent);
                parent = tempR;
            }
            else
            {
                while (tempR->l_next != NULL)
                {
                    tempR = tempR->l_next;
                    parent->data = tempR->data;
                    parent->r_next = d_eleteUtill(parent->r_next, tempR->data);
                }
            }
        }
        if (parent == NULL)
            return parent;
        parent->height = max(parent->l_next->height, parent->r_next->height);
        int b_factor = getHeight(parent->l_next) - getHeight(parent->r_next);
        if (b_factor > 1)
        {
            if (x < parent->l_next->height >= parent->r_next->height)
            {
                return r_rotation(parent);
            }
            else
            {
                parent->l_next = l_rotation(parent->l_next);
                return r_rotation(parent);
            }
        }
        else if (b_factor < -1)
        {
            if (x < parent->r_next->height >= parent->l_next->height)
            {
                return r_rotation(parent);
            }
            else
            {
                parent->r_next = l_rotation(parent->l_next);
                return l_rotation(parent);
            }
        }
        return parent;
    }
    void traverseUtil(Node *parent)
    {
        if (parent == NULL)
            return;
        traverseUtil(parent->l_next);
        cout << parent->data << " ";
        traverseUtil(parent->r_next);
    }
    void printAVL(Node *root)
    {
        if (root == nullptr)
            return;
        cout << root->data;

        if (root->l_next || root->r_next)
        {
            cout << "(";
            printAVL(root->l_next);
            cout << ",";
            printAVL(root->r_next);
            cout << ")";
        }
    }
};

int main()
{
    freopen("out.txt", "w", stdout);
    freopen("in.txt", "r", stdin);
    avl_tree t;
    char ch;
    int x;
    bool flag;
    while (cin >> ch)
    {
        if (ch == 'F')
        {
            cin >> x;
            flag = t.find(x);
            if (flag == true)
                cout << "found" << endl;
            else
                cout << "not found " << endl;
        }
        if (ch == 'I')
        {
            cin >> x;
            t.insert(x);
        }
        if (ch == 'D')
        {
            cin >> x;
            t.d_elete(x);
        }
        if (ch == 'T')
            t.traverse();
    }
    return 0;
}