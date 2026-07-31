#include <iostream>
#include "edmond-carp.cpp"
#include <string>

//using namespace std;

struct Flight{
    string id, from, to;
    int depr_time, arr_time;
};

int toMin(string& t){
    int h = stoi(t.substr(0, 2));
    int m = stoi(t.substr(3, 2));

    return h * 60 + m;
}

int main(){
    int F;
    cin >> F;

    vector<Flight> flights(F);
    for(int i = 0; i < F; i++){
        string depr, arr;
        cin >> flights[i].id >> flights[i].from >> flights[i].to >> depr >> arr;
        flights[i].depr_time = toMin(depr);
        flights[i].arr_time = toMin(arr);        
    }

    int n = F * 2 + 2;
    int source = 0;
    int sink = n - 1;
    
    vector<vector<int>> capacity(n, vector<int>(n, 0));
    vector<vector<int>> adj(n);

    for(int i = 0; i < F; i++){
        adj[source].push_back(i + 1);
        adj[i + 1].push_back(source);
        capacity[source][i + 1] += 1;

        adj[F + i + 1].push_back(sink);
        adj[sink].push_back(F + i + 1);
        capacity[F + i + 1][sink] += 1;
    }

    vector<vector<bool>> feasibility(F, vector<bool>(F, false));
    for(int i = 0; i < F; i++){
        for(int j = 0; j < F; j++){
            if(i == j) continue;
            if(flights[i].to == flights[j].from && flights[i].arr_time + 180 <= flights[j].depr_time){
                adj[i + 1].push_back(F + j + 1);
                adj[F + j + 1].push_back(i + 1);
                capacity[i + 1][F + j + 1] += 1;
                feasibility[i][j] = true;
            }
        }
    }

    int maxMatch = edmondKarp(source, sink, n, adj, capacity);

    vector<int> next(F, -1);
    vector<int> prev(F, -1);

    for(int i = 0; i < F; i++){
        for(int j = 0; j < F; j++){
            if(feasibility[i][j] && !capacity[i + 1][F + j + 1]){
                next[i] = j;
                prev[j] = i;
                break;
            }
        }
    }

    cout << "Number of Aircraft: " << F - maxMatch << endl;

    int aircraftNo = 1;
    for(int i = 0; i < F; i++){
        if(prev[i] == -1){
            cout << "Aircraft " << aircraftNo++ << ": " << flights[i].id;
            int curr = i;
            while(next[curr] != -1){
                curr = next[curr];
                cout << " -> " << flights[curr].id;
            }
            cout << endl;
        }
    }

    return 0;
}
