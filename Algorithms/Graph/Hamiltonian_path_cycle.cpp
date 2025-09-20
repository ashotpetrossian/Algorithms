/**
 * @brief Hamiltonian Paths and Cycles Solver
 * 
 * This class provides methods to enumerate all Hamiltonian paths and cycles 
 * in an undirected graph.
 * 
 * A Hamiltonian path visits each vertex exactly once.
 * A Hamiltonian cycle is a Hamiltonian path that starts and ends at the same vertex.
 * 
 * Finding Hamiltonian paths or cycles is an NP-complete problem:
 *   - Solutions can be verified quickly (polynomial time), but finding them
 *     is computationally hard in the worst case.
 *   - Related NP-complete problems include the Traveling Salesman Problem, 
 *     Boolean Satisfiability (SAT), and Graph Coloring.
 * 
 * Methods:
 *   - simpleSolver():
 *       Brute-force recursive DFS that enumerates all Hamiltonian paths 
 *       and stores them, along with cycle information.
 *       Time complexity: O(n!) since it generates all possible paths.
 * 
 *   - pureSimpleSolver():
 *       DFS-based solver that counts all Hamiltonian paths without storing them.
 *       Useful for benchmarking and large graphs where storing paths is expensive.
 *       Time complexity: O(n!) as it explores all permutations of vertices.
 * 
 *   - dpSolver():
 *       Dynamic programming approach using bitmasking to count all Hamiltonian 
 *       paths while caching dead-end states to avoid redundant computations.
 *       This reduces redundant DFS calls compared to simpleSolver.
 *       Time complexity: O(n * 2^n), with space complexity O(n * 2^n) for the DP table.
 * 
 * Note:
 *   - simpleSolver and pureSimpleSolver are primarily educational and for verification.
 *   - dpSolver is recommended for counting paths efficiently on moderately sized graphs.
 *   - Graphs with more than ~20 vertices may be infeasible due to exponential growth.
 */

#include <iostream>
#include <vector>
#include "gtest/gtest.h"

class HamiltonSolver{
public:
    HamiltonSolver(int numOfVertices, const std::vector<std::pair<int, int>>& edges) : n{numOfVertices}, graph(n)
    {
        total = (1 << n) - 1; // all vertices visited mask
        for (auto [u, v] : edges) {
            graph[u].push_back(v);
            graph[v].push_back(u); // undirected graph
        }
    }    

    /**
     * @brief Brute-force DFS: finds and stores all Hamiltonian paths + cycle info
     */
    void simpleSolver()
    {
        std::vector<int> path;
        
        for (int u{}; u < n; ++u) {
            int visitedMask{};
            // Try every vertex as a starting point
            dfs(u, visitedMask, path, u/*starting node for cycle detection*/);
        }
    }

    /**
     * @brief DFS that only counts Hamiltonian paths (no storage).
     * Faster and memory-efficient; useful for benchmarking.
     */
    int pureSimpleSolver()
    {
        int numberOfPaths{};
        for (int u{}; u < n; ++u) {
            dfs(u, 0/*mask*/, numberOfPaths);
        }

        return numberOfPaths;
    }

    /**
     * @brief Bitmask DP solution: counts Hamiltonian paths with memoization.
     * Avoids recomputation of dead-end states.
     */
    int dpSolver()
    {
        std::vector<std::vector<int>> dp((1 << n), std::vector<int>(n, -1)); 
        // dp[mask][u] = number of Hamiltonian paths from state (mask, u)

        int numberOfPaths{};
        for (int u{}; u < n; ++u) {
            numberOfPaths += dfs(u, 0/*mask*/, dp);
        }

        return numberOfPaths;
    }

    const std::vector<std::vector<int>>& getPaths() const { return paths; }
    const std::vector<bool>& getCycles() const { return areCycles; }
    
private:
    /**
     * @brief DFS used by simpleSolver.
     * Stores all Hamiltonian paths and marks whether they form cycles.
     */
    void dfs(int u, int mask, std::vector<int>& path, int startingVertex)
    {
        mask |= (1 << u);
        path.push_back(u);

        if (mask == total) { // all vertices visited
            paths.push_back(path);

            // check if it's a cycle (last vertex connected to starting one)
            bool isCycle{false};
            for (int v : graph[u]) {
                if (v == startingVertex) {
                    isCycle = true;
                    break;
                }
            }
            areCycles.push_back(isCycle);

            path.pop_back();
            return;
        }

        // explore neighbors
        for (int v : graph[u]) {
            if ((mask & (1 << v)) == 0) { // not visited
                dfs(v, mask, path, startingVertex);
            }
        }

        path.pop_back();
    }

    /**
     * @brief DFS used by pureSimpleSolver.
     * Counts paths instead of storing them.
     */
    void dfs(int u, int mask, int& numberOfPaths)
    {
        mask |= (1 << u);

        if (mask == total) { // full path found
            ++numberOfPaths;
            return;
        }

        for (int v : graph[u]) {
            if ((mask & (1 << v)) == 0) {
                dfs(v, mask, numberOfPaths);
            }
        }
    }

