#include <vector>
#include <queue>
#include <iostream>
#include <limits>

/*
* Dijkstra's algorithm is used to find the shortest path from a source node to all other nodes in a weighted graph with non-negative edges.
* It uses a greedy approach and priority queue (min-heap) to always expand the shortest known path first.
* 
* Algorithm Steps
* Initialize distances:
* Set all nodes' distances to infinity (std::numeric_limits<int>::max()).
* Set the source node’s distance to 0 (since the shortest path to itself is 0).
* Use a priority queue (min-heap) to store nodes in the format (distance, vertex).
*
* Process nodes:
* Extract the node with the smallest known distance from the priority queue.
* For each of its neighbors, update their distance if the current path is shorter.
* If a shorter path is found, push (new_distance, neighbor) into the priority queue.
* Repeat until all nodes are processed.
*/

class Dijkstra
{
public:
    Dijkstra(const std::vector<std::vector<std::pair<int, int>>>& graph) : m_graph{ graph }, n(graph.size())
    { }

    void solve(int sourceVertex)
    {
        std::vector<int> dist(n, std::numeric_limits<int>::max());
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;
        pq.push(std::make_pair(0, sourceVertex));
        dist[sourceVertex] = 0;

        while (!pq.empty()) {
            auto [cost, u] = pq.top();
            pq.pop();

            if (cost > dist[u]) continue;

            for (auto [v, w] : m_graph[u]) {
                if (dist[v] > cost + w) {
                    dist[v] = cost + w;
                    pq.push(std::make_pair(dist[v], v));
                }
            }
        }

        for (int i : dist) std::cout << i << " ";
        std::cout << std::endl;
    }

private:
    std::vector<std::vector<std::pair<int, int>>> m_graph;
    int n{};
};

int main()
{
    // Cormen's example, chapter 24
    Dijkstra d{std::vector<std::vector<std::pair<int, int>>>
                {
                std::vector<std::pair<int, int>>{{1, 3}, {2, 5}},
                std::vector<std::pair<int, int>>{{3, 6}, {2, 2}},
                std::vector<std::pair<int, int>>{{1, 1}, {4, 6}, {3, 4}},
                std::vector<std::pair<int, int>>{{4, 2}},
                std::vector<std::pair<int, int>>{{3, 7}, {0, 3}}
                }
            };
    d.solve(0);
}