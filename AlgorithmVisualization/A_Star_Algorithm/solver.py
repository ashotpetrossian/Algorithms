from copy import deepcopy
import math
import heapq


class AStarSolver:
    def __init__(self, grid):
        self.grid = deepcopy(grid)
        
        self.n = len(grid)
        self.m = len(grid[0])
        
        self.shortest_distance = None
        
        self._init_robot_and_endpoint_positions()
        
        print(f'after init robot position: {self.robot_row} - {self.robot_col}, endpoint: {self.endpoint_row} - {self.endpoint_col}')
        
        self.dist = [[math.inf] * self.m for _ in range(self.n)]
        self.closed = [[False] * self.m for _ in range(self.n)]
        self.parents = [[None] * self.m for _ in range(self.n)]
        self.open = set()
        
    def _init_robot_and_endpoint_positions(self):
        for i in range(self.n):
            for j in range(self.m):
                if self.grid[i][j] == 2:
                    self.robot_row, self.robot_col = i, j
                elif self.grid[i][j] == 3:
                    self.endpoint_row, self.endpoint_col = i, j

    def get_heuristic_value(self, x, y):
        return abs(x - self.endpoint_row) + abs(y - self.endpoint_col)

    def solve(self):
        f = lambda x, y: self.dist[x][y] + self.get_heuristic_value(x, y)

        pq = []
        
        heapq.heappush(pq, (f(self.robot_row, self.robot_col), self.robot_row, self.robot_col))
        
        self.open.add((self.robot_row, self.robot_col))
        self.dist[self.robot_row][self.robot_col] = 0
        
        while self.open:
            while pq and self.closed[pq[0][1]][pq[0][2]]:
                heapq.heappop(pq)
            
            if not pq:
                return False
                
            f_value, x, y = heapq.heappop(pq)
            self.open.remove((x, y))
            
            yield (x, y)
            
            if x == self.endpoint_row and y == self.endpoint_col:
                self.shortest_distance = self.dist[x][y]
                return True
            
            self.closed[x][y] = True
            
            for dx, dy in [(1, 0), (0, 1), (-1, 0), (0, -1)]:
                xx = x + dx
                yy = y + dy
                
                if xx < 0 or yy < 0 or xx >= self.n or yy >= self.m or self.closed[xx][yy] or self.grid[xx][yy] == 1:
                    continue
                
                tentative_g = self.dist[x][y] + 1
                
                if tentative_g < self.dist[xx][yy]:
                    self.dist[xx][yy] = tentative_g
                    self.parents[xx][yy] = x, y
                    self.open.add((xx, yy))
                    heapq.heappush(pq, (f(xx, yy), xx, yy))
                
                    
