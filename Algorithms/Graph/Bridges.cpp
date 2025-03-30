#include <vector>
#include <iostream>

class BridgeSolver
{
public:
    BridgeSolver(const std::vector<std::vector<int>>& adjList) : graph{ adjList }, n{ adjList.size() }
    {
        parent.resize(n, -1);
        lowLink.resize(n);
        times.resize(n, -1);
    }

    std::vector<std::pair<int, int>> solve()
    {
        for (int u{}; u < n; ++u) {
            if (times[u] == -1) {
                dfs(u);
            }
        }

        return bridges;
    }

    void dfs(int u)
    {
        times[u] = lowLink[u] = time++;

        for (int v : graph[u]) {
            if (times[v] == -1) {
                parent[v] = u;

                dfs(v);

                lowLink[u] = std::min(lowLink[u], lowLink[v]);
                // if no back edge
                if (lowLink[v] > times[u]) {
                    bridges.push_back({u, v});
                }
            } else if (v != parent[u]) {
                lowLink[u] = std::min(lowLink[u], times[v]);
            }
        }
    }
    
private:
    std::vector<std::vector<int>> graph;
    std::vector<std::pair<int, int>> bridges;
    std::vector<int> parent;
    std::vector<int> lowLink;
    std::vector<int> times;
    int time{};
    size_t n{};
};

void test1()
{
    std::vector<std::vector<int>> adjList {
        {1, 5}, 
        {0, 2, 3}, 
        {1, 3, 4},
        {1, 2, 4},
        {2, 3}, 
        {0}
    };

    BridgeSolver solver{adjList};
    std::vector<std::pair<int, int>> bridges{solver.solve()};

    if (bridges.empty()) {
        std::cout << "There are no bridges" << std::endl;
    } else {
        for (auto [u, v] : bridges) std::cout << u << " - " << v << ", ";
        std::cout << std::endl;
    }
}

void test2()
{
    std::vector<std::vector<int>> adjList {
        {1, 3},
        {0, 2, 3},
        {1, 4, 5},
        {0, 1, 5},
        {2, 5},
        {2, 3, 4, 6},
        {5}
    };

    BridgeSolver solver{adjList};
    std::vector<std::pair<int, int>> bridges{solver.solve()};

    if (bridges.empty()) {
        std::cout << "There are no bridges" << std::endl;
    } else {
        for (auto [u, v] : bridges) std::cout << u << " - " << v << ", ";
        std::cout << std::endl;
    }
}

void test3()
{
    std::vector<std::vector<int>> adjList {
        {1, 2},
        {0, 2},
        {0, 1, 3},
        {2, 4},
        {3}
    };

    BridgeSolver solver{adjList};
    std::vector<std::pair<int, int>> bridges{solver.solve()};

    if (bridges.empty()) {
        std::cout << "There are no bridges" << std::endl;
    } else {
        for (auto [u, v] : bridges) std::cout << u << " - " << v << ", ";
        std::cout << std::endl;
    }
}

int main()
{
    std::cout << "Test 1\n";
    test1();
    std::cout << "Test 2\n";
    test2();
    std::cout << "Test 3\n";
    test3();
}