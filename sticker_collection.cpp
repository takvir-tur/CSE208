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

    int n, m;
    if (!(cin >> n >> m)) return 0;
    
    // Parse your stickers
    int my_total;
    cin >> my_total;
    vector<int> my_stickers(m + 1, 0);
    for (int i = 0; i < my_total; ++i) {
        int id;
        cin >> id;
        my_stickers[id]++;
    }

    // Parse your friends' stickers
    vector<vector<int>> friend_stickers(n + 1, vector<int>(m + 1, 0));
    for (int j = 1; j <= n; ++j) {
        int friend_total;
        cin >> friend_total;
        for (int i = 0; i < friend_total; ++i) {
            int id;
            cin >> id;
            friend_stickers[j][id]++;
        }
    }

    // 1. Graph Node Setup
    // Source = Node 0
    // Stickers = Nodes 1 to m
    // Friends = Nodes m + 1 to m + n
    // Sink = Node m + n + 1
    int total_nodes = m + n + 2;
    int source = 0;
    int sink = total_nodes - 1;

    vector<vector<int>> adj(total_nodes);
    vector<vector<int>> capacity(total_nodes, vector<int>(total_nodes, 0));
    vector<vector<int>> orig_capacity(total_nodes, vector<int>(total_nodes, 0));

    // Helper lambda to cleanly add edges
    auto addEdge = [&](int u, int v, int cap) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        capacity[u][v] = cap;
        orig_capacity[u][v] = cap; // Keep track for tracing the residual graph later
    };

    // 2. Build edges based exactly on problem instructions
    for (int i = 1; i <= m; ++i) {
        // Edge from source to sticker
        if (my_stickers[i] > 0) {
            addEdge(source, i, my_stickers[i]);
        }
        
        // Edge from sticker to dummy sink
        addEdge(i, sink, 1);

        for (int j = 1; j <= n; ++j) {
            int friend_node = m + j;
            
            // Edge from sticker to friend (if friend doesn't have it)
            if (friend_stickers[j][i] == 0) {
                addEdge(i, friend_node, 1);
            }
            
            // Edge from friend to sticker (if friend has duplicates)
            if (friend_stickers[j][i] > 1) {
                addEdge(friend_node, i, friend_stickers[j][i] - 1);
            }
        }
    }

    // 3. Compute maximum flow
    int max_distinct_stickers = edmondKarp(source, sink, total_nodes, adj, capacity);
    
    // Output the maximum number of different stickers
    cout << max_distinct_stickers << "\n";

    // 4. Trace the residual graph to find the exchanges
    bool exchanged = false;
    for (int j = 1; j <= n; ++j) {
        int friend_node = m + j;
        
        vector<int> received_stickers;
        vector<int> given_stickers;
        
        // Find stickers this friend received from you
        for (int i = 1; i <= m; ++i) {
            if (orig_capacity[i][friend_node] == 1 && capacity[i][friend_node] == 0) {
                received_stickers.push_back(i);
            }
        }
        
        // Find duplicate stickers this friend gave to you
        for (int i = 1; i <= m; ++i) {
            if (orig_capacity[friend_node][i] > 0) {
                int flow = orig_capacity[friend_node][i] - capacity[friend_node][i];
                // A friend might give multiple of the same duplicate
                for (int k = 0; k < flow; ++k) {
                    given_stickers.push_back(i);
                }
            }
        }
        
        // Pair up the exchanges (Flow in = Flow out)
        for (size_t k = 0; k < received_stickers.size(); ++k) {
            cout << "Exchange a sticker with number " << received_stickers[k] 
                 << " against a sticker with number " << given_stickers[k] 
                 << " of friend " << j << "\n";
            exchanged = true;
        }
    }

    if (!exchanged) {
        cout << "No exchange is possible\n";
    }

    return 0;
}