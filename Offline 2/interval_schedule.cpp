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
        int start;
        int end;
        int id;
        int maxEnd;
        int height;
        Node* left;
        Node* right;
        
        Node(int s, int e, int i) : start(s), end(e), id(i), maxEnd(e), height(1), left(nullptr), right(nullptr) {}
    };

class AVL_tree_interval{
    Node* root;
    unordered_map<int, pair<int, int>> idMap;
    int nextId;

    int getHeight(Node* n){
        if(n == nullptr) return 0;

        return n->height;
    }
    
    int getMaxEnd(Node* n) {
        if (n == nullptr) return -2147483648; // Represents negative infinity
        return n->maxEnd;
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

        a->maxEnd = max(a->end, max(getMaxEnd(a->left), getMaxEnd(a->right)));
        b->maxEnd = max(b->end, max(getMaxEnd(b->left), getMaxEnd(b->right)));

        return b;
    }

    Node* leftRotate(Node* a){
        Node* b = a->right;
        a->right = b->left;
        b->left = a;

        a->height = 1 + max(getHeight(a->left), getHeight(a->right));
        b->height = 1 + max(getHeight(b->left), getHeight(b->right));

        a->maxEnd = max(a->end, max(getMaxEnd(a->left), getMaxEnd(a->right)));
        b->maxEnd = max(b->end, max(getMaxEnd(b->left), getMaxEnd(b->right)));

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

    Node* insertHelper(Node* node, int start, int end, int id){
        //base case
        if(node == nullptr){
            return new Node(start, end, id);
        }

        //typical bst insertion
        if(start < node->start || (start == node->start && id < node->id)) node->left = insertHelper(node->left, start, end, id);
        else if(start > node->start || (start == node->start && id > node->id)) node->right = insertHelper(node->right, start, end, id);
        // else{
        //     duplicate = true;
        //     return node;
        // }

        // //duplicate check
        // if(duplicate) return node;

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        node->maxEnd = max(node->end, max(getMaxEnd(node->left), getMaxEnd(node->right)));

        int bf = getBalanceFactor(node);

        //LL
        if(bf > 1 && (node->left->start > start || (start == node->left->start && id < node->left->id))) return rightRotate(node);
        //RR
        if(bf < -1 && (node->right->start < start || (start == node->right->start && id > node->right->id))) return leftRotate(node);
        //LR
        if(bf > 1 && (node->left->start < start || (start == node->left->start && id > node->left->id))) return LR_rotate(node);
        //RL
        if(bf < -1 && (node->right->start > start || (start == node->right->start && id < node->right->id))) return RL_rotate(node);

        return node;
    }

    Node* miniSuccessor(Node* node){
        if(node == nullptr) return nullptr;

        while(node->left != nullptr){
            node = node->left;
        }
        return node;
    }

    Node* deleteHelper(Node* node, int start, int end, int id, bool& found){
        if(node == nullptr){
            found = false;
            return node;
        }

        if(start < node->start || (start == node->start && id < node->id)) node->left = deleteHelper(node->left, start, end, id, found);
        else if(start > node->start || (start == node->start && id > node->id)) node->right = deleteHelper(node->right, start, end, id, found);
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
                node->start = temp->start;
                node->end = temp->end;
                node->id = temp->id;
                bool temp2 = false;
                node->right = deleteHelper(node->right, temp->start, temp->end, temp->id, temp2);
            }
        }
                if(node == nullptr || !found) return node;

                node->height = 1 + max(getHeight(node->left), getHeight(node->right));
                node->maxEnd = max(node->end, max(getMaxEnd(node->left), getMaxEnd(node->right)));

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

    void overlapsHelper(Node* node, int start, int end, vector<int>& result){
        if(node == nullptr)return;

        if(node->maxEnd <= start)return;

        overlapsHelper(node->left, start, end, result);

        if(start < node->end && node->start < end){
            result.push_back(node->id);
        }

        if(node->start < end){
            overlapsHelper(node->right, start, end, result);
        }
    }

    // void traverseHelper(Node* node, vector<int>& result){
    //     if(node == nullptr) return;
        
    //     traverseHelper(node->left, result);
    //     result.push_back(node->key);
    //     traverseHelper(node->right, result);
    // }

    string printHelper(Node* node){
        //base case
        if(node == nullptr) return "";

        //leaf node
        if(node->left == nullptr && node->right == nullptr) return to_string(node->id);

        string leftTree = printHelper(node->left);
        string rightTree = printHelper(node->right);
        
        return to_string(node->id) + "(" + leftTree + "," + rightTree + ")";
    }

public:
    AVL_tree_interval(){
        root = nullptr;
        nextId = 1;
    }

    void add(int start, int end){
        idMap[nextId] = {start, end};
        root = insertHelper(root, start, end, nextId);
        nextId++;
    }

    // bool find(int key){
    //     Node* current = root;

    //     while(current != nullptr){
    //         if(current->id == id) return true;
    //         else if(current->key < key) current = current->right;
    //         else current = current->left;
    //     }

    //     return false;
    // }

    bool remove(int id){
        if(idMap.find(id) == idMap.end()) return false;

        int start = idMap[id].first;
        bool isDeleted = false;
        root = deleteHelper(root, start, idMap[id].second, id, isDeleted);

        if(isDeleted) idMap.erase(id);

        return isDeleted;
    }

    bool update(int id, int newStart, int newEnd){
        if(idMap.find(id) == idMap.end()) return false;

        remove(id);

        idMap[id] = {newStart, newEnd};
        root = insertHelper(root, newStart, newEnd, id);

        return true;
    }

    bool conflict(int start, int end) {
        Node* current = root;
        while(current != nullptr){
            if(start < current->end && current->start < end){
                return true; 
            }

            if(current->left != nullptr && current->left->maxEnd > start){
                current = current->left;
            }
            else{
                current = current->right;
            }
        }

        return false;
    }

    vector<int> overlaps(int start, int end){
        vector<int> result;
        overlapsHelper(root, start, end, result);
        return result;
    }

    vector<int> at(int t){
        return overlaps(t, t + 1);
    }

    Node* next(int t){
        Node* give = nullptr;
        Node* current = root;

        while (current != nullptr) {
            if(current->start >= t){
                give = current;
                current = current->left;
            }
            else current = current->right;
        }

        return give;
    }
    // vector<int> traverse(){
    //     vector<int> result;
    //     traverseHelper(root, result);
    //     return result;
    // }

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