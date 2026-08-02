//in the Min-Cut, the flow and the capacity are exactly the same thing.


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
    
    // As per the standard 6-node network, 
    // source is 0 and sink is the last node N-1
    int source = 0;
    int sink = N - 1;

    vector<vector<int>> adj(N);
    vector<vector<int>> capacity(N, vector<int>(N, 0));
    
    // We must keep a copy of the original capacities because 
    // the algorithm alters the main 'capacity' matrix.
    vector<vector<int>> orig_capacity(N, vector<int>(N, 0));
    
    // We also keep the list of edges in the order they were provided 
    // so we can iterate through them and match the sample output order exactly.
    vector<pair<int, int>> edges_list;

    for(int i = 0; i < M; i++){
        int u, v, c;
        cin >> u >> v >> c;
        
        adj[u].push_back(v);
        adj[v].push_back(u);
        
        capacity[u][v] += c;
        orig_capacity[u][v] += c;
        edges_list.push_back({u, v});
    }

    // 1. Compute the Maximum Flow
    int max_flow = edmondKarp(source, sink, N, adj, capacity);
    
    // 2. Output the Max Flow (which equals Min Cut Capacity)
    cout << "Min cut capacity = " << max_flow << "\n";
    cout << "Min cut edges:\n";

    // 3. Find the Source Set (S) using a final BFS on the residual graph
    vector<bool> reachable(N, false);
    queue<int> q;
    
    q.push(source);
    reachable[source] = true;

    while(!q.empty()){
        int curr = q.front();
        q.pop();

        for(int neigh : adj[curr]){
            // Only traverse edges that still have residual capacity
            if(!reachable[neigh] && capacity[curr][neigh] > 0){
                reachable[neigh] = true;
                q.push(neigh);
            }
        }
    }

    // 4. Identify the cut edges
    // Any original edge that goes from a reachable node to an unreachable node is in the min-cut.
    for(auto& edge : edges_list){
        int u = edge.first;
        int v = edge.second;
        
        if(reachable[u] && !reachable[v]){
            cout << u << " -> " << v << "  (capacity " << orig_capacity[u][v] << ")\n";
        }
    }

    return 0;
}