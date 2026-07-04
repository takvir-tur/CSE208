#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// We use a pair where the first element is the edge WEIGHT 
// and the second element is the VERTEX. 
// We put weight first so the Priority Queue sorts by weight automatically.
typedef pair<int, int> pii;

class DisjointSet {
    vector<int> parent, size;
public:
    DisjointSet(int n) {
        parent.resize(n);
        size.resize(n, 1); // Initially, every group has a size of 1
        for (int i = 0; i < n; i++) {
            parent[i] = i; // Initially, every node is its own boss
        }
    }

    // Finds the ultimate parent of a node (with Path Compression)
    int findParent(int node) {
        if (node == parent[node])
            return node;
        // Path compression: attach directly to the ultimate boss
        return parent[node] = findParent(parent[node]); 
    }

    // Merges two groups together based on size
    void unionBySize(int u, int v) {
        int ultimateParentU = findParent(u);
        int ultimateParentV = findParent(v);

        // If they have the same boss, they are already connected
        if (ultimateParentU == ultimateParentV) return;

        // Attach the smaller group to the larger group
        if (size[ultimateParentU] < size[ultimateParentV]) {
            parent[ultimateParentU] = ultimateParentV;
            size[ultimateParentV] += size[ultimateParentU];
        } else {
            parent[ultimateParentV] = ultimateParentU;
            size[ultimateParentU] += size[ultimateParentV];
        }
    }
};

void kruskalMST() {
        // 1. Extract all edges into a flat list
        // Store as pairs of {weight, {u, v}} to easily sort by weight
        vector<pair<int, pair<int, int>>> edges;
        
        for (int u = 0; u < V; ++u) {
            for (auto& edge : adj[u]) {
                int w = edge.first;
                int v = edge.second;
                // To avoid duplicate edges in an undirected graph, 
                // only add the edge if u < v
                if (u < v) {
                    edges.push_back({w, {u, v}});
                }
            }
        }

        // 2. Sort all edges from lightest to heaviest
        // This takes O(E log E) time
        sort(edges.begin(), edges.end());

        // 3. Initialize the Disjoint Set
        DisjointSet ds(V);
        int mstWT = 0;
        
        cout << "\nKruskal's Minimum Spanning Tree Edges:\n";
        cout << "----------------------------\n";

        // 4. Greedily pick edges
        for (auto& edge : edges) {
            int w = edge.first;
            int u = edge.second.first;
            int v = edge.second.second;

            // Check if adding this edge creates a cycle
            // (Do u and v have the same ultimate boss?)
            if (ds.findParent(u) != ds.findParent(v)) {
                
                // No cycle! Add it to the MST
                mstWT += w;
                ds.unionBySize(u, v); // Merge their groups
                
                cout << "Edge: " << u << " - " << v << " | Weight: " << w << "\n";
            }
        }
        cout << "----------------------------\n";
        cout << "Total MST Weight (Kruskal's): " << mstWT << "\n\n";
    }

void primsMST(int V, vector<vector<pii>>& adj) {
    // 1. Initialize the Priority Queue (Min-Heap)
    // syntax for a min-heap holding pairs in C++
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    
    // 2. The three core tracking structures
    vector<bool> inMST(V, false); // Tracks if a vertex is in the tree
    vector<int> parent(V, -1);    // Tracks the MST structure (who pulled who in)
    vector<int> key(V, 1e9);      // Tracks minimum weights (initialized to infinity)

    // 3. Setup the starting vertex (Vertex 0)
    int startVertex = 0;
    pq.push({0, startVertex}); // {weight, vertex}
    key[startVertex] = 0;

    int mstWeight = 0; // To keep track of the total weight of the MST

    // 4. The Main Loop
    while (!pq.empty()) {
        // Extract the vertex with the minimum edge weight
        int u = pq.top().second;
        int currentWeight = pq.top().first;
        pq.pop();

        // Check Status: If vertex is already in MST, skip it
        if (inMST[u]) {
            continue;
        }

        // Add to Tree: Mark vertex as included
        inMST[u] = true;
        mstWeight += currentWeight; 

        // Update Neighbors: Look at all adjacent vertices
        for (auto& edge : adj[u]) {
            int v = edge.second; // The neighbor vertex
            int w = edge.first;  // The edge weight

            // If neighbor is NOT in MST and edge weight is cheaper than its current key
            if (!inMST[v] && w < key[v]) {
                key[v] = w;               // Update the key
                pq.push({key[v], v});     // Push the new cheaper connection to the queue
                parent[v] = u;            // Update parent for the MST structure
            }
        }
    }

    // 5. Print the Results
    cout << "Minimum Spanning Tree Edges:\n";
    cout << "----------------------------\n";
    for (int i = 1; i < V; ++i) { 
        // We start printing from 1 because 0 is the root and has no parent
        if (parent[i] != -1) {
            cout << "Edge: " << parent[i] << " - " << i << " | Weight: " << key[i] << "\n";
        }
    }
    cout << "----------------------------\n";
    cout << "Total MST Weight: " << mstWeight << "\n";
}

int main() {
    int V = 5; 
    
    // Adjacency list: A vector of vectors containing {weight, neighbor_vertex}
    vector<vector<pii>> adj(V);

    // Helper function to add undirected edges easily
    auto addEdge = [&](int u, int v, int w) {
        adj[u].push_back({w, v});
        adj[v].push_back({w, u});
    };

    // Constructing a sample graph
    addEdge(0, 1, 2);
    addEdge(0, 3, 6);
    addEdge(1, 2, 3);
    addEdge(1, 3, 8);
    addEdge(1, 4, 5);
    addEdge(2, 4, 7);
    addEdge(3, 4, 9);

    // Run the algorithm
    primsMST(V, adj);

    return 0;
}