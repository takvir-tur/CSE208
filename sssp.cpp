#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;

// Structure for Bellman-Ford
struct Edge {
    int u, v, weight;
};

// ---------------------------------------------------------
// 1. Path Reconstruction Helper
// ---------------------------------------------------------
vector<int> reconstructPath(int target, int source, const vector<int>& parent) {
    vector<int> path;
    
    // If the target is unreachable
    if (parent[target] == -1 && target != source) {
        return path; 
    }

    // Backtrack from target to source
    for (int curr = target; curr != -1; curr = parent[curr]) {
        path.push_back(curr);
    }

    // Reverse to get the path from source to target
    reverse(path.begin(), path.end());
    return path;
}

void printResult(int n, int source, const vector<int>& dist, const vector<int>& parent) {
    for (int i = 0; i < n; ++i) {
        vector<int> path = reconstructPath(i, source, parent);
        if (path.empty()) {
            cout << "Node " << i << ": Unreachable\n";
        } else {
            cout << "Node " << i << " (Cost: " << dist[i] << ") | Path: ";
            for (size_t j = 0; j < path.size(); ++j) {
                cout << path[j] << (j == path.size() - 1 ? "" : " -> ");
            }
            cout << "\n";
        }
    }
}

// ---------------------------------------------------------
// 2. Dijkstra's Algorithm
// ---------------------------------------------------------
void dijkstra(int source, int n, const vector<vector<pair<int, int>>>& adj) {
    vector<int> dist(n, INF);
    vector<int> parent(n, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u; // Record path
                pq.push({dist[v], v});
            }
        }
    }

    cout << "\n--- Dijkstra's Results ---\n";
    printResult(n, source, dist, parent);
}

// ---------------------------------------------------------
// 3. Bellman-Ford Algorithm
// ---------------------------------------------------------
void bellmanFord(int source, int n, const vector<Edge>& edges) {
    vector<int> dist(n, INF);
    vector<int> parent(n, -1);
    
    dist[source] = 0;

    // Relax all edges V-1 times
    for (int i = 1; i <= n - 1; ++i) {
        for (auto edge : edges) {
            if (dist[edge.u] != INF && dist[edge.u] + edge.weight < dist[edge.v]) {
                dist[edge.v] = dist[edge.u] + edge.weight;
                parent[edge.v] = edge.u; // Record path
            }
        }
    }

    // Check for negative weight cycles
    bool negativeCycle = false;
    for (auto edge : edges) {
        if (dist[edge.u] != INF && dist[edge.u] + edge.weight < dist[edge.v]) {
            negativeCycle = true;
            break;
        }
    }

    cout << "\n--- Bellman-Ford Results ---\n";
    if (negativeCycle) {
        cout << "Error: Graph contains a negative weight cycle!\n";
    } else {
        printResult(n, source, dist, parent);
    }
}

// ---------------------------------------------------------
// 4. Main Function (Input Parsing)
// ---------------------------------------------------------
int main() {
    int V, E;
    cout << "Enter number of vertices and edges: ";
    if (!(cin >> V >> E)) return 0; // Safeguard for empty input

    // Data structures for both algorithms
    vector<vector<pair<int, int>>> adj(V);
    vector<Edge> edges(E);

    cout << "Enter edges (u v weight):\n";
    for (int i = 0; i < E; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        
        // Populate Dijkstra's adjacency list (Directed Edge)
        adj[u].push_back({v, w});
        // Note: If the graph is undirected, uncomment the line below:
        // adj[v].push_back({u, w}); 

        // Populate Bellman-Ford's edge list
        edges[i] = {u, v, w};
        // Note: For undirected Bellman-Ford, you'd also need to add {v, u, w} to the edges vector.
    }

    int source;
    cout << "Enter source node: ";
    cin >> source;

    // Run both to compare
    dijkstra(source, V, adj);
    bellmanFord(source, V, edges);

    return 0;
}