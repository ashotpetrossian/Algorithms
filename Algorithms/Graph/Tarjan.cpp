#include <vector>
#include <iostream>
#include <stack>

/*
* A Strongly Connected Component (SCC) in a directed graph is a maximal set of nodes where every node is reachable from every other node in that SCC.
* 
* Tarjan’s algorithm is a linear-time (O(V + E)) algorithm that efficiently finds SCCs using Depth-First Search (DFS) and low-link values.
* 
* The low-link value of a node u represents the lowest (earliest) discovery time of any node that can be reached from u,
* including itself and its descendants in the DFS tree.
* 
* Formally: 𝑙𝑜𝑤𝐿𝑖𝑛𝑘[𝑢]=min⁡(discovery time of 𝑢, discovery time of any ancestor reachable from 𝑢)
* Each node is assigned a unique discovery time when visited. ids[u] is set to a running counter id++, ensuring unique timestamps.
* 
* Note: for "another implementation" - need to find a test case where one line update is not working - who finds, ping me.
*/

class Tarjan
{
public:
	Tarjan(const std::vector<std::vector<int>>& graph) : m_graph{ graph }, n( graph.size() ), ids(n, -1), lowLink(n), onStack(n, false)
	{
		solve();
	}

	const std::vector<std::vector<int>>& getSCC() const { return sccs; }

	void printSCC()
	{
		std::cout << "Low Link values\n";
		for (int i : lowLink) std::cout << i << " ";
		std::cout << std::endl;

		std::cout << "The SCCs\n";
		for (const std::vector<int>& vec : sccs) {
			for (int i : vec) std::cout << i << " ";
			std::cout << std::endl;
		}
	}
private:
	void solve()
	{
		for (int u{}; u < n; ++u) {
			if (ids[u] == -1) {
				dfs(u);
			}
		}
	}

	void dfs(int u)
	{
		ids[u] = lowLink[u] = id++;
		onStack[u] = true;
		st.push(u);

		for (int v : m_graph[u]) {
			if (ids[v] == -1) {
				dfs(v);
				//lowLink[u] = std::min(lowLink[u], lowLink[v]); // another implementation
			}

			// back edge
			if (onStack[v]) {
				lowLink[u] = std::min(lowLink[u], lowLink[v]);
				//lowLink[u] = std::min(lowLink[u], ids[v]); // another implementation
			}
		}

		if (ids[u] == lowLink[u]) {
			std::vector<int> scc;
			for (int top{ st.top() }; ; top = st.top()) {
				scc.push_back(top);
				st.pop();
				onStack[top] = false;

				if (top == u) break;
			}

			sccs.push_back(scc);
		}

	}
private:
	std::vector<std::vector<int>> m_graph;
	int n{};
	std::vector<int> ids, lowLink;
	std::vector<bool> onStack;
	std::stack<int> st;
	

	std::vector<std::vector<int>> sccs;
	int id{};
};

int main()
{
	std::vector<std::vector<int>> graph{ {1}, {2}, {3, 5}, {0}, {1}, {4} };
	Tarjan t{ graph };
	t.printSCC();
}