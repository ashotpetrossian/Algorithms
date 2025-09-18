/**
 * @brief Implementation of the A* Search Algorithm for shortest path finding on a weighted graph.
 *
 * A* (A star) is a popular graph traversal and pathfinding algorithm that combines features of
 * Dijkstra's algorithm and Best-First Search by using heuristics to efficiently find the shortest path.
 *
 * Key concepts:
 * - g(n): Exact cost from the start node to node n.
 * - h(n): Heuristic estimate of the cost from node n to the goal node. It must be admissible
 *         (never overestimate) and preferably consistent (monotonic).
 * - f(n) = g(n) + h(n): Estimated total cost of the cheapest solution through node n.
 *
 * Heuristic:
 * - This implementation uses the Manhattan distance as the heuristic function, suitable for
 *   grid-based maps where movement is allowed horizontally and vertically.
 * - In the original A* algorithm, any admissible heuristic appropriate to the problem domain
 *   can be used.
 *
 * Algorithm flow:
 * 1. Initialize the priority queue with the start node, setting g(start) = 0.
 * 2. While the priority queue is not empty:
 *    a. Pop the node u with the lowest f(u) = g(u) + h(u).
 *    b. If u is the destination node, the shortest path is found.
 *    c. Mark u as closed (fully explored).
 *    d. For each neighbor v of u:
 *       - Ignore if v is closed.
 *       - Calculate tentative_g = g(u) + cost(u, v).
 *       - If tentative_g < g(v), update g(v), set parent[v] = u,
 *         and push v into the priority queue.
 * 3. If the priority queue becomes empty and the goal was not reached, no path exists.
 *
 * Time complexity:
 * - Worst-case between O(b^d) and O(E log V):
 *   - b = average branching factor (number of neighbors per node).
 *   - d = solution depth (number of edges along the shortest path from start to goal).
 * - In practice, A* explores far fewer nodes than O(b^d) due to heuristic pruning.
 * - The efficiency heavily depends on the heuristic quality: better heuristics reduce the explored nodes.
 *
 * Space complexity:
 * - O(V) for storing distances, parents, heuristic values, and closed nodes.
 *
 * Implementation details:
 * - The graph is represented as an adjacency list with weighted edges.
 * - Heuristic values are precomputed using Manhattan distance relative to the destination coordinates.
 * - A priority queue manages nodes ordered by their f-score (estimated total cost).
 * - Closed nodes are tracked in a boolean vector.
 * - Path reconstruction follows parent pointers from destination to source after completion.
 */



#include <vector>
#include <iostream>
#include <exception>
#include <queue>
#include <algorithm>
#include <limits>
#include <unordered_set>

class AStarSolver
{
public:
    AStarSolver(const std::vector<std::vector<int>> &grid, const std::vector<std::tuple<int, int, int>>& edges, int src, int dst, int numberOfVertices)
        : source{src}, destination{dst}, V{numberOfVertices}
    {
        if (V <= 0) throw std::invalid_argument("Number of Vertices should be > 0");
        graph.assign(V, {});
        heuristicValues.assign(V, 0);

        for (auto [u, v, w] : edges) {
            if (u < 0 || u >= V || v < 0 || v >= V) {
                throw std::out_of_range("Edge contains node outside [0,V)");
            }
            if (w < 0) {
                throw std::invalid_argument("Negative edge weights are not supported by A*");
            }

            graph[u].push_back(std::make_pair(v, w));
            graph[v].push_back(std::make_pair(u, w));
        }

        initHeuristicValues(grid);
    }