    /**
     * @brief DFS + DP (bitmasking).
     * Returns number of Hamiltonian paths starting from (mask, u).
     */
    int dfs(int u, int mask, std::vector<std::vector<int>>& dp)
    {
        mask |= (1 << u);

        if (mask == total) return 1; // base case: valid Hamiltonian path
        if (dp[mask][u] != -1) return dp[mask][u]; // memoized

        int numberOfPaths{};
        for (int v : graph[u]) {
            if ((mask & (1 << v)) == 0) { // not visited
                numberOfPaths += dfs(v, mask, dp);
            }
        }

        return dp[mask][u] = numberOfPaths;
    }

private:
    int n{};                        // number of vertices
    int total{};                    // bitmask with all vertices visited
    std::vector<std::vector<int>> graph; // adjacency list
    std::vector<std::vector<int>> paths; // all Hamiltonian paths (simpleSolver)
    std::vector<bool> areCycles;         // cycle info for each path
};

// ---------------- Google Tests ------------------

TEST(HamiltonSolver, CompleteGraph4)
{
    int n{4};
    std::vector<std::pair<int, int>> edges{{0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2,3}};
    HamiltonSolver solver{n, edges};
    solver.simpleSolver();

    int numberOfPaths{solver.dpSolver()};
    EXPECT_EQ(solver.getPaths().size(), numberOfPaths);

    EXPECT_EQ(solver.getPaths().size(), 24); // 4! permutations
    for (auto c : solver.getCycles()) {
        EXPECT_TRUE(c); // complete graph => all cycles
    }
}

TEST(HamiltonSolver, CompleteGraph3)
{
    int n{3};
    std::vector<std::pair<int,int>> edges{{0, 1}, {0, 2}, {1, 2}};
    HamiltonSolver solver{n, edges};
    solver.simpleSolver();

    int numberOfPaths{solver.dpSolver()};
    EXPECT_EQ(solver.getPaths().size(), numberOfPaths);

    EXPECT_EQ(solver.getPaths().size(), 6); // 3! permutations
    for (auto c : solver.getCycles()) EXPECT_TRUE(c);
}

TEST(HamiltonSolver, LineGraph5)
{
    int n{5};
    std::vector<std::pair<int,int>> edges{{0, 1}, {1, 2}, {2, 3}, {3, 4}};
    HamiltonSolver solver{n, edges};
    solver.simpleSolver();

    int numberOfPaths{solver.dpSolver()};
    EXPECT_EQ(solver.getPaths().size(), numberOfPaths);

    EXPECT_EQ(solver.getPaths().size(), 1 * 2); // only 2 valid Hamiltonian paths
    for (auto c : solver.getCycles()) EXPECT_FALSE(c); // no cycles
}

TEST(HamiltonSolver, SingleCycle)
{
    int n{4};
    std::vector<std::pair<int, int>> edges{{0, 1}, {1, 2}, {2, 3}, {3, 0}};
    HamiltonSolver solver{n, edges};
    solver.simpleSolver();

    int numberOfPaths{solver.dpSolver()};
    EXPECT_EQ(solver.getPaths().size(), numberOfPaths);

    EXPECT_EQ(solver.getPaths().size(), n * 2); // each start has 2 orientations

    int numberOfCycles{};
    for (auto c : solver.getCycles()) {
        if (c) ++numberOfCycles;
    }

    EXPECT_EQ(numberOfCycles, n * 2); // all paths are cycles here
}

TEST(HamiltonSolver, BenchmarkDPvsSimple)
{
    int n{12}; // bigger graph to see performance difference
    std::vector<std::pair<int,int>> edges{ {0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}, {0,8}, {0,9}, {0,10}, {0,11}, {1,2}, {1,3}, {1,4}, {1,5}, {1,6}, {1,7}, {1,8}, {1,9}, {1,10}, {1,11}, {2,3}, {2,4}, {2,5}, {2,6}, {2,7}, {2,8}, {2,9}, {2,10}, {2,11}, {3,4}, {3,5}, {3,6}, {3,7}, {3,8}, {3,9}, {3,10}, {3,11}, {4,5}, {4,6}, {4,7}, {4,8}, {4,9}, {4,10}, {4,11}, {5,6}, {5,7}, {5,8}, {5,9}, {5,10}, {5,11}, {6,7}, {6,8}, {6,9}, {6,10}, {6,11}, {7,8}, {7,9}, {7,10}, {7,11}, {8,9}, {8,10}, {8,11}, {9,10}, {9,11}, {10,11} };
    HamiltonSolver solver{n, edges};

    auto start1 = std::chrono::high_resolution_clock::now();
    int paths1{solver.pureSimpleSolver()};
    auto end1 = std::chrono::high_resolution_clock::now();

    auto start2 = std::chrono::high_resolution_clock::now();
    int paths2{solver.dpSolver()};
    auto end2 = std::chrono::high_resolution_clock::now();

    EXPECT_EQ(paths1, paths2);

    std::cout << "pureSimpleSolver: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count()
              << " ms\n";

    std::cout << "dpSolver: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count()
              << " ms\n";
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
