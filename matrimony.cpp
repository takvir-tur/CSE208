#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>

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

// Struct to neatly hold candidate information
struct Person {
    int height;
    int age;
    int id;
};

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int m, n;
    if (!(cin >> m >> n)) return 0;
    
    // Read Men's information
    vector<Person> men(m + 1);
    for (int i = 1; i <= m; ++i) {
        cin >> men[i].height >> men[i].age >> men[i].id;
    }
    
    // Read Women's information
    vector<Person> women(n + 1);
    for (int j = 1; j <= n; ++j) {
        cin >> women[j].height >> women[j].age >> women[j].id;
    }

    // 1. Graph Node Setup
    // Source = Node 0
    // Men = Nodes 1 to m
    // Women = Nodes m+1 to m+n
    // Sink = Node m+n+1
    int total_nodes = m + n + 2;
    int source = 0;
    int sink = total_nodes - 1;

    vector<vector<int>> adj(total_nodes);
    vector<vector<int>> capacity(total_nodes, vector<int>(total_nodes, 0));

    // 2. Connect Source to all Men (Capacity 1)
    for (int i = 1; i <= m; ++i) {
        adj[source].push_back(i);
        adj[i].push_back(source);
        capacity[source][i] = 1;
    }

    // 3. Connect all Women to Sink (Capacity 1)
    for (int j = 1; j <= n; ++j) {
        adj[m + j].push_back(sink);
        adj[sink].push_back(m + j);
        capacity[m + j][sink] = 1;
    }

    // 4. Connect Men to Women based on matchmaking constraints
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            // Constraint 1: Height gap must not exceed 10 inches
            bool valid_height = abs(men[i].height - women[j].height) <= 10;
            // Constraint 2: Age gap must not surpass 5 years
            bool valid_age = abs(men[i].age - women[j].age) <= 5;
            
            if (valid_height && valid_age) {
                adj[i].push_back(m + j);
                adj[m + j].push_back(i); 
                
                capacity[i][m + j] = 1;
            }
        }
    }

    // 5. Compute the maximum matches
    int max_matches = edmondKarp(source, sink, total_nodes, adj, capacity);
    
    // Output the maximum number of matches
    cout << max_matches << "\n";

    // 6. Trace the residual graph to print who married whom
    for (int i = 1; i <= m; ++i) {           // Loop through all men
        for (int j = 1; j <= n; ++j) {       // Loop through all women
            // If the reverse edge from Woman to Man has a capacity of 1,
            // it means flow was successfully pushed from this Man to this Woman!
            if (capacity[m + j][i] == 1) {
                cout << "Man " << men[i].id << " - Woman " << women[j].id << "\n";
            }
        }
    }

    return 0;
}