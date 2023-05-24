#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

void dfs(vector<vector<int>>& graph, int start, vector<bool>& visited) {
    stack<int> s;
    s.push(start);
    visited[start] = true;

    #pragma omp parallel
    {
        #pragma omp single
        {
            while (!s.empty()) {
                int vertex = s.top();
                s.pop();

                #pragma omp task firstprivate(vertex)
                {
                    for (int neighbor : graph[vertex]) {
                        if (!visited[neighbor]) {
                            s.push(neighbor);
                            visited[neighbor] = true;
                            dfs(graph, neighbor, visited);
                        }
                    }
                }
            }
        }
    }
}

void parallel_dfs(vector<vector<int>>& graph, int start, vector<bool>& visited) {
    dfs(graph, start, visited);
}

int main() {
    // Create a sample graph
    vector<vector<int>> graph = {
        {},             // Node 0
        {2, 3, 4},      // Node 1
        {5},            // Node 2
        {6},            // Node 3
        {7, 8},         // Node 4
        {},             // Node 5
        {},             // Node 6
        {},             // Node 7
        {9},            // Node 8
        {10},           // Node 9
        {}              // Node 10
    };

    int start = 1; // Starting node for DFS

    vector<bool> visited(graph.size(), false);
    parallel_dfs(graph, start, visited);

    cout << "Visited nodes: ";
    for (int i = 0; i < graph.size(); i++) {
        if (visited[i])
            cout << i << " ";
    }
    cout << endl;

    return 0;
}
