#include <vector>
#include <queue>
#include <iostream>

/*
    Kahn's algorithm is a graph traversal algorithm used to find a topological ordering of a Directed Acyclic Graph (DAG).
    It works by iteratively removing nodes with zero in-degree (nodes that have no incoming edges) and updating the in-degree of their neighbors.
    Purpose
    Topological Sorting – It finds a valid ordering of nodes where each node appears before its dependencies in a DAG.
    Cycle Detection – If the algorithm processes all nodes, the graph is a DAG. If there are leftover nodes with nonzero in-degree,
        the graph contains a cycle, making topological sorting impossible.
    This method is widely used in task scheduling, dependency resolution (e.g., package managers), and compiler optimizations.
*/


// Function to perform Kahn's algorithm for topological sorting
// If a cycle is detected, the function will print an appropriate message.
void KahnAlgorithm(int n/*number of vertices*/, const std::vector<std::pair<int, int>>& edges) {
    std::vector<std::vector<int>> graph(n);
    std::vector<int> inDegree(n); // Stores the in-degree of each vertex

    for (auto [u, v] : edges) {
        graph[u].push_back(v); // Add directed edge u -> v
        ++inDegree[v]; // Increase in-degree of v
    }

    std::queue<int> q;
    // Push all nodes with in-degree 0 into the queue (starting points)
    for (int u{}; u < n; ++u) {
        if (inDegree[u] == 0) q.push(u); 
    }

    int visitedNodes{}; // Counter for cycle detection
    std::vector<int> topSort{}; // Stores topologically sorted order

    while (!q.empty()) {
        int u{q.front()};
        q.pop();
        ++visitedNodes;
        topSort.push_back(u); // Add vertex to the topological order

        // Reduce the in-degree of neighboring nodes
        for (int v : graph[u]) {
            --inDegree[v];
            if (!inDegree[v]) q.push(v); 
        }
    }

    // If we visited all nodes, a topological sort is possible
    if (visitedNodes == n) {
        std::cout << "Topological Order: ";
        for (int v : topSort) std::cout << v << " ";
        std::cout << std::endl;
    } else {
        std::cout << "Cycle detected! No valid topological sorting." << std::endl;
    }
}