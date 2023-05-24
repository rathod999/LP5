#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

void bfs(vector<vector<int>>& graph, int start, vector<bool>& visited) {
    queue<int> q;
    q.push(start);
    visited[start] = true;

    #pragma omp parallel
    {
        #pragma omp single
        {
            while (!q.empty()) {
                int vertex = q.front();
                q.pop();
                cout<<vertex<< " "; 
                #pragma omp task firstprivate(vertex)
                {
                    for (int neighbor : graph[vertex]) {
                        if (!visited[neighbor]) {
                            q.push(neighbor);
                            visited[neighbor] = true;
                            bfs(graph, neighbor, visited);
                        }
                    }
                }
            }
        }
    }
}

void parallel_bfs(vector<vector<int>>& graph, int start) {
    vector<bool> visited(graph.size(), false);
    bfs(graph, start, visited);
}

int main() {
    // Create a sample graph
    vector<vector<int>> graph = {
        {},             // Node 0
        {2, 5},      // Node 1
        {3,4},            // Node 2
        {6},            // Node 3
        {7, 8},         // Node 4
        {},             // Node 5
        {},             // Node 6
        {},             // Node 7
        {9},            // Node 8
        {10},           // Node 9
        {}              // Node 10
    };

    int start = 1; // Starting node for BFS
    double st, end;
    st = omp_get_wtime();
    parallel_bfs(graph, start);
    end = omp_get_wtime();
    cout<<endl;
    cout<<"Time taken: "<<end-st<<endl;

    return 0;
}
