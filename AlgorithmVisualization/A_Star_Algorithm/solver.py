from abc import ABC, abstractmethod
from copy import deepcopy
import math
import heapq
from typing import Tuple, Generator, Optional

class Solver(ABC):
    def __init__(self, grid: list[list[int]]):
        self.grid = deepcopy(grid)
        self.n = len(grid)
        self.m = len(grid[0])        
        self.shortest_distance: Optional[int] = None
        
        self._init_robot_and_endpoint_positions()
        
        print(f'after init robot position: {self.robot_row} - {self.robot_col}, endpoint: {self.endpoint_row} - {self.endpoint_col}')
        
        self.dist = [[math.inf] * self.m for _ in range(self.n)]
        self.parents = [[None] * self.m for _ in range(self.n)]
    
    def _init_robot_and_endpoint_positions(self) -> None:
        for i in range(self.n):
            for j in range(self.m):
                if self.grid[i][j] == 2:
                    self.robot_row, self.robot_col = i, j
                elif self.grid[i][j] == 3:
                    self.endpoint_row, self.endpoint_col = i, j
                    
    @abstractmethod
    def solve(self) -> 'Generator[tuple[int, int], None, bool]':
        """
        Generator-based A*/Dijkstra solver.
        
        Yields:
            (x, y): Current cell being processed.
        
        Returns:
            bool: True if path found, False otherwise.
        """
        pass


class AStarSolver(Solver):
    def get_heuristic_value(self, x, y)  -> int:
        return abs(x - self.endpoint_row) + abs(y - self.endpoint_col)

    def solve(self) -> 'Generator[tuple[int, int], None, bool]':
        f = lambda x, y: self.dist[x][y] + self.get_heuristic_value(x, y)
        pq = []
        
        self.dist[self.robot_row][self.robot_col] = 0
        heapq.heappush(pq, (f(self.robot_row, self.robot_col), self.robot_row, self.robot_col))        
        
        while pq:
            f_value, x, y = heapq.heappop(pq)
            if f_value > self.dist[x][y] + self.get_heuristic_value(x, y):
                continue

            yield (x, y)
            
            if x == self.endpoint_row and y == self.endpoint_col:
                self.shortest_distance = self.dist[x][y]
                return True
            
            for dx, dy in [(1, 0), (0, 1), (-1, 0), (0, -1)]:
                xx = x + dx
                yy = y + dy
                
                if xx < 0 or yy < 0 or xx >= self.n or yy >= self.m or self.grid[xx][yy] == 1:
                    continue
                
                tentative_g = self.dist[x][y] + 1
                
                if tentative_g < self.dist[xx][yy]:
                    self.dist[xx][yy] = tentative_g
                    self.parents[xx][yy] = (x, y)
                    heapq.heappush(pq, (f(xx, yy), xx, yy))
                    
        return False
                
                    
class DijkstraSolver(Solver):
    def solve(self) -> 'Generator[tuple[int, int], None, bool]':
        pq = []
        
        self.dist[self.robot_row][self.robot_col] = 0
        heapq.heappush(pq, (0, self.robot_row, self.robot_col))        
        
        while pq:
            cost, x, y = heapq.heappop(pq)
            
            yield (x, y)
            
            if x == self.endpoint_row and y == self.endpoint_col:
                self.shortest_distance = self.dist[x][y]
                return True
            
            if cost > self.dist[x][y]:
                continue
            
            for dx, dy in [(1, 0), (0, 1), (-1, 0), (0, -1)]:
                xx = x + dx
                yy = y + dy
                
                if xx < 0 or yy < 0 or xx >= self.n or yy >= self.m or self.grid[xx][yy] == 1:
                    continue
                
                if cost + 1 < self.dist[xx][yy]:
                    self.dist[xx][yy] = cost + 1
                    self.parents[xx][yy] = (x, y)
                    heapq.heappush(pq, (self.dist[xx][yy], xx, yy))
                    
        return False