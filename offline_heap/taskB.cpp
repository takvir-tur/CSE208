#include <string>
#include <algorithm>
#include <bitset>
#include <climits>
#include "binomial_heap.cpp"

using namespace std;

class BinomialHeapVisualizer{
private:
    static vector<Node*> getChildren(Node* parent){
        vector<Node*> children;
        Node* curr = parent->child;
        while (curr) {
            children.push_back(curr);
            curr = curr->sibling;
        }
        reverse(children.begin(), children.end());
        return children;
    }

    static void renderSubtree(Node* node, const string& prefix, bool isLast){
        if (!node) return;

        cout << prefix;
        cout << (isLast ? "└── " : "├── ");
        cout << "[" << node->key << "]" << " (deg: " << node->degree << ")\n";

        vector<Node*> children = getChildren(node);
        for (size_t i = 0; i < children.size(); ++i) {
            bool lastChild = (i == children.size() - 1);
            string nextPrefix = prefix + (isLast ? "    " : "│   ");
            renderSubtree(children[i], nextPrefix, lastChild);
        }
    }

    static void printRootChain(Node* head) {
    cout << "  Current Root Chain:\n    ";
    Node* curr = head;
    while (curr != nullptr) {
        cout << "[B" << curr->degree << " (Root: " << curr->key << ")]";
        if (curr->sibling != nullptr) {
            cout << " -> ";
        }
        curr = curr->sibling;
    }
    cout << " -> nullptr\n";
}

public:
    static void displayTree(Node* root){
        if (!root) return;
        cout << "  +---------------------------------------------------------\n";
        cout << "  | Binomial Tree B" << root->degree << " | Root: " << root->key 
             << " | Capacity: " << (1 << root->degree) << " nodes\n";
        cout << "  +---------------------------------------------------------\n";

        cout << "  [" << root->key << "] (Root)\n";
        vector<Node*> children = getChildren(root);
        for (size_t i = 0; i < children.size(); ++i) {
            bool isLast = (i == children.size() - 1);
            renderSubtree(children[i], "  ", isLast);
        }
        cout << "\n";
    }

    static void displayHeap(const BinomialHeap& heap, const string& heapName = "H"){
        cout << "\n===========================================================\n";
        cout << "                HEAP VISUALIZATION: " << heapName << "\n";
        cout << "===========================================================\n";
        cout << "Total Elements: " << heap.nodeCount << "\n";

        if(heap.root == nullptr || heap.nodeCount == 0){
            cout << "Status: Heap is empty.\n";
            cout << "===========================================================\n\n";
            return;
        }

        cout << "\n[Bit Analysis]\n";
        int n = heap.nodeCount;
        bitset<8> b(n);
        cout << "  • Binary Decomposition of Size (" << n << "): " << b.to_string() << "_2\n";
        cout << "  • Active Binomial Trees in Heap:\n";
        for (int k = 0; k < 6; ++k) {
            if ((n >> k) & 1) {
                cout << "    -> Bit " << k << " is SET: Exactly ONE B" << k 
                     << " tree (" << (1 << k) << " nodes) must exist.\n";
            }
        }
        cout << "  • Minimum Key in Heap: " << heap.findMin() << "\n\n";

        cout << "[Binomial Heap Visual]\n";
        Node* curr = heap.root;
        while (curr != nullptr) {
            displayTree(curr);
            curr = curr->sibling;
        }
        cout << "===========================================================\n\n";
    }

