#include <iostream>

using namespace std;

// struct Node{
//     int key;
//     int height;
//     Node* left;
//     Node* right;
// };

class Node {
    public:
        int key;
        int height;
        Node* left;
        Node* right;
        
        Node(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
    };

class AVL_tree{
    Node* root;

    int getHeight(Node* n){
        if(n == nullptr) return 0;

        return n->height;
    }

    int getBalanceFactor(Node* n){
        if(n == nullptr) return 0;

        return getHeight(n->left) - getHeight(n->right);
    }

    Node* rightRotate(Node* a){
        Node* b = a->left;
        a->left = b->right;
        b->right = a;

        a->height = 1 + max(getHeight(a->left), getHeight(a->right));
        b->height = 1 + max(getHeight(b->left), getHeight(b->right));

        return b;
    }

    Node* leftRotate(Node* a){
        Node* b = a->right;
        a->right = b->left;
        b->left = a;

        a->height = 1 + max(getHeight(a->left), getHeight(a->right));
        b->height = 1 + max(getHeight(b->left), getHeight(b->right));

        return b;
    }

    Node* LR_rotate(Node* a){
        a->left = leftRotate(a->left);

        return rightRotate(a);
    }

    Node* RL_rotate(Node* a){
        a->right = rightRotate(a->right);

        return leftRotate(a);
    }

    Node* insertHelper(Node* node, int key, bool& duplicate){
        //base case
        if(node == nullptr){
            duplicate = false;
            return new Node(key);
        }

        //typical bst insertion
        if(key < node->key) node->left = insertHelper(node->left, key, duplicate);
        else if(key > node->key) node->right = insertHelper(node->right, key, duplicate);
        else{
            duplicate = true;
            return node;
        }

        //duplicate check
        if(duplicate) return node;

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        int bf = getBalanceFactor(node);

        //LL
        if(bf > 1 && node->left->key > key) return rightRotate(node);
        //RR
        if(bf < -1 && node->right->key < key) return leftRotate(node);
        //LR
        if(bf > 1 && node->left->key < key) return LR_rotate(node);
        //RL
        if(bf < -1 && node->right->key > key) return RL_rotate(node);

        return node;
    }

    Node* miniSuccessor(Node* node){
        if(node == nullptr) return nullptr;

        while(node->left != nullptr){
            node = node->left;
        }
        return node;
    }

    Node* deleteHelper(Node* node, int key, bool& found){
        if(node == nullptr){
            found = false;
            return node;
        }

        if(key < node->key) node->left = deleteHelper(node->left, key, found);
        else if(key > node->key) node->right = deleteHelper(node->right, key, found);
        else{
            found = true;

            if(node->left == nullptr){
                Node* temp = node->right;
                delete node;
                node = temp;
            }
            else if(node->right == nullptr){
                Node* temp = node->left;
                delete node;
                node = temp;
            }
            else{
                //have both children
                Node* temp = miniSuccessor(node->right);
                node->key = temp->key;
                bool temp2 = false;
                node->right = deleteHelper(node->right, temp->key, temp2);
            }
        }
                if(node == nullptr || !found) return node;

                node->height = 1 + max(getHeight(node->left), getHeight(node->right));

                int bf = getBalanceFactor(node);

                if(bf > 1){
                    int leftChildBF = getBalanceFactor(node->left);
                    if(leftChildBF >= 0) return rightRotate(node);
                    if(leftChildBF < 0) return LR_rotate(node);
                }

                if (bf < -1) {
                    int rightChildBF = getBalanceFactor(node->right);
                    if(rightChildBF <= 0) return leftRotate(node);
                    if(rightChildBF > 0) return RL_rotate(node);
                }

                return node;
    }

    void traverseHelper(Node* node, vector<int>& result){
        if(node == nullptr) return;
        
        traverseHelper(node->left, result);
        result.push_back(node->key);
        traverseHelper(node->right, result);
    }

    string printHelper(Node* node){
        //base case
        if(node == nullptr) return "";

        //leaf node
        if(node->left == nullptr && node->right == nullptr) return to_string(node->key);

        string leftTree = printHelper(node->left);
        string rightTree = printHelper(node->right);
        
        return to_string(node->key) + "(" + leftTree + "," + rightTree + ")";
    }

public:
    AVL_tree(){
        root = nullptr;
    }

    bool insert(int key){
        bool duplicate = true;
        root = insertHelper(root, key, duplicate);
        return !duplicate;
    }

    bool find(int key){
        Node* current = root;

        while(current != nullptr){
            if(current->key == key) return true;
            else if(current->key < key) current = current->right;
            else current = current->left;
        }

        return false;
    }

    bool erase(int key){
        bool isDeleted = false;
        root = deleteHelper(root, key, isDeleted);
        return isDeleted;
    }

    vector<int> traverse(){
        vector<int> result;
        traverseHelper(root, result);
        return result;
    }

    string getTree(){
        return printHelper(root); 
    }

    void print(){
        if(root == nullptr) cout << endl;
        else{
            cout << printHelper(root) << endl;
        }
    }
};