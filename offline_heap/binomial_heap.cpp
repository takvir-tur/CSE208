#include <iostream>
#include<fstream>

using namespace std;
const int NEGA_INF = -2000000000;

struct Node
{
    int key;
    int degree;
    Node* parent;
    Node* child;
    Node* sibling;

    Node(int val) : key(val), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
};

class BinomialHeap{
    Node* root;
    int nodeCount;
    unordered_map<int, Node*> nodeMap;

    friend class BinomialHeapVisualizer;

    //make ones root others child
    void joinTrees(Node* root1, Node* root2){
        root1->parent = root2;
        root1->sibling = root2->child;
        root2->child = root1;
        root2->degree++;
    }

    // merge roots to LL in ascending of order (no consolidation yet)
    Node* mergeRoots(BinomialHeap* heap2){
    if(!this->root) return heap2->root;
    if(!heap2->root) return this->root;

    Node* curr1 = this->root;
    Node* curr2 = heap2->root;
    Node* newHead = nullptr;
    Node* tail = nullptr;

    if(curr1->degree <= curr2->degree){
        newHead = curr1;
        curr1 = curr1->sibling;
    }
    else{
        newHead = curr2;
        curr2 = curr2->sibling;
    }
    tail = newHead;

    while(curr1 && curr2){
        if(curr1->degree <= curr2->degree){
            tail->sibling = curr1;
            curr1 = curr1->sibling;
        }
        else{
            tail->sibling = curr2;
            curr2 = curr2->sibling;
        }
        tail = tail->sibling;
    }

    if (curr1) tail->sibling = curr1;
    if (curr2) tail->sibling = curr2;

    return newHead;
}

    void levelNodes(Node* node, int depth, vector<vector<int>>& levels){
        if(!node) return;
        
        if(depth >= levels.size()){
            levels.push_back(vector<int>());
        }
        levels[depth].push_back(node->key);
        
        Node* child = node->child;
        while(child){
            levelNodes(child, depth + 1, levels);
            child = child->sibling;
        }
    }

public:
    BinomialHeap() : root(nullptr), nodeCount(0) {}

    void unionHeap(BinomialHeap* heap2) {
        if (!heap2 || heap2->root == nullptr) return;

        for(auto const& [key, node] : heap2->nodeMap){
            this->nodeMap[key] = node;
        }
        this->nodeCount += heap2->nodeCount;
        
        heap2->nodeMap.clear();
        heap2->nodeCount = 0;

        Node* mergedHead = mergeRoots(heap2);
        this->root = nullptr;
        heap2->root = nullptr;

        if (!mergedHead) return;

        Node* prevX = nullptr;
        Node* x = mergedHead;
        Node* nextX = x->sibling;

        while(nextX != nullptr){
            //if degree not same(B1 -> B2) or 3 consecutive same degree (B2 -> B2 -> B2) skip for first 
            if((x->degree != nextX->degree) || 
                (nextX->sibling != nullptr && nextX->sibling->degree == x->degree)){
                prevX = x;
                x = nextX;
            }
            //same degree x and nextX and x key smaller
            else if(x->key <= nextX->key){
                x->sibling = nextX->sibling;
                joinTrees(nextX, x);
            }
            //same degree and nextX key smaller
            else{
                if (prevX == nullptr) {
                    mergedHead = nextX;
                } else {
                    prevX->sibling = nextX;
                }
                joinTrees(x, nextX);
                x = nextX;
            }
            nextX = x->sibling;
        }
        this->root = mergedHead;
    }

    void insert(int key){
        BinomialHeap* insertHeap = new BinomialHeap();
        Node* newNode = new Node(key);
        insertHeap->root = newNode;
        insertHeap->nodeCount = 1;
        insertHeap->nodeMap[key] = newNode;
        
        this->unionHeap(insertHeap);
        delete insertHeap;
    }

    int findMin() const {
        if (!root) return -1;
        Node* minNode = root;
        Node* next = root->sibling;
        while(next){
            if(next->key < minNode->key){
                minNode = next;
            }
            next = next->sibling;
        }
        return minNode->key;
    }

    void decreaseKey(int oldKey, int newKey) {
        if (nodeMap.find(oldKey) == nodeMap.end()) return;
        
        Node* curr = nodeMap[oldKey];
        
        nodeMap.erase(oldKey);
        curr->key = newKey;
        nodeMap[newKey] = curr;
        
        Node* parent = curr->parent;

        while(parent != nullptr && curr->key < parent->key){
            nodeMap[curr->key] = parent;
            nodeMap[parent->key] = curr;
            
            swap(curr->key, parent->key);
            
            curr = parent;
            parent = curr->parent;
        }
    }

    int extractMin() {
        if (!root) return -1;

        //find minNode
        Node* minNode = root;
        Node* minPrev = nullptr;
        Node* next = root->sibling;
        Node* prev = root;
        while (next != nullptr) {
            if (next->key < minNode->key) {
                minNode = next;
                minPrev = prev;
            }
            prev = next;
            next = next->sibling;
        }

        //cut minNode out of roots
        if (minPrev == nullptr) {
            root = minNode->sibling;
        } else {
            minPrev->sibling = minNode->sibling;
        }

        //making minNode's childer into roots
        BinomialHeap* orphans = new BinomialHeap();
        Node* child = minNode->child;
        Node* prevChild = nullptr;

        while (child != nullptr) {
            Node* nextChild = child->sibling;
            child->sibling = prevChild;
            child->parent = nullptr;
            prevChild = child;
            child = nextChild;
        }
        orphans->root = prevChild;

        //union to actual heap
        int minVal = minNode->key;
        this->nodeMap.erase(minVal);
        this->nodeCount--;
        
        minNode->child = nullptr;
        minNode->sibling = nullptr;

        this->unionHeap(orphans);
        
        delete minNode;
        delete orphans;
        
        return minVal;
    }

    void remove(int key) {
        decreaseKey(key, NEGA_INF);
        extractMin();
    }

    void print(int heapId, ofstream& outFile) {
        auto writeOut = [&](const string& msg) {
            cout << msg;
            outFile << msg;
        };

        writeOut("Printing Binomial Heap H" + to_string(heapId) + "\n");
        writeOut("Heap size: " + to_string(nodeCount) + "\n");

        if (nodeCount == 0) {
            writeOut("Heap H" + to_string(heapId) + " is empty.\n");
            return;
        }

        Node* curr = root;
        while (curr != nullptr) {
            writeOut("Binomial Tree, B" + to_string(curr->degree) + "\n");
            
            vector<vector<int>> levels;
            levelNodes(curr, 0, levels);

            for (size_t d = 0; d < levels.size(); ++d) {
                sort(levels[d].begin(), levels[d].end());
                string levelStr = "Level " + to_string(d) + ":";
                for (int val : levels[d]) {
                    levelStr += " " + to_string(val);
                }
                levelStr += "\n";
                writeOut(levelStr);
            }
            curr = curr->sibling;
        }
    }
};