    static void visualizeUnion(BinomialHeap& h1, BinomialHeap& h2, 
                               const string& name1 = "H1", const string& name2 = "H2") {
        cout << "\n###########################################################\n";
        cout << "         STARTING UNION OPERATION: " << name1 << " U " << name2 << "\n";
        cout << "###########################################################\n";

        cout << "\n>>> [STATE BEFORE UNION] <<<\n";
        displayHeap(h1, name1);
        displayHeap(h2, name2);

        cout << ">>> [STEP 1: INTERLEAVING ROOT LISTS] <<<\n";
        cout << "Zippering roots of " << name1 << " and " << name2 << " by degree...\n\n";

        for (auto const& item : h2.nodeMap) {
            h1.nodeMap[item.first] = item.second;
        }
        h1.nodeCount += h2.nodeCount;
        h2.nodeMap.clear();
        h2.nodeCount = 0;

        Node* mergedHead = h1.mergeRoots(&h2);
        h1.root = nullptr;
        h2.root = nullptr;

        if(!mergedHead){
            cout << "Union complete: Both heaps were empty.\n";
            return;
        }

        // Show interleaved state
        cout << "--- Initial Interleaved State ---" << endl;
        printRootChain(mergedHead);
        cout << endl;

        Node* prevX = nullptr;
        Node* x = mergedHead;
        Node* nextX = x->sibling;

        int stepCount = 1;

        cout << ">>> [STEP 2: RESOLVING DEGREE COLLISIONS (CARRY LOGIC)] <<<\n\n";
        while(nextX != nullptr){
            cout << "================== [ITERATION " << stepCount++ << "] ==================\n";
            
            // Degree mismatch or 3 consecutive identical degrees
            if((x->degree != nextX->degree) ||
                (nextX->sibling != nullptr && nextX->sibling->degree == x->degree)) {
                
                if (x->degree != nextX->degree) {
                    cout << "ACTION: DO NOTHING [Degree Mismatch]\n";
                    cout << "  Degrees differ (B" << x->degree << " != B" << nextX->degree 
                         << "). Advancing sliding window.\n";
                } else {
                    cout << "ACTION: DO NOTHING [Three Consecutive Matches]\n";
                    cout << "  Three consecutive B" << x->degree 
                         << " trees detected. Skipping first tree to maintain degree order.\n";
                }
                prevX = x;
                x = nextX;
            } 
            // Same degree and x has the smaller key
            else if(x->key <= nextX->key){
                cout << "ACTION: [Link Under Current Root]\n";
                cout << "  Merging B" << x->degree << " (Root: " << x->key 
                     << ") and B" << nextX->degree << " (Root: " << nextX->key << ")\n";
                cout << "  " << x->key << " <= " << nextX->key 
                     << " ==> Tree [" << nextX->key << "] linked as child of [" << x->key << "]\n";
                cout << "  Formed new Tree B" << (x->degree + 1) << " with root " << x->key << "\n";

                x->sibling = nextX->sibling;
                h1.joinTrees(nextX, x);
                displayTree(x);
            } 
            // Same degree and nextX has the smaller key
            else{
                cout << "ACTION: [Link Under Next Root]\n";
                cout << "  Merging B" << x->degree << " (Root: " << x->key 
                     << ") and B" << nextX->degree << " (Root: " << nextX->key << ")\n";
                cout << "  " << nextX->key << " < " << x->key 
                     << " ==> Tree [" << x->key << "] linked as child of [" << nextX->key << "]\n";
                cout << "  Formed new Tree B" << (nextX->degree + 1) << " with root " << nextX->key << "\n";

                if(prevX == nullptr){
                    mergedHead = nextX;
                }
                else{
                    prevX->sibling = nextX;
                }
                h1.joinTrees(x, nextX);
                x = nextX;
                displayTree(x);
            }

            nextX = x->sibling;
            cout << "State after Iteration:\n";
            printRootChain(mergedHead);
            cout << "\n";
        }

        h1.root = mergedHead;

        cout << ">>> [STATE AFTER UNION FINALIZATION] <<<\n";
        displayHeap(h1, name1 + " (Merged Result)");
    }
};

int main(){
    BinomialHeap H1;
    BinomialHeap H2;

    cout << "===============================================================\n";
    cout << "                    BINOMIAL HEAP VISUALIZER.                  \n";
    cout << "===============================================================\n\n";

    H1.insert(5);
    H1.insert(100);
    H1.insert(101);
    H1.insert(102);
    H1.insert(103);
    H1.insert(104);
    H1.insert(105);
    H1.insert(106);

    H1.insert(12);
    H1.insert(10);

    H1.insert(50);

    H2.insert(40);
    H2.insert(30);

    H2.insert(20);

    BinomialHeapVisualizer::displayHeap(H1, "H1");
    BinomialHeapVisualizer::displayHeap(H2, "H2");

    BinomialHeapVisualizer::visualizeUnion(H1, H2, "H1", "H2");

    return 0;
}