#include <iostream>
#include "edmond-carp.cpp"

int main() {
    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<int>> adj(n);
    vector<vector<int>> capacity(n, vector<int>(n, 0));     //adj matrix, residual calculation

    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        
        adj[u].push_back(v);
        adj[v].push_back(u);

        capacity[u][v] += w;
    }
    cout << edmondKarp(0, n-1, n, adj, capacity) << endl;

    return 0;
}