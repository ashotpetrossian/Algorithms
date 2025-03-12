#include <vector>
#include <iostream>

class ArticulationPointSolver
{
public:
    ArticulationPointSolver(const std::vector<std::vector<int>>& adjList) : graph{ adjList }, n { adjList.size() }
    {
        parent.resize(n, -1);
        lowLink.resize(n);
        times.resize(n, -1);
    }

    std::vector<int> solve()
    {
        for (int u{}; u < n; ++u) {
            if (times[u] == -1) {
                dfs(u);
            }
        }

        return articulationPoints;
    }

    void dfs(int u)
    {
        times[u] = lowLink[u] = time++;
        int childrenCount{};

        for (int v : graph[u]) {
            if (times[v] == -1) {
                parent[v] = u;
                ++childrenCount;

                dfs(v);

                lowLink[u] = std::min(lowLink[u], lowLink[v]);

                // case 1: source is the dfs's root
                if (parent[u] == -1 && childrenCount > 1) {
                    articulationPoints.push_back(u);
                } else if (parent[u] != -1 && lowLink[v] >= times[u]) { // case 2: no back edge from child to any ancestor
                    articulationPoints.push_back(u);
                }
            } else if (v != parent[u]) {
                lowLink[u] = std::min(lowLink[u], times[v]);
            }
        }
    }
    
private:
    std::vector<std::vector<int>> graph;
    std::vector<int> articulationPoints;
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

    ArticulationPointSolver solver{adjList};
    std::vector<int> points{solver.solve()};

    if (points.empty()) {
        std::cout << "There are no articulation points" << std::endl;
    } else {
        for (int point : points) std::cout << point << " ";
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

    ArticulationPointSolver solver{adjList};
    std::vector<int> points{solver.solve()};
    
    if (points.empty()) {
        std::cout << "There are no articulation points" << std::endl;
    } else {
        for (int point : points) std::cout << point << " ";
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

    ArticulationPointSolver solver{adjList};
    std::vector<int> points{solver.solve()};
    
    if (points.empty()) {
        std::cout << "There are no articulation points" << std::endl;
    } else {
        for (int point : points) std::cout << point << " ";
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