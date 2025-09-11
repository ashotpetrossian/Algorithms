#include <iostream>
#include <vector>
#include <algorithm>

/**
 * @brief Eulerian Path / Cycle Solver for directed graphs.
 *
 * This class finds an Eulerian path or cycle in a directed graph using a modified
 * Hierholzer's algorithm. An Eulerian path is a path that visits every edge exactly
 * once. An Eulerian cycle is an Eulerian path that starts and ends at the same vertex.
 *
 * Directed Graph Rules:
 * - Eulerian cycle: Every vertex has equal in-degree and out-degree.
 * - Eulerian path: Exactly one vertex has out-degree = in-degree + 1 (start node),
 *   exactly one vertex has in-degree = out-degree + 1 (end node), all other vertices
 *   have equal in-degree and out-degree.
 *
 * Undirected Graph Note:
 * - This implementation currently only works for directed graphs.
 * - In an undirected graph:
 *     - Eulerian cycle: All vertices must have even degree.
 *     - Eulerian path: Exactly two vertices have odd degree (start and end), all others even.
 *   Adapting this algorithm for undirected graphs would require tracking edge usage
 *   in DFS to avoid traversing the same edge twice.
 *
 * Implementation Details:
 * - Graph is stored as an adjacency list (`std::vector<std::vector<int>> graph`).
 * - `inDegree` and `outDegree` vectors track in/out degrees for each vertex.
 * - Hierholzer's algorithm is implemented via a DFS (`dfs` function) using a cloned
 *   outDegree vector to mark edges as visited.
 * - The algorithm checks that all edges are used by comparing the number of edges in
 *   the path with the original number of edges.
 * - Time complexity: O(E), where E is the number of edges.
 * - Space complexity: O(V + E), for the adjacency list, degree arrays, and recursion stack.
 *
 * Key Methods:
 * - pathExists(int& source): Checks if an Eulerian path/cycle exists and determines
 *   the start vertex if applicable.
 * - dfs(std::vector<int>& path, std::vector<int>& out, int u): Performs DFS to construct
 *   the Eulerian path.
 * - solve(): Returns the Eulerian path as a vector of vertex indices, empty if none exists.
 *
 * Usage:
 * @code
 * std::vector<std::pair<int,int>> edges = {{0,1}, {1,2}, {2,0}};
 * EulerianPathSolver solver(3, edges);
 * auto path = solver.solve();
 * if (!path.empty()) {
 *     // Eulerian path found
 * }
 * @endcode
 */


class EulerianPathSolver
{
private:
    bool pathExists(int& source)
    {
        int startNodes{}, endNodes{};
        int possibleStartNode{};
        for (int i{}; i < n; ++i) {
            if (std::abs(inDegree[i] - outDegree[i]) > 1) return false;
    
            if (inDegree[i] - outDegree[i] == 1) ++endNodes;
            else if (outDegree[i] - inDegree[i] == 1) {
                ++startNodes;
                source = i;
            }
    
            if (outDegree[i] > 0) possibleStartNode = i;
        }
    
        // if all outs and ins were equal, then it doesn't matter what is the source vertex
        if (source == -1) source = possibleStartNode;
        
        return (startNodes == 0 && endNodes == 0) || (startNodes == 1 && endNodes == 1);
    }
    
    void dfs(std::vector<int>& path, std::vector<int>& out, int u)
    {
        while (out[u] > 0) {
            int index{--out[u]};
            int v{graph[u][index]};
            dfs(path, out, v);
        }
    
        path.push_back(u);
    }

public:
    EulerianPathSolver(int V /* number of vertices */, const std::vector<std::pair<int, int>>& edges) : n{V}, numberOfEdges{edges.size()}
    {
        graph.resize(n);
        inDegree.resize(n);
        outDegree.resize(n);

        for (auto [u, v] : edges) {
            graph[u].push_back(v);
            ++inDegree[v];
            ++outDegree[u];
        }
    }

    std::vector<int> solve()
    {
        int source{-1};
        bool exists = pathExists(source);

        if (!exists) return {};

        std::vector<int> path;
        auto outClone = outDegree;
        dfs(path, outClone, source);

        if (path.empty() || getNumberOfEdges() != path.size() - 1) return {};
        std::reverse(path.begin(), path.end());

        return path;
    }

    std::size_t getNumberOfEdges() const { return numberOfEdges; }

    void print()
    {
        for (int i{}; i < n; ++i) {
            std::cout << "Vertex: " << i << " -> ";
            for (int v : graph[i]) std::cout << v << ", ";
            std::cout << std::endl;
        }
    }

private:
    std::vector<std::vector<int>> graph;
    std::vector<int> inDegree, outDegree;
    int n{};
    std::size_t numberOfEdges{};
};

void test1()
{
    std::vector<std::pair<int, int>> edges {
        {1, 2}, {1, 3}, {3, 1}, {2, 2}, {2, 4}, {2, 4}, {4, 3}, {3, 2}, {3, 5}, {6, 3}, {4, 6}, {5, 6}
    };

    EulerianPathSolver solver(7, edges);
    auto path = solver.solve();

    if (path.empty()) {
        std::cout << "Eulerian Path does not exist" << std::endl;
        return;
    }

    std::cout << "The path\n";
    for (int v : path) std::cout << v << " ";
    std::cout << std::endl;

    if (path.front() == path.back()) std::cout << "The Eulerian path is also an Eulerian cycle\n";
}

void test2()
{
    std::vector<std::pair<int, int>> edges {
        {0, 1}, {1, 2}, {2, 0}
    };

    EulerianPathSolver solver(3, edges);
    auto path = solver.solve();

    if (path.empty()) {
        std::cout << "Eulerian Path does not exist" << std::endl;
        return;
    }

    std::cout << "The path\n";
    for (int v : path) std::cout << v << " ";
    std::cout << std::endl;

    if (path.front() == path.back()) std::cout << "The Eulerian path is also an Eulerian cycle\n";
}

int main()
{
    std::cout << "----------------test1\n";
    test1();
    std::cout << "----------------test2\n";
    test2();
}