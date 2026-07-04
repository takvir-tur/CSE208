#include <iostream>
#include <vector>
#include <queue>

using namespace  std;

typedef pair<int, int> pii;     //pii == pair of integers

void primMST(int V, vector<vector<pii>>& adj){
    priority_queue<pii, vector<pii>, greater<pii>> pq;          //Create a priority queue that holds pairs, store those pairs inside a vector under the hood(required cause cant edit 3rd argument without mentioning 2nd), and use the greater function to reverse the sorting so its min heap
    vector<bool> inMST(V, false);
    vector<int> parent(V, -1);
    vector<int> wt(V, 1e9);

    


}