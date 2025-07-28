import tkinter as tk
import threading

class GridGUI:
    def __init__(self):
        self.root = tk.Tk()

        self.set_title()
        self.create_canvas()
        self.set_coordinates()
        self.grid = [[0 for _ in range(self.cols)] for _ in range(self.rows)]
        self.cell_rects = [[None for _ in range(self.cols)] for _ in range(self.rows)]

        self.draw_rectangle()
        self.create_buttons_and_frame()
        self.create_status_label()
        self.create_algorithm_flow_label()

        self.obstacle_init_active = False
        self.robot_init_complete = False
        self.robot_position = None

        self.endpoint_init_active = False
        self.endpoint_position = None

        self.root.grid_rowconfigure(1, weight=1)
        self.root.grid_columnconfigure(0, weight=1)
        self.canvas.bind("<Button-1>", self.on_click)

    def set_title(self):
        self.root.title("A* Algorithm visualizer")

    def create_status_label(self):
        self.status_label = tk.Label(self.root, text="",
                                     font=("Segoe UI", 14), bg="white", fg="blue", anchor="w")
        self.status_label.grid(row=0, column=0, sticky='nw', padx=30, pady=(15, 10))

    def create_algorithm_flow_label(self):
        self.algorithm_flow_label = tk.Label(
            self.root,
            text="",
            font=("Segoe UI", 12),
            fg="black",
            bg="white",
            anchor="w"
        )
        self.algorithm_flow_label.grid(row=0, column=0, sticky='sew', padx=30, pady=(80, 20))
        
    def create_canvas(self):
        self.canvas = tk.Canvas(self.root, width=1200, height=800, bg='white')
        self.canvas.grid(row=1, column=0)

    def update_status(self, text, color="blue"):
        self.status_label.config(text=text, fg=color)

    def update_algorithm_flow(self, text):
        self.algorithm_flow_label.config(text=text)

    def set_coordinates(self):
        self.start_x, self.start_y = 100, 100
        self.end_x, self.end_y = self.start_x + 700, self.start_y + 1000

        self.cell_width = 30
        self.cell_height = 30

        self.rows, self.cols = 20, 30
        
    def draw_rectangle(self):
        for i in range(self.rows):
            for j in range(self.cols):
                x1 = self.start_x + j * self.cell_width
                y1 = self.start_y + i * self.cell_height
                x2 = x1 + self.cell_width
                y2 = y1 + self.cell_height
                rect = self.canvas.create_rectangle(x1, y1, x2, y2, outline="black", fill="white")
                self.cell_rects[i][j] = rect


    def start_obstacle_init(self):
        self.update_status("Click on the grid to mark obstacle cells. Click again to unmark.", "green")
        self.obstacle_init_active = True
        self.start_button.config(state='disabled')
        self.finish_button.config(state='normal')

    def finish_obstacle_init(self):
        self.obstacle_init_active = False
        self.robot_init_complete = True
        self.update_status("Obstacle initialization complete. Now click on the robot's start cell.", "purple")
        self.finish_button.config(state='disabled')

    def create_buttons_and_frame(self):
        self.button_frame = tk.Frame(self.root)
        self.button_frame.grid(row=1, column=1, sticky='n', padx=20, pady=20)

        button_font = ("Segoe UI", 14, "bold")
        self.start_button = tk.Button(
            self.button_frame,
            text="Start initializing obstacles",
            command=self.start_obstacle_init,
            font=button_font,
            relief="raised",
            bd=2,
            padx=10,
            pady=5,
            bg="#f0f0f0",
            activebackground="#d9d9d9",
        )
        self.start_button.pack(pady=8, fill='x')

        self.finish_button = tk.Button(
            self.button_frame,
            text="Finish initializing obstacles",
            command=self.finish_obstacle_init,
            font=button_font,
            relief="raised",
            bd=2,
            padx=10,
            pady=5,
            bg="#f0f0f0",
            activebackground="#d9d9d9",
        )
        self.finish_button.pack(pady=8, fill='x')

        self.run_button = tk.Button(
            self.button_frame,
            text="Start",
            command=self.start_pathfinding,
            font=button_font,
            relief="raised",
            bd=2,
            padx=10,
            pady=5,
            bg="#f0f0f0",
            activebackground="#d9d9d9",
            state='disabled'
        )
        self.run_button.pack(pady=20, fill='x')

    
    def on_click(self, event):
        col = (event.x - self.start_x) // self.cell_width
        row = (event.y - self.start_y) // self.cell_height

        if 0 <= row < self.rows and 0 <= col < self.cols:
            if self.obstacle_init_active:
                current = self.grid[row][col]
                if current == 0:
                    self.grid[row][col] = 1
                    self.canvas.itemconfig(self.cell_rects[row][col], fill="gray")
                else:
                    self.grid[row][col] = 0
                    self.canvas.itemconfig(self.cell_rects[row][col], fill="white")
            elif self.robot_init_complete:
                if self.grid[row][col] == 0:
                    self.robot_position = (row, col)
                    self.grid[row][col] = 2
                    self.canvas.itemconfig(self.cell_rects[row][col], fill='green')
                    self.update_status("Robot starting point set. Please set the endpoint.", "purple")
                    self.robot_init_complete = False
                    self.endpoint_init_active = True
            elif self.endpoint_init_active and (row, col) != self.robot_position:
                if self.grid[row][col] == 0:
                    self.endpoint_position = (row, col)
                    self.grid[row][col] = 3
                    self.canvas.itemconfig(self.cell_rects[row][col], fill='red')
                    self.update_status("Destination point set. Please press start.", "green")
                    self.endpoint_init_active = False
                    self.run_button.config(state='normal')

    def move_robot(self, row, col):
        if self.robot_position:
            r, c = self.robot_position
            if self.grid[r][c] != 2:
                raise IndexError("The robot moves from some position where it has not been.")

        self.update_algorithm_flow(f"Robot moving to ({row}, {col})")

        self.canvas.itemconfig(self.cell_rects[r][c], fill='yellow')
        self.grid[r][c] = 0
        self.grid[row][col] = 2
        self.robot_position = row, col
        self.canvas.itemconfig(self.cell_rects[row][col], fill='green')

    def get_grid(self):
        if self.robot_position is None or self.endpoint_position is None:
            raise Exception("Robot or endpoint not set")
        return self.grid

    def set_start_callback(self, callback):
        self._start_callback = callback

    def start_pathfinding(self):
        self.update_status("Starting pathfinding algorithm...", "blue")
        if hasattr(self, '_start_callback'):
            self._start_callback()

    def run(self):
        self.root.mainloop()
