// this is a perfect textbook example of Menger’s Theorem in graph theory,
// which states that the maximum number of edge-disjoint (link-disjoint) paths
// from a source to a destination is exactly equal to the maximum flow when
// every edge is given a capacity of exactly 1.

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

// --- YOUR BFS & EDMONDS-KARP TEMPLATE (UNCHANGED) ---

bool bfs(int source, int sink, vector<int>& aug_path, const vector<vector<int>>& adj, const vector<vector<int>>& capacity){
    fill(aug_path.begin(), aug_path.end(), -1);
    aug_path[source] = -2;

    queue<int> q;
    q.push(source);

    while(!q.empty()){
        int curr = q.front();
        q.pop();

        for(int neigh : adj[curr]){
            if(aug_path[neigh] == -1 && capacity[curr][neigh] > 0){
                aug_path[neigh] = curr;

                if(neigh == sink) return true;
                q.push(neigh);
            }
        }
    }
    return false;
}

int edmondKarp(int source, int sink, int n, const vector<vector<int>>& adj, vector<vector<int>>& capacity){
    int max_flow = 0;
    vector<int> aug_path(n);

    while(bfs(source, sink, aug_path, adj, capacity)){
        int bottleneck = 1e9;
        int curr = sink;

        while(curr != source){
            int prev = aug_path[curr];
            bottleneck = min(bottleneck, capacity[prev][curr]);
            curr = prev;
        }

        max_flow += bottleneck;

        curr = sink;

        while(curr != source){
            int prev = aug_path[curr];
            
            capacity[prev][curr] -= bottleneck;
            capacity[curr][prev] += bottleneck;

            curr = prev;
        }
    }
    return max_flow;
}

// ----------------------------------------

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;
    
    int s, t;
    cin >> s >> t;

    // The nodes are indexed from 0 to N-1 based on the sample inputs,
    // so our total number of nodes is simply N.
    vector<vector<int>> adj(N);
    vector<vector<int>> capacity(N, vector<int>(N, 0));

    // Parse the M links
    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        
        // Add the directed edge to the adjacency list
        adj[u].push_back(v);
        // Add the reverse edge for the residual graph
        adj[v].push_back(u); 
        
        // Each link can transmit data in only one direction, 
        // so we add 1 to the forward capacity.
        capacity[u][v] += 1;
    }

    // Run your algorithm
    int max_paths = edmondKarp(s, t, N, adj, capacity);
    
    // Output formatting based on the problem statement
    if (max_paths == 0) {
        cout << -1 << "\n"; // t is not reachable from s
    } else {
        cout << max_paths << "\n";
    }

    return 0;
}