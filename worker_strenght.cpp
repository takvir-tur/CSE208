#include <iostream>
#include "edmond-carp.cpp"
#include <string>


int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;
    
    // Read the strength of the workers
    vector<int> worker_strength(N + 1);
    for (int i = 1; i <= N; ++i) {
        cin >> worker_strength[i];
    }
    
    // Read the strength required for the tasks
    vector<int> task_strength(M + 1);
    for (int j = 1; j <= M; ++j) {
        cin >> task_strength[j];
    }

    // 1. Graph Node Setup
    // Source = Node 0
    // Workers = Nodes 1 to N
    // Tasks = Nodes N+1 to N+M
    // Sink = Node N+M+1
    int total_nodes = N + M + 2;
    int source = 0;
    int sink = total_nodes - 1;

    vector<vector<int>> adj(total_nodes);
    vector<vector<int>> capacity(total_nodes, vector<int>(total_nodes, 0));

    // 2. Connect Source to all Workers (Capacity 1)
    for (int i = 1; i <= N; ++i) {
        adj[source].push_back(i);
        adj[i].push_back(source);
        capacity[source][i] = 1;
    }

    // 3. Connect all Tasks to Sink (Capacity 1)
    for (int j = 1; j <= M; ++j) {
        adj[N + j].push_back(sink);
        adj[sink].push_back(N + j);
        capacity[N + j][sink] = 1;
    }

    // 4. Connect Workers to Tasks based on strength
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= M; ++j) {
            // A worker can only do a task if their strength is greater than or equal to the required strength
            if (worker_strength[i] >= task_strength[j]) {
                adj[i].push_back(N + j);
                adj[N + j].push_back(i); 
                
                capacity[i][N + j] = 1;
            }
        }
    }

    // 5. Compute and print the result
    cout << edmondKarp(source, sink, total_nodes, adj, capacity) << "\n";

    for (int i = 1; i <= N; ++i) {           // Loop through all workers
        for (int j = 1; j <= M; ++j) {       // Loop through all tasks
            // If the reverse edge from Task to Worker has a capacity of 1,
            // it means flow was successfully pushed from this Worker to this Task!
            if (capacity[N + j][i] == 1) {
                cout << "Worker " << i << " assigned to Task " << j << "\n";
            }
        }
    }
    return 0;
}