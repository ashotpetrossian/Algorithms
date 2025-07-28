import threading
from gui import GridGUI
from solver import AStarSolver

class Controller:
    def __call__(self):
        self.gui = GridGUI()
        self.grid = None
        
        self.gui.set_start_callback(self.get_grid_and_run)

        self.gui.update_status("Please start initializing obstacles...", "blue")
        self.gui.run()

    def get_grid_and_run(self):
        print("Grid is ready! Getting grid and starting A*...")
        self.grid = self.gui.get_grid()
        self.start_solving()

    def start_solving(self):
        self.initial_robot_position = self.gui.robot_position
        self.solver = AStarSolver(self.grid)
        gen = self.solver.solve()

        self.run_solver_with_delay(gen)  # delay in milliseconds

    def run_solver_with_delay(self, gen, delay=300):
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
                self.gui.update_status(f"Path found with cost: {self.solver.shortest_distance}!", "green")
                self.reconstruct_path()
            else:
                print("❌ No path found.")
                self.gui.update_status("No path found!", "red")
                return
                
    def reconstruct_path(self):
        print("Starting path reconstruction")
        x, y = self.gui.endpoint_position
        path = []

        while (x, y) != self.initial_robot_position:
            parent = self.solver.parents[x][y]
            if parent is None:
                print("🚨 No path found during reconstruction!")
                return
            path.append((x, y))
            x, y = parent
        
        delay = 300
        
        def draw_step(index):
            if index == (len(path)):
                return
            
            x, y = path[index]
            nonlocal delay;

            if (x, y) != self.gui.robot_position and (x, y) != self.gui.endpoint_position:
                self.gui.canvas.itemconfig(self.gui.cell_rects[x][y], fill='purple')
            self.gui.root.after(delay, lambda: draw_step(index + 1))

        draw_step(0)
        
        self.gui.run_button.config(state='disabled')


