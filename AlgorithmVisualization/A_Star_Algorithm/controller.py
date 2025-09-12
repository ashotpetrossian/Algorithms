import threading
from gui import GridGUI
import solver as PathSolver

from typing import Optional, Generator, Tuple

class Controller:
    gui: GridGUI
    grid: Optional[list[list[int]]]
    solver: Optional[PathSolver.Solver]
    initial_robot_position: Optional[Tuple[int, int]]
    
    def __call__(self):
        self.gui = GridGUI()
        self.grid = None
        
        self.gui.set_start_callback(self.get_grid_and_run)

        self.gui.update_status("Please start initializing obstacles...", "blue")
        self.gui.run()

    def get_grid_and_run(self, solver_type: str) -> None:
        print("Grid is ready! Getting grid and starting A*...")
        self.grid = self.gui.get_grid()
        self.start_solving(solver_type)

    def start_solving(self, solver_type: str):
        if self.grid is None:
            raise ValueError("Grid not initialized")
        
        self.initial_robot_position = self.gui.robot_position
        if self.initial_robot_position is None:
            raise ValueError("Robot starting position not set")
        
        if solver_type == "astar":
            self.solver = PathSolver.AStarSolver(self.grid)
        elif solver_type == "dijkstra":
            self.solver = PathSolver.DijkstraSolver(self.grid)
        else:
            raise ValueError(f"Unknown solver type: {solver_type}")

        gen = self.solver.solve()
        self.run_solver_with_delay(gen)  # delay in milliseconds


    def run_solver_with_delay(self, gen, delay=50):
        try:
            result = next(gen)
            if result is None:
                self.gui.update_status("No path found.", "red")
                return
            
            x, y = result
            self.gui.move_robot(x, y)
            self.gui.root.after(delay, lambda: self.run_solver_with_delay(gen, delay))
        except StopIteration as e:
            result = e.value
            if result:
                print("✅ Path found and solved.")
                if self.solver is None or self.solver.shortest_distance is None:
                    raise RuntimeError("Solver is not initialized or finished without setting shortest_distance — this should never happen")
                self.gui.update_status(f"Path found with cost: {self.solver.shortest_distance}!", "green")
                self.reconstruct_path()
            else:
                print("❌ No path found.")
                self.gui.update_status("No path found!", "red")
                return
                
    def reconstruct_path(self):
        print("Starting path reconstruction")
        if self.gui.endpoint_position is None:
            raise ValueError("Endpoint position not set")
        x, y = self.gui.endpoint_position
        path = []

        while (x, y) != self.initial_robot_position:
            if self.solver is None:
                raise RuntimeError("Solver is not initialized — cannot access parents")
            parent = self.solver.parents[x][y]
            if parent is None:
                print("🚨 No path found during reconstruction!")
                return
            path.append((x, y))
            x, y = parent
        
        path.append(self.initial_robot_position)
        
        delay = 50
        
        def draw_step(index):
            if index >= (len(path)):
                return
            
            x, y = path[index]
            nonlocal delay

            if (x, y) != self.gui.robot_position and (x, y) != self.gui.endpoint_position:
                self.gui.canvas.itemconfig(self.gui.cell_rects[x][y], fill='purple')
            self.gui.root.after(delay, lambda index = index + 1: draw_step(index))

        draw_step(0)
        
        
        self.gui.astar_button.config(state='disabled')
        self.gui.dijkstra_button.config(state='disabled')


