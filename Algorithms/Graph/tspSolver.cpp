/**
 * @file tspSolver.cpp
 * @brief Traveling Salesman Problem (TSP) solver using Held–Karp dynamic programming.
 * 
 * @details
 * ## Problem Definition
 * The Traveling Salesman Problem (TSP) is a classic NP-hard problem in combinatorial optimization.
 * Given a set of `n` cities (vertices) and the cost (weight) of traveling between each pair of cities, 
 * the goal is to find the shortest possible route that visits each city exactly once and returns to the starting city.
 *
 * ## History
 * - TSP is one of the most studied problems in computer science and operations research.
 * - Exact solutions are feasible only for small to moderate `n` due to its NP-hard nature.
 * - The Held–Karp algorithm (1962) uses dynamic programming to solve TSP in `O(n^2 * 2^n)` time.
 *
 * ## Solution Overview
 * We implement two exact DP approaches:
 * 1. **TopDown**: Recursive DFS with memoization (bitmask DP)
 * 2. **BottomUp**: Iterative DP (classic Held–Karp table)
 * 
 * Both approaches use a **state representation** where each DP entry represents:
 * 
 * - `dp[mask][u]` = the minimum cost to reach vertex `u` having visited all vertices in `mask`
 * 
 * **State transition**:
 * ```
 * dp[mask | (1<<v)][v] = min(dp[mask | (1<<v)][v], dp[mask][u] + cost[u][v])
 * ```
 * for all unvisited `v` connected to `u`.
 *
 * ## Time & Space Complexity
 * - **Time**: O(n^2 * 2^n) for both TopDown and BottomUp approaches
 * - **Space**: O(n * 2^n) to store the DP table
 */

#include <iostream>
#include <vector>
#include <limits>
#include "gtest/gtest.h"

class TSPSolver
{
public:
    TSPSolver(int numOfVertices, const std::vector<std::tuple<int, int, int>>& edges, int start = 0) : n{numOfVertices}, startPoint{start}
    {
        graph.assign(n, std::vector<int>(n, INF)); // INF - no connection
        for (auto [u, v, w] : edges) {
            graph[u][v] = w;
            graph[v][u] = w;
        }
    }

    int solveTopDown()
    {
        std::vector<std::vector<int>> dp(1 << n, std::vector<int>(n, -1));
        int minCost{dfs(dp, startPoint, 0/*mask*/)};
        
        return minCost == INF ? -1 : minCost;
    }

    int solveBottomUp()
    {
        std::vector<std::vector<int>> dp(1 << n, std::vector<int>(n, INF));
        dp[1 << startPoint][startPoint] = 0;

        for (int mask{}; mask < (1 << n); ++mask) {
            for (int u{}; u < n; ++u) {
                if ((mask & (1 << u)) == 0) continue; // u not in the mask
                if (dp[mask][u] == INF) continue;

                for (int v{}; v < n; ++v) {
                    if ((mask & (1 << v)) != 0 || graph[u][v] == INF) continue;
                    int newMask{mask | (1 << v)};

                    dp[newMask][v] = std::min(dp[newMask][v], dp[mask][u] + graph[u][v]);
                }
            }
        }

        int minCost{INF};
        int allVisited{(1 << n) - 1};
        for (int u{}; u < n; ++u) {
            if (u == startPoint || graph[u][startPoint] == INF) continue;
            minCost = std::min(minCost, dp[allVisited][u] + graph[u][startPoint]); 
        }

        return minCost == INF ? -1 : minCost;
    }

private:
    int dfs(std::vector<std::vector<int>>& dp, int u, int mask)
    {
        mask |= (1 << u);
        if (mask == (1 << n) - 1) {
            return graph[u][startPoint];
        }

        if (dp[mask][u] != -1) return dp[mask][u];

        int minCost{INF};
        for (int v{}; v < n; ++v) {
            if (graph[u][v] == INF) continue;

            if ((mask & (1 << v)) == 0) {
                minCost = std::min(minCost, graph[u][v] + dfs(dp, v, mask));
            }
        }

        return dp[mask][u] = minCost;
    }

private:
    int n{};
    int startPoint{};

    int minCost{};
    const int INF{std::numeric_limits<int>::max() / 2};
    
    std::vector<std::vector<int>> graph; // matrix
};

TEST(TSPSolverTest, SmallGraph)
{
    std::vector<std::tuple<int, int, int>> edges{
        {0, 1, 10}, {0, 2, 15}, {0, 3, 20}, {1, 2, 35}, {1, 3, 25}, {2, 3, 30}
    };

    TSPSolver solver{4, edges};

    int topDown{solver.solveTopDown()};
    int bottomUp{solver.solveBottomUp()};

    EXPECT_EQ(topDown, bottomUp);
    EXPECT_EQ(topDown, 80);
}

TEST(TSPSolverTest, TriangleGraph)
{
    std::vector<std::tuple<int, int, int>> edges{
        {0, 1, 5}, {1, 2, 7}, {0, 2, 9}
    };

    TSPSolver solver{3, edges};

    int topDown{solver.solveTopDown()};
    int bottomUp{solver.solveBottomUp()};

    EXPECT_EQ(topDown, bottomUp);
    EXPECT_EQ(topDown, 21); // 0 -> 1 -> 2 -> 0 = 5 + 7 + 9
}

TEST(TSPSolverTest, NoPathGraph)
{
    std::vector<std::tuple<int, int, int>> edges{
        {0, 1, 10}, {2, 3, 20} // disconnected graph
    };

    TSPSolver solver{4, edges};

    int topDown{solver.solveTopDown()};
    int bottomUp{solver.solveBottomUp()};

    EXPECT_EQ(topDown, bottomUp);
    EXPECT_EQ(topDown, -1); // No Hamiltonian cycle
}

TEST(TSPSolverTest, SquareGraph)
{
    std::vector<std::tuple<int, int, int>> edges{
        {0, 1, 1}, {1, 2, 1}, {2, 3, 1}, {3, 0, 1}, {0, 2, 2}, {1, 3, 2}
    };

    TSPSolver solver{4, edges};

    int topDown{solver.solveTopDown()};
    int bottomUp{solver.solveBottomUp()};

    EXPECT_EQ(topDown, bottomUp);
    EXPECT_EQ(topDown, 4); // minimal cycle cost
}

TEST(TSPSolverTest, BenchmarkBigGraph)
{
    int n{12};
    std::vector<std::tuple<int, int, int>> edges;
    for (int i{}; i < n; ++i) {
        for (int j{i + 1}; j < n; ++j) {
            edges.push_back({i, j, 1}); // complete graph, weight 1
        }
    }

    TSPSolver solver{n, edges};

    auto startTop{std::chrono::high_resolution_clock::now()};
    int topDown{solver.solveTopDown()};
    auto endTop{std::chrono::high_resolution_clock::now()};

    auto startBottom{std::chrono::high_resolution_clock::now()};
    int bottomUp{solver.solveBottomUp()};
    auto endBottom{std::chrono::high_resolution_clock::now()};

    EXPECT_EQ(topDown, bottomUp);
    EXPECT_EQ(topDown, n); // complete graph with weight 1 → cost = n

    std::cout << "TopDown time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endTop - startTop).count()
              << " ms\n";
    std::cout << "BottomUp time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endBottom - startBottom).count()
              << " ms\n";
    std::cout << "Diff (TopDown - BottomUp): "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endTop - startTop).count()
                 - std::chrono::duration_cast<std::chrono::milliseconds>(endBottom - startBottom).count()
              << " ms\n";
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
