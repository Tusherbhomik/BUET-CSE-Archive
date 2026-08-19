#include<bits/stdc++.h>
using namespace std;

template <typename T>
class AVL{
    public:
        class node{
            public:
                T key;
                int height;
                node * left;
                node * right;
                node(T k){
                    height = 1;
                    key = k;
                    left = NULL;
                    right = NULL;
                }
        };
        node * root = NULL;
        int n;
        void insert(T x){
            root=insertUtil(root, x);
        }
        void remove(T x){
            root=removeUtil(root, x);
        }
        node * search(T x){
            return searchUtil(root,x);
        }
        void inorder(){
            inorderUtil(root);
            cout<<endl;
        }
    private:
        int height(node * head){
            if(head==NULL) return 0;
            return head->height;
        }
        node * rightRotation(node * head){
            node * newhead = head->left;
            head->left = newhead->right;
            newhead->right = head;
            head->height = 1+max(height(head->left), height(head->right));
            newhead->height = 1+max(height(newhead->left), height(newhead->right));
            return newhead;
        }

        node * leftRotation(node * head){
            node * newhead = head->right;
            head->right = newhead->left;
            newhead->left = head;
            head->height = 1+max(height(head->left), height(head->right));
            newhead->height = 1+max(height(newhead->left), height(newhead->right));
            return newhead;
        }

        void inorderUtil(node * head){
            if(head==NULL) return ;
            inorderUtil(head->left);
            cout<<head->key<<" ";
            inorderUtil(head->right);
        }

        node * insertUtil(node * head, T x){
            if(head==NULL){
                n+=1;
                node * temp = new node(x);
                return temp;
            }
            if(x < head->key) head->left = insertUtil(head->left, x);
            else if(x > head->key) head->right = insertUtil(head->right, x);
            head->height = 1 + max(height(head->left), height(head->right));
            int bal = height(head->left) - height(head->right);
            if(bal>1){
                if(x < head->left->key){
                    return rightRotation(head);
                }else{
                    head->left = leftRotation(head->left);
                    return rightRotation(head);
                }
            }else if(bal<-1){
                if(x > head->right->key){
                    return leftRotation(head);
                }else{
                    head->right = rightRotation(head->right);
                    return leftRotation(head);
                }
            }
            return head;
        }
        node * removeUtil(node * head, T x){
            if(head==NULL) return NULL;
            if(x < head->key){
                head->left = removeUtil(head->left, x);
            }else if(x > head->key){
                head->right = removeUtil(head->right, x);
            }else{
                node * r = head->right;
                if(head->right==NULL){
                    node * l = head->left;
                    delete(head);
                    head = l;
                }else if(head->left==NULL){
                    delete(head);
                    head = r;
                }else{
                    while(r->left!=NULL) r = r->left;
                    head->key = r->key;
                    head->right = removeUtil(head->right, r->key);
                }
            }
            if(head==NULL) return head;
            head->height = 1 + max(height(head->left), height(head->right));
            int bal = height(head->left) - height(head->right);
            if(bal>1){
                if(height(head->left) >= height(head->right)){
                    return rightRotation(head);
                }else{
                    head->left = leftRotation(head->left);
                    return rightRotation(head);
                }
            }else if(bal < -1){
                if(height(head->right) >= height(head->left)){
                    return leftRotation(head);
                }else{
                    head->right = rightRotation(head->right);
                    return leftRotation(head);
                }
            }
            return head;
        }
        node * searchUtil(node * head, T x){
            if(head == NULL) return NULL;
            T k = head->key;
            if(k == x) return head;
            if(k > x) return searchUtil(head->left, x);
            if(k < x) return searchUtil(head->right, x);
        }
};

int main(){
   
    freopen("out.txt", "w", stdout);
    freopen("in.txt", "r", stdin);
    AVL<float> t;
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
            t.remove(x);
            auto end = chrono::high_resolution_clock::now();
            d_duration += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
            break;
        }

        case 'F': // Find
        {
            cin >> x;
            auto start = chrono::high_resolution_clock::now();
            flag = t.search(x);
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
            t.inorder();
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
}
