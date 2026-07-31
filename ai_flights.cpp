#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <climits>
#include <algorithm>

using namespace std;

struct Flight {
    string id, from, to;
    int dep, arr; // minutes since midnight
};

int timeToMinutes(const string& t) {
    int hh = stoi(t.substr(0, 2));
    int mm = stoi(t.substr(3, 2));
    return hh * 60 + mm;
}

bool bfs(int source, int sink, int n, vector<int>& parent,
         vector<vector<int>>& capacity, vector<vector<int>>& adj) {
    fill(parent.begin(), parent.end(), -1);
    parent[source] = -2;
    queue<int> q;
    q.push(source);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (parent[v] == -1 && capacity[u][v] > 0) {
                parent[v] = u;
                if (v == sink) return true;
                q.push(v);
            }
        }
    }
    return false;
}

int edmondsKarp(int source, int sink, int n,
                vector<vector<int>>& capacity, vector<vector<int>>& adj) {
    int maxFlow = 0;
    vector<int> parent(n);

    while (bfs(source, sink, n, parent, capacity, adj)) {
        int bottleneck = INT_MAX;
        int cur = sink;
        while (cur != source) {
            int prev = parent[cur];
            bottleneck = min(bottleneck, capacity[prev][cur]);
            cur = prev;
        }

        cur = sink;
        while (cur != source) {
            int prev = parent[cur];
            capacity[prev][cur] -= bottleneck;
            capacity[cur][prev] += bottleneck;
            cur = prev;
        }

        maxFlow += bottleneck;
    }
    return maxFlow;
}

int main() {
    int F;
    cin >> F;

    vector<Flight> flights(F);
    for (int i = 0; i < F; ++i) {
        string dep, arr;
        cin >> flights[i].id >> flights[i].from >> flights[i].to >> dep >> arr;
        flights[i].dep = timeToMinutes(dep);
        flights[i].arr = timeToMinutes(arr);
    }

    // Node layout: 0 = source
    //              1..F        = flight_out (left partition)
    //              F+1..2F     = flight_in  (right partition)
    //              2F+1        = sink
    int n = 2 * F + 2;
    int source = 0, sink = 2 * F + 1;

    vector<vector<int>> capacity(n, vector<int>(n, 0));
    vector<vector<int>> adj(n);

    auto addEdge = [&](int u, int v, int cap) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        capacity[u][v] += cap;
    };

    const int TURNAROUND = 180; // 3 hours in minutes

    for (int i = 0; i < F; ++i) {
        addEdge(source, 1 + i, 1);       // source -> i_out
        addEdge(F + 1 + i, sink, 1);     // i_in -> sink
    }

    vector<vector<bool>> feasible(F, vector<bool>(F, false));

    for (int i = 0; i < F; ++i) {
        for (int j = 0; j < F; ++j) {
            if (i == j) continue;
            if (flights[i].to == flights[j].from &&
                flights[i].arr + TURNAROUND <= flights[j].dep) {
                addEdge(1 + i, F + 1 + j, 1); // i_out -> j_in
                feasible[i][j] = true;
            }
        }
    }

    int maxMatching = edmondsKarp(source, sink, n, capacity, adj);

    // Reconstruct matching: edge i_out -> j_in was used if its residual
    // capacity dropped to 0 (started at capacity 1).
    vector<int> nextFlight(F, -1);
    vector<int> prevFlight(F, -1);

    for (int i = 0; i < F; ++i) {
        for (int j = 0; j < F; ++j) {
            if (feasible[i][j] && capacity[1 + i][F + 1 + j] == 0) {
                nextFlight[i] = j;
                prevFlight[j] = i;
                break;
            }
        }
    }

    int minAircraft = F - maxMatching;
    cout << "Number of Aircraft: " << minAircraft << "\n";

    int aircraftNum = 1;
    for (int i = 0; i < F; ++i) {
        if (prevFlight[i] == -1) { // start of a chain
            cout << "Aircraft " << aircraftNum++ << ": " << flights[i].id;
            int cur = i;
            while (nextFlight[cur] != -1) {
                cur = nextFlight[cur];
                cout << " -> " << flights[cur].id;
            }
            cout << "\n";
        }
    }

    return 0;
}