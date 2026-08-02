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

    int m, n;
    // m = max dances per pair, n = max dances per student
    if (!(cin >> m >> n)) return 0;
    
    int numX, numY;
    cin >> numX >> numY;
    
    int p;
    cin >> p;

    // 1. Graph Node Setup
    // Source = Node 0
    // Girls (Set X) = Nodes 1 to numX
    // Boys (Set Y) = Nodes numX+1 to numX+numY
    // Sink = Node numX+numY+1
    int total_nodes = numX + numY + 2;
    int source = 0;
    int sink = total_nodes - 1;

    vector<vector<int>> adj(total_nodes);
    vector<vector<int>> capacity(total_nodes, vector<int>(total_nodes, 0));

    // 2. Connect Source to all Girls (Capacity n)
    for (int i = 0; i < numX; ++i) {
        adj[source].push_back(i + 1);
        adj[i + 1].push_back(source);
        capacity[source][i + 1] = n;
    }

    // 3. Connect all Boys to Sink (Capacity n)
    for (int j = 0; j < numY; ++j) {
        adj[numX + j + 1].push_back(sink);
        adj[sink].push_back(numX + j + 1);
        capacity[numX + j + 1][sink] = n;
    }

    // Store the order of input pairs so we can print them sequentially later
    vector<pair<int, int>> pairs;
    
    // 4. Connect Girls to Boys based on registration (Capacity m)
    for (int i = 0; i < p; ++i) {
        int x, y;
        cin >> x >> y;
        pairs.push_back({x, y});
        
        int girl_node = x + 1;
        int boy_node = numX + y + 1;
        
        adj[girl_node].push_back(boy_node);
        adj[boy_node].push_back(girl_node); 
        
        capacity[girl_node][boy_node] = m;
    }

    // 5. Compute the maximum flow
    edmondKarp(source, sink, total_nodes, adj, capacity);
    
    // 6. Trace the residual graph to print output in the exact requested order
    for (auto& pr : pairs) {
        int x = pr.first;
        int y = pr.second;
        
        int girl_node = x + 1;
        int boy_node = numX + y + 1;
        
        // The flow successfully pushed across the edge is now stored in the reverse edge capacity
        int flow = capacity[boy_node][girl_node];
        
        cout << "(" << x << "," << y << ") -> " << flow << " dance";
        
        // Handle pluralization perfectly matching the sample output
        if (flow != 1) {
            cout << "s";
        }
        cout << "\n";
    }

    return 0;
}