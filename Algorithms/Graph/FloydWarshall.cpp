#include <vector>
#include <iostream>
#include <limits>

/**
 * @class FloydWarshallSolver
 * 
 * @brief Implements the Floyd-Warshall algorithm to compute the shortest paths
 * between all pairs of vertices in a weighted directed graph.
 * 
 * The Floyd-Warshall algorithm is a dynamic programming algorithm that:
 *   - Finds the shortest path distances between every pair of vertices.
 *   - Works for graphs with positive or negative edge weights (but no negative cycles).
 *   - Runs in O(n³) time, where n is the number of vertices.
 * 
 * This implementation also includes:
 *   - Path reconstruction support using a 'next' matrix.
 *   - Detection and propagation of negative cycles. If a negative cycle is detected,
 *     all pairs of nodes reachable through it are marked with a special value (`negInf`)
 *     and path reconstruction will throw an exception if such a path is queried.
 * 
 * Usage:
 *   - Initialize the class with number of vertices and the adjacency matrix.
 *   - Call solve() to compute shortest distances.
 *   - Use getShortestPath(u, v) to retrieve shortest distance from u to v.
 *   - Use reconstructPath(u, v) to retrieve the actual path as a list of vertices.
 */

class FloydWarshallSolver
{
private: 
    int n{};    
    std::vector<std::vector<int>> graph;
    std::vector<std::vector<int>> dp;
    std::vector<std::vector<int>> next; // for path recovery
    const int inf{std::numeric_limits<int>::max()};
    const int negInf{std::numeric_limits<int>::min()};

    void propagateNegativeCycle()
    {
        for (int k{}; k < n; ++k) {
            for (int i{}; i < n; ++i) {
                for (int j{}; j < n; ++j) {
                    if (dp[i][k] != inf && dp[k][j] != inf && dp[i][k] + dp[k][j] < dp[i][j]) {
                        dp[i][j] = negInf; // no "shortest" path available
                        next[i][j] = -1;
                    }
                }
            }
        }
    }
public:
    FloydWarshallSolver(int numberOfVertices, const std::vector<std::vector<int>>& g) : n{numberOfVertices}, graph{g}, dp{graph}
    {
        next.assign(n, std::vector<int>(n, -1)); // -1 indicates invalid paths

        for (int i{}; i < n; ++i) {
            for (int j{}; j < n; ++j) {
                if (graph[i][j] != inf) {
                    next[i][j] = j; // from i to j
                }
            }
        }
    }

    void solve()
    {
        for (int k{}; k < n; ++k) {
            for (int i{}; i < n; ++i) {
                for (int j{}; j < n; ++j) {
                    if (dp[i][k] != inf && dp[k][j] != inf && dp[i][k] + dp[k][j] < dp[i][j]) {
                        dp[i][j] = dp[i][k] + dp[k][j];
                        next[i][j] = next[i][k];
                    }
                }
            }
        }

        // dp matrix is the solution
        // usage: get shortest path from u -> v = dp[u][v] == inf ? no path : dp[u][v]

        // additional step
        propagateNegativeCycle();
    }

    int getShortestPath(int u, int v) const { return dp[u][v]; }

    std::vector<int> reconstructPath(int u, int v) const
    {
        std::vector<int> path;
        if (dp[u][v] == inf) {
            return path; // no path can be found
        }
        
        for (int i{u}; i != v; i = next[i][v]) {
            if (dp[i][v] == negInf) throw std::logic_error("Path contains negative cycle");
            if (next[i][v] == -1) return {}; // no valid path forward

            path.push_back(i);
        }

        path.push_back(v);

        return path;
    }
};

// normal testcase
void normalTest()
{
    const int INF = std::numeric_limits<int>::max();

    std::vector<std::vector<int>> graph = {
        {  0,  4, INF, INF, INF, INF, INF,  8, INF },
        {  4,  0,  8, INF, INF, INF, INF, 11, INF },
        {INF,  8,  0,  7, INF,  4, INF, INF,  2 },
        {INF, INF,  7,  0,  9, 14, INF, INF, INF },
        {INF, INF, INF,  9,  0, 10, INF, INF, INF },
        {INF, INF,  4, 14, 10,  0,  2, INF, INF },
        {INF, INF, INF, INF, INF,  2,  0,  1,  6 },
        {  8, 11, INF, INF, INF, INF,  1,  0,  7 },
        {INF, INF,  2, INF, INF, INF,  6,  7,  0 }
    };

    FloydWarshallSolver solver(9, graph);
    solver.solve();

    auto path = solver.reconstructPath(0, 4);
    std::cout << "From 0 -> 4 = " << solver.getShortestPath(0, 4) << std::endl;
    for (int v : path) std::cout << v << " ";
    std::cout << "\n";
}

void testNegativeCycle() {
    const int INF = std::numeric_limits<int>::max();
    std::vector<std::vector<int>> graph = {
        {0,    1,    INF},
        {INF,  0,   -1  },
        {-1,   INF,  0  }
    };
    FloydWarshallSolver solver(3, graph);
    solver.solve();

    // shortest path between nodes, should detect negative cycle
    try {
        auto path = solver.reconstructPath(0, 2);
        for (int v : path) std::cout << v << " ";
        std::cout << "\n";
    } catch (const std::logic_error& e) {
        std::cout << "Negative cycle detected in path reconstruction.\n";
    }
}

void testNoPath() {
    const int INF = std::numeric_limits<int>::max();
    std::vector<std::vector<int>> graph = {
        {0,   5,   INF, INF},
        {INF, 0,   3,   INF},
        {INF, INF, 0,   INF},
        {INF, INF, INF, 0}
    };
    // Node 3 is completely disconnected

    FloydWarshallSolver solver(4, graph);
    solver.solve();

    // There should be no path from node 0 to node 3
    int dist = solver.getShortestPath(0, 3);
    if (dist == INF) {
        std::cout << "No path from 0 to 3\n";
    } else {
        std::cout << "Shortest path from 0 to 3 = " << dist << "\n";
    }

    auto path = solver.reconstructPath(0, 3);
    if (path.empty()) {
        std::cout << "Path is empty, as expected.\n";
    } else {
        std::cout << "Unexpected path found: ";
        for (int v : path) std::cout << v << " ";
        std::cout << "\n";
    }
}

int main()
{
    // normalTest();
    // testNegativeCycle();
    testNoPath();
}