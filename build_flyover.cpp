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
    
    // Nodes are 1-indexed according to the problem description
    int source = 1;
    int sink = N;
    int total_nodes = N + 1; 

    vector<vector<int>> adj(total_nodes);
    vector<vector<int>> capacity(total_nodes, vector<int>(total_nodes, 0));

    // Parse existing roads
    for(int i = 0; i < M; i++){
        int u, v, c;
        cin >> u >> v >> c;
        adj[u].push_back(v);
        adj[v].push_back(u); // Reverse edge for residual tracking
        capacity[u][v] += c;
    }

    // 1. Run Edmonds-Karp to get the final baseline residual graph
    edmondKarp(source, sink, total_nodes, adj, capacity);

    // 2. Find all nodes reachable from the Source in the residual graph
    vector<bool> reachable_from_S(total_nodes, false);
    queue<int> q_S;
    q_S.push(source);
    reachable_from_S[source] = true;

    while(!q_S.empty()){
        int curr = q_S.front();
        q_S.pop();
        for(int neigh : adj[curr]){
            // Follow edges where we STILL have capacity to push flow forward
            if(!reachable_from_S[neigh] && capacity[curr][neigh] > 0){
                reachable_from_S[neigh] = true;
                q_S.push(neigh);
            }
        }
    }

    // 3. Find all nodes that can reach the Sink in the residual graph
    // We do this by running a backward BFS starting from the Sink!
    vector<bool> reaches_to_T(total_nodes, false);
    queue<int> q_T;
    q_T.push(sink);
    reaches_to_T[sink] = true;

    while(!q_T.empty()){
        int curr = q_T.front();
        q_T.pop();
        for(int neigh : adj[curr]){
            // We check if flow can go from 'neigh' into 'curr'
            if(!reaches_to_T[neigh] && capacity[neigh][curr] > 0){
                reaches_to_T[neigh] = true;
                q_T.push(neigh);
            }
        }
    }

    // 4. Process the Proposals
    int P;
    cin >> P;
    vector<int> valid_proposals;

    for (int i = 1; i <= P; ++i) {
        int u, v, c;
        cin >> u >> v >> c;
        
        // If the Source can reach 'u', and 'v' can reach the Sink, 
        // connecting them with a capacity > 0 strictly increases Max Flow!
        if (reachable_from_S[u] && reaches_to_T[v] && c > 0) {
            valid_proposals.push_back(i);
        }
    }

    // 5. Output Results
    if (valid_proposals.empty()) {
        cout << "None\n";
    } else {
        for (int i = 0; i < valid_proposals.size(); ++i) {
            cout << valid_proposals[i] << (i + 1 == valid_proposals.size() ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}

/*
4 5
1 2 5
1 3 5
2 3 5
2 4 5
3 4 5
3
2 3 100
1 4 50
1 4 100
*/