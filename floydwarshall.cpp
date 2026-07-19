#include <iostream>
#include <vector>

using namespace std;

const int INF = 1e9;

// ---------------------------------------------------------
// 1. Path Reconstruction Helper
// ---------------------------------------------------------
vector<int> reconstructPath(int start, int end, const vector<vector<int>>& next_node) {
    // If there is no valid next step, the target is unreachable
    if (next_node[start][end] == -1) {
        return {}; 
    }

    vector<int> path = {start};
    int curr = start;
    
    // Walk through the next_node matrix until we reach the destination
    while (curr != end) {
        curr = next_node[curr][end];
        path.push_back(curr);
    }
    
    return path;
}

void printAllPairsResult(int n, const vector<vector<int>>& dist, const vector<vector<int>>& next_node) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue; // Skip self-paths

            if (dist[i][j] == INF) {
                cout << "Path " << i << " -> " << j << ": Unreachable\n";
            } else {
                vector<int> path = reconstructPath(i, j, next_node);
                cout << "Path " << i << " -> " << j << " (Cost: " << dist[i][j] << ") | Steps: ";
                for (size_t p = 0; p < path.size(); ++p) {
                    cout << path[p] << (p == path.size() - 1 ? "" : " -> ");
                }
                cout << "\n";
            }
        }
        cout << "---\n";
    }
}

// ---------------------------------------------------------
// 2. Floyd-Warshall Algorithm
// ---------------------------------------------------------
void floydWarshall(int n, vector<vector<int>>& dist, vector<vector<int>>& next_node) {
    // Core Algorithm: Try routing every path through intermediate node k
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                // Skip if a path through k doesn't exist (prevents integer overflow)
                if (dist[i][k] < INF && dist[k][j] < INF) {
                    // Relaxation Step
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        
                        // To go from i to j, we now take the first step toward k
                        next_node[i][j] = next_node[i][k]; 
                    }
                }
            }
        }
    }

    // Check for negative weight cycles
    // If the distance from a node to itself becomes negative, a cycle exists.
    bool negativeCycle = false;
    for (int i = 0; i < n; ++i) {
        if (dist[i][i] < 0) {
            negativeCycle = true;
            break;
        }
    }

    cout << "\n--- Floyd-Warshall Results ---\n";
    if (negativeCycle) {
        cout << "Error: Graph contains a negative weight cycle!\n";
    } else {
        printAllPairsResult(n, dist, next_node);
    }
}

// ---------------------------------------------------------
// 3. Main Function (Input Parsing)
// ---------------------------------------------------------
int main() {
    int V, E;
    cout << "Enter number of vertices and edges: ";
    if (!(cin >> V >> E)) return 0;

    // Initialize distance matrix with INF, and self-distances with 0
    vector<vector<int>> dist(V, vector<int>(V, INF));
    for (int i = 0; i < V; ++i) {
        dist[i][i] = 0;
    }

    // Initialize next_node matrix with -1
    vector<vector<int>> next_node(V, vector<int>(V, -1));

    cout << "Enter edges (u v weight):\n";
    for (int i = 0; i < E; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        
        // Setup for directed graph
        dist[u][v] = w;
        next_node[u][v] = v; // The next node after u is immediately v
        
        // Note: For undirected graph, uncomment the below lines:
        // dist[v][u] = w;
        // next_node[v][u] = u;
    }

    floydWarshall(V, dist, next_node);

    //for specific routes wanted by user----

    //first make floydWarshall function return boolean type, then the following...

    // bool hasNegativeCycle = floydWarshall(V, dist, next_node);

    // if (hasNegativeCycle) {
    //     cout << "\nCRITICAL ERROR: Infinite subsidy loop (negative cycle) detected!\n";
    //     cout << "Network routing shut down.\n";
    //     return 0;
    // }

    // Process specific user queries-----
    // int Q;
    // cout << "\nEnter number of routing queries: ";
    // cin >> Q;

    // cout << "Enter queries (StartCity DestCity):\n";
    // while (Q--) {
    //     int u, v;
    //     cin >> u >> v;

    //     if (dist[u][v] == INF) {
    //         cout << "City " << u << " -> City " << v << ": Unreachable\n";
    //     } else {
    //         vector<int> path = reconstructPath(u, v, next_node);
    //         cout << "City " << u << " -> City " << v 
    //              << " (Total Toll: " << dist[u][v] << ") | Route: ";
            
    //         for (size_t p = 0; p < path.size(); ++p) {
    //             cout << path[p] << (p == path.size() - 1 ? "" : " -> ");
    //         }
    //         cout << "\n";
    //     }
    // }

    return 0;
}