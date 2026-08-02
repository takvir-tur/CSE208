#include <iostream>
#include "edmond-carp.cpp"
#include <string>
#include <sstream>

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;
    
    // Consume the newline character left in the input stream after reading N and M
    string line;
    getline(cin, line); 

    // Graph Node Setup
    int total_nodes = N + M + 2;
    int source = 0;
    int sink = total_nodes - 1;

    vector<vector<int>> adj(total_nodes);
    vector<vector<int>> capacity(total_nodes, vector<int>(total_nodes, 0));

    // Connect Source to all Applicants (Capacity 1)
    for (int i = 1; i <= N; ++i) {
        adj[source].push_back(i);
        adj[i].push_back(source);
        capacity[source][i] = 1;
    }

    // Connect all Jobs to Sink (Capacity 1)
    for (int j = 1; j <= M; ++j) {
        adj[N + j].push_back(sink);
        adj[sink].push_back(N + j);
        capacity[N + j][sink] = 1;
    }

    // Parse the dynamic input and connect Applicants to Jobs
    for (int i = 0; i < M; ++i) {
        getline(cin, line);
        if (line.empty()) {
            i--; 
            continue;
        }
        
        stringstream ss(line);
        int job_id;
        ss >> job_id; 
        
        int applicant_id;
        while (ss >> applicant_id) {
            adj[applicant_id].push_back(N + job_id);
            adj[N + job_id].push_back(applicant_id); 
            
            capacity[applicant_id][N + job_id] = 1;
        }
    }

    // Compute maximum flow
    int max_assignments = edmondKarp(source, sink, total_nodes, adj, capacity);
    cout << max_assignments << "\n";

    // Trace the residual graph to find who got which job
    for (int i = 1; i <= N; ++i) {           // Loop through all applicants
        for (int j = 1; j <= M; ++j) {       // Loop through all jobs
            // If the reverse edge from Job to Applicant has a capacity of 1,
            // it means flow was successfully pushed from this Applicant to this Job!
            if (capacity[N + j][i] == 1) {
                cout << "Applicant " << i << " assigned to Job " << j << "\n";
            }
        }
    }

    return 0;
}