    bool solve()
    {
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
        
        std::vector<bool> closed(V);

        std::vector<int> dist(V, inf);
        parent.assign(V, -1);

        // f(n) = g(n) + h(n)
        auto f = [this, &dist](int node) {
            return dist[node] + heuristicValues[node];
        };

        dist[source] = 0;
        pq.push(std::make_pair(f(source), source));

        while (!pq.empty()) {
            // popping all explored nodes
            while (!pq.empty() && closed[pq.top().second] == true) pq.pop();

            if (pq.empty()) break;

            auto [f_score, u] = pq.top();
            pq.pop();

            if (u == destination) {
                std::cout << "Destination found\n";
                shortestDistance = dist[destination];
                return true;
            }

            closed[u] = true;

            for (auto [v, w] : graph[u])  {
                if (closed[v] == true) continue;

                int tentativeGValue = dist[u] + w;
                if (tentativeGValue < dist[v]) {
                    dist[v] = tentativeGValue;
                    parent[v] = u;

                    pq.push(std::make_pair(f(v), v));
                }
            }
        }

        std::cout << "Destination cannot be found\n";
        return false;
    }

    int getShortestPath()
    {
        if (shortestDistance == -1) solve();
        return shortestDistance;
    }

    std::vector<int> reconstructPath()
    {
        path.clear();
        if (shortestDistance == -1)
        {
            throw std::logic_error("You cannot reconstruct the path as the shortest path has not been found yet");
        }

        int u{destination};
        while (u != source)
        {
            path.push_back(u);
            u = parent[u];
        }

        path.push_back(u);
        std::reverse(path.begin(), path.end());
        return path;
    }

    std::pair<int, int> getCoordinates(const std::vector<std::vector<int>> &grid, int node)
    {
        for (int i{}; i < grid.size(); ++i)
        {
            for (int j{}; j < grid[0].size(); ++j)
            {
                if (grid[i][j] == node) return std::make_pair(i, j);
            }
        }

        throw std::invalid_argument("The destination node cannot be found in the grid.");
    }

    int getManhattenDistance(int x1, int y1, int x2, int y2) const noexcept
    {
        return std::abs(x1 - x2) + std::abs(y1 - y2);
    }

    void initHeuristicValues(const std::vector<std::vector<int>> &grid)
    {
        auto [x, y] = getCoordinates(grid, destination);

        // caching all heuristic values
        for (int i{}; i < grid.size(); ++i) {
            for (int j{}; j < grid[0].size(); ++j) {
                int u{grid[i][j]};
                if (u != -1) {
                    heuristicValues[u] = getManhattenDistance(i, j, x, y);
                }
            }
        }
    }

private:
    std::vector<std::vector<std::pair<int, int>>> graph;
    std::vector<int> heuristicValues;
    int source{};
    int destination{};
    int V{};

    int shortestDistance{-1};
    std::vector<int> parent;
    std::vector<int> path; // for reconstruction

    const int inf{std::numeric_limits<int>::max()};
};

void test1()
{
    std::vector<std::tuple<int, int, int>> edges {
        {0, 1, 5},
        {0, 2, 5},
        {1, 2, 4},
        {1, 3, 3},
        {2, 3, 7},
        {2, 4, 7},
        {4, 5, 4},
        {5, 6, 9},
        {2, 7, 8},
        {4, 7, 5},
        {7, 8, 3},
        {8, 9, 4},
        {9, 13, 3},
        {6, 13, 12},
        {3, 12, 14},
        {3, 11, 13},
        {3, 10, 16},
        {10, 11, 5},
        {11, 12, 9},
        {11, 14, 4},
        {12, 14, 5},
        {10, 15, 4},
        {10, 13, 7},
        {9, 15, 8},
        {13, 15, 7}
    };

    std::vector<std::vector<int>> grid {
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 12, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, 3,  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14, -1},
        {-1, -1,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 11, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        { 0, -1, -1, 2,  -1, -1, -1, -1, -1, -1, -1, -1, 10, -1, -1, -1, 15},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, 7,  -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, 8,  -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, 4,  -1, -1, -1, -1, -1, -1, -1, 9,  -1, -1, -1, -1},
        {-1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 13, -1, -1},
        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, 6,  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}
    };

    int source{0}, destination{15};
    int numberOfNodes{16};

    AStarSolver solver{grid, edges, source, destination, numberOfNodes};
    int shortestDistance = solver.getShortestPath();

    std::cout << source << " -> " << destination << " = " << shortestDistance << std::endl;
    auto path = solver.reconstructPath();
    for (int i : path) std::cout << i << " ";
    std::cout << std::endl;
}

int main()
{
    test1();
}