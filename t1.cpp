#include <iostream>
#include "edmond-carp.cpp"
#include <string>
#include <sstream>

//p1 - job application
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

//p2 - worker strenght
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

//P3 - matrimony
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

//P4- dance pairs
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

//P5- disjoint edge
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

//P-6 mincut edges

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

//P7 - flyover build

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

//P8 - sticker collection
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