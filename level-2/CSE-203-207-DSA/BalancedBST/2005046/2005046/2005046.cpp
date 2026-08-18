#include <bits/stdc++.h>
using namespace std;

class Node
{
public:
    int data;
    int height;
    Node *l_next;
    Node *r_next;
    Node(int x)
    {
        height = 1;
        data = x;
        l_next = NULL;
        r_next = NULL;
    }
};
class avl_tree
{
public:
    Node *root = NULL;
    int tree_height;

    void insert(int x)
    {
        root = insertUtil(root, x);
        printavl_tree(root);
        cout << endl;
    }
    void d_elete(int x)
    {
        root = d_eleteUtil(root, x);
        printavl_tree(root);
        cout << endl;
    }
    Node *find(int x)
    {
        return findUtil(root, x);
    }
    void traversal()
    {
        traversalUtil(root);
        cout << endl;
    }

private:
    int getHeight(Node *parent)
    {
        if (parent == NULL)
            return 0;
        return parent->height;
    }
    void updateHeight(Node *parent)
    {
        int lh = getHeight(parent->l_next);
        int rh = getHeight(parent->r_next);

        if (lh > rh)
        {
            parent->height = lh + 1;
        }
        else
        {
            parent->height = rh + 1;
        }
        // parent->height = 1 + max(getHeight(parent->l_next), getHeight(parent->r_next));
    }

    Node *r_rotation(Node *parent)
    {
        Node *newparent = parent->l_next;
        parent->l_next = newparent->r_next;
        newparent->r_next = parent;
        // parent->height = 1 + max(getHeight(parent->l_next), getHeight(parent->r_next));
        // newparent->height = 1 + max(getHeight(newparent->l_next), getHeight(newparent->r_next));
        // parent->height =
        updateHeight(parent);
        // newparent->height =
        updateHeight(newparent);
        return newparent;
    }

    Node *l_rotation(Node *parent)
    {
        Node *newparent = parent->r_next;
        parent->r_next = newparent->l_next;
        newparent->l_next = parent;
        // parent->height = 1 + max(getHeight(parent->l_next), getHeight(parent->r_next));
        // parent->height =
        updateHeight(parent);
        // newparent->height = 1 + max(getHeight(newparent->l_next), getHeight(newparent->r_next));
        // newparent->height=
        updateHeight(newparent);
        return newparent;
    }

    void traversalUtil(Node *parent)
    {
        if (parent == NULL)
            return;
        traversalUtil(parent->l_next);
        cout << parent->data << " ";
        traversalUtil(parent->r_next);
    }

    Node *insertUtil(Node *parent, int x)
    {
        if (parent == NULL)
        {
            tree_height++;
            Node *temp = new Node(x);
            return temp;
        }
        if (x < parent->data)
            parent->l_next = insertUtil(parent->l_next, x);
        else if (x > parent->data)
            parent->r_next = insertUtil(parent->r_next, x);

        // parent->height = updateHeight(parent);//Another problem
        parent->height = 1 + max(getHeight(parent->l_next), getHeight(parent->r_next));
        int bal = getHeight(parent->l_next) - getHeight(parent->r_next);
        if (bal > 1)
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
        else if (bal < -1)
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
        return parent;
    }
    Node *d_eleteUtil(Node *parent, int x)
    {
        if (parent == NULL)
            return NULL;
        if (x < parent->data)
        {
            parent->l_next = d_eleteUtil(parent->l_next, x);
        }
        else if (x > parent->data)
        {
            parent->r_next = d_eleteUtil(parent->r_next, x);
        }
        else
        {
            Node *r = parent->r_next;
            if (parent->r_next == NULL) // only right null kina,doesnt check left null or not
            {
                Node *l = parent->l_next;
                delete (parent);
                parent = l;
            }
            else if (parent->l_next == NULL) // only left null kina ami sure right null na
            {
                delete (parent);
                parent = r;
            }
            // only right /left alada alada null na , tar mane 2 tai null
            else
            {
                while (r->l_next != NULL)
                    r = r->l_next;
                parent->data = r->data;
                parent->r_next = d_eleteUtil(parent->r_next, r->data);
            }
        }
        if (parent == NULL)
            return parent;
        //parent->height = 1 + max(getHeight(parent->l_next), getHeight(parent->r_next));
        // parent->height=
        updateHeight(parent);
        int bal = getHeight(parent->l_next) - getHeight(parent->r_next);
        if (bal > 1)
        {
            //if (getHeight(parent->l_next) >= getHeight(parent->r_next))
            if(getHeight(parent->l_next->l_next)>=getHeight(parent->l_next->r_next))
            {
                return r_rotation(parent);
            }
            else
            {
                parent->l_next = l_rotation(parent->l_next);
                return r_rotation(parent);
            }
        }
        else if (bal < -1)
        {
            //if (getHeight(parent->r_next) >= getHeight(parent->l_next))
            if(getHeight(parent->r_next->l_next)<=getHeight(parent->r_next->r_next))
            {
                return l_rotation(parent);
            }
            else
            {
                parent->r_next = r_rotation(parent->r_next);
                return l_rotation(parent);
            }
        }
        return parent;
    }
    Node *findUtil(Node *parent, int x)
    {
        if (parent == NULL)
            return NULL;
        int k = parent->data;
        if (k == x)
            return parent;
        else if (k > x)
            return findUtil(parent->l_next, x);
        else
            return findUtil(parent->r_next, x);
    }
    void printavl_tree(Node *root)
    {
        if (root == nullptr)
            return;

        cout << root->data;

        if (root->l_next || root->r_next)
        {
            cout << "(";
            printavl_tree(root->l_next);
            cout << ",";
            printavl_tree(root->r_next);
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
    auto f_duration = 0;
    auto i_duration = 0;
    auto d_duration = 0;
    auto t_duration = 0;

    while (cin >> ch)
    {
        switch (ch)
        {
        case 'I':
        {
            cin >> x;
            auto start = chrono::high_resolution_clock::now();
            t.insert(x);
            auto end = chrono::high_resolution_clock::now();
            i_duration += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
            break;
        }

        case 'D': // Delete
        {
            cin >> x;
            auto start = chrono::high_resolution_clock::now();
            t.d_elete(x);
            auto end = chrono::high_resolution_clock::now();
            d_duration += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
            break;
        }

        case 'F': // Find
        {
            cin >> x;
            auto start = chrono::high_resolution_clock::now();
            flag = t.find(x);
            auto end = chrono::high_resolution_clock::now();
            f_duration += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
            if (flag == true)
                cout << "found" << endl;
            else
                cout << "not found" << endl;
            break;
        }
        case 'T':
        {
            auto start = chrono::high_resolution_clock::now();
            t.traversal();
            auto end = chrono::high_resolution_clock::now();
            t_duration += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
            break;
        }
        default:
        {
            cout << "Invalid command: " << ch << endl;
            break;
        }
        }
    }
    fclose(stdout);
    freopen("report_avl.txt", "w", stdout);

    cout << setw(12) << "Operation "
         << "Time(ms)" << endl;
    cout << setw(12) << "Insert    " << i_duration << endl;
    cout << setw(12) << "delete    " << d_duration << endl;
    cout << setw(12) << "search    " << f_duration << endl;
    cout << setw(12) << "traversal " << t_duration << endl;
    cout << setw(12) << "total     " << f_duration + d_duration + i_duration + t_duration << endl;
    return 0;
}
