#include <iostream>
#include <vector>
#include <queue>

using namespace  std;

typedef pair<int, int> pii;     //pii == pair of integers

class DisjointSet{
    vector<int> parent, size;
public:
    DisjointSet(int n){
        parent.resize(n);
        size.resize(n, 1);
        for(int i = 0; i < n; i++){
            parent[i] = i;      //initially every nodes parent is itself
        }
    }

    int find(int node){
        if(node == parent[node]){
            return node;
        }

        return parent[node] = find(parent[node]);
    }

    void unionSize(int u, int v){
        int rootU = find(u);
        int rootV = find(v);

        if(rootU == rootV) return;

        if(size[rootU] < size[rootV]){
            parent[rootU] = rootV;
            size[rootV] += size[rootU];
        }
        else{
            parent[rootV] = rootU;
            size[rootU] += size[rootV];
        }
    }
};


class Graph {
private:
    int V; // Number of vertices
    vector<vector<pii>> adj; // Adjacency list: array of vectors containing {weight, neighbor}

public:
    // Constructor to initialize the graph with V vertices
    Graph(int vertices) {
        this->V = vertices;
        adj.resize(vertices);
    }

    // Input Method: Adds an undirected edge between u and v with weight w
    void addEdge(int u, int v, int w) {
        // Since it's undirected, add edge from u to v AND v to u
        adj[u].push_back({w, v});
        adj[v].push_back({w, u});
    }

    void removeEdge(int u, int v) {
        // 1. Remove v from u's adjacency list
        for (auto it = adj[u].begin(); it != adj[u].end(); ++it) {
            if (it->second == v) { // Remember: .second is the vertex, .first is the weight
                adj[u].erase(it);
                break; // Stop searching once we find and remove it
            }
        }

        // 2. Remove u from v's adjacency list
        for (auto it = adj[v].begin(); it != adj[v].end(); ++it) {
            if (it->second == u) {
                adj[v].erase(it);
                break; 
            }
        }
    }

    // Display Method: Prints all unique edges and their weights
    void displayGraph() const {
        cout << "\n--- Graph Edges ---\n";
        for (int i = 0; i < V; ++i) {
            for (const auto& edge : adj[i]) {
                int weight = edge.first;
                int neighbor = edge.second;
                
                // Only print if i < neighbor to prevent printing the same undirected edge twice
                if (i < neighbor) {
                    cout << "Edge: " << i << " - " << neighbor << " | Weight: " << weight << "\n";
                }
            }
        }
        cout << "-------------------\n";
    }

    void primMST(){
        priority_queue<pii, vector<pii>, greater<pii>> pq;          //Create a priority queue that holds pairs, store those pairs inside a vector under the hood(required cause cant edit 3rd argument without mentioning 2nd), and use the greater function to reverse the sorting so its min heap
        vector<bool> inMST(V, false);
        vector<int> parent(V, -1);
        vector<int> wt(V, 1e9);

        int startVertex = 0;
        pq.push({0, startVertex});    //{weight, vertex}
        wt[startVertex] = 0;

        int mstWT = 0;

        while(!pq.empty()){
            int u = pq.top().second;
            int currentWT = pq.top().first;
            pq.pop();

            if(inMST[u]) continue;

            inMST[u] = true;
            mstWT += currentWT;

            for(auto& edge : adj[u]){
                int v = edge.second;
                int w = edge.first;

                if(!inMST[v] && w < wt[v]){
                    wt[v] = w;
                    pq.push({wt[v], v});
                    parent[v] = u;
                }
            }
        }

        // 5. Print the Results
        cout << "Minimum Spanning Tree Edges:\n";
        cout << "----------------------------\n";
        for (int i = 1; i < V; ++i) { 
            // We start printing from 1 because 0 is the root and has no parent
            if (parent[i] != -1) {
                cout << "Edge: " << parent[i] << " - " << i << " | Weight: " << wt[i] << "\n";
            }
        }
        cout << "----------------------------\n";
        cout << "Total MST Weight: " << mstWT << "\n";

    }

    void kruskalMST(){
        vector<pair<int, pii>> edges;

        for(int u = 0; u < V; u++){
            for(auto& edge : adj[u]){
                int w = edge.first;
                int v = edge.second;

                if(u < v){                          //avoid duplications for undirected
                    edges.push_back({w, {u, v}});
                }
            }
        }

        sort(edges.begin(), edges.end());

        DisjointSet dsu(V);
        int mstWT = 0;

        for(auto& edge : edges){
            int w = edge.first;
            int u = edge.second.first;
            int v = edge.second.second;

            if(dsu.find(u) != dsu.find(v)){     //different root = doesnt make cycle
                mstWT += w;
                dsu.unionSize(u, v);
                cout << "Edge: " << u << " - " << v << " | Weight: " << w << "\n";
            }
        }
        cout << "----------------------------\n";
        cout << "Total MST Weight (Kruskal's): " << mstWT << "\n\n";


    }
    };

int main() {
    // Instantiate a Graph object with 5 vertices (0 to 4)
    Graph g(5);

    // Input data using the object's method
    g.addEdge(0, 1, 2);
    g.addEdge(0, 3, 6);
    g.addEdge(1, 2, 3);
    g.addEdge(1, 3, 8);
    g.addEdge(1, 4, 5);
    g.addEdge(2, 4, 7);
    g.addEdge(3, 4, 9);

    // Call the display function to inspect the input graph structures
    g.displayGraph();

    // Run Prim's algorithm embedded inside the object
    cout << "Kruskals" << endl << endl;
    g.kruskalMST();

    cout << "prims" << endl << endl;
    g.primMST();

    return 0;
}