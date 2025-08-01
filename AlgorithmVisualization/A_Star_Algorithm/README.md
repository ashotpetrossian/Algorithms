
---

### **1. `controller.py` — The Application Coordinator**
- Acts as the **bridge** between the **GUI** and the **solver**.
- Handles **user input** and updates the state of the application.
- Main responsibilities:
  - Listens for mouse clicks or key presses from `gui.py`.
  - Updates the grid (placing walls, start, and end points).
  - Initiates the pathfinding process by calling `solver.py`.
  - Resets the board or clears the path as requested.
- Think of `controller.py` as the **"game master"** — it decides what happens next depending on user actions.

---

### **2. `gui.py` — The User Interface**
- Responsible for **drawing everything** on the screen.
- Usually implemented with a library like **Pygame** or Tkinter.
- Main responsibilities:
  - Draws the grid and color-codes cells (empty, wall, start, end, visited, path).
  - Handles interactive elements like mouse events and button clicks.
  - Displays animations as the algorithm progresses.
- `gui.py` **does not** decide *how* the algorithm works — it only shows the current state.

---

### **3. `solver.py` — The A\* Algorithm**
- Contains the actual **A\*** search logic.
- Main responsibilities:
  - Implements the A\* algorithm to find the shortest path:
    - Uses a **priority queue** based on the `f = g + h` cost function.
    - `g` = cost from start to current node.
    - `h` = heuristic estimate from current node to goal.
  - Returns the sequence of nodes that form the shortest path.
  - Reports intermediate states (visited nodes) so the GUI can animate the search.
- The solver is **algorithm-only** — it doesn’t care about mouse clicks or drawing, just pure logic.

---

## ⚙ How They Work Together

1. **User interacts** with the GUI (`gui.py`) — placing start/end points and walls.
2. `controller.py` **captures these actions** and updates the internal grid representation.
3. When the user starts the search:
   - `controller.py` passes the grid to `solver.py`.
   - `solver.py` runs A\* and returns the path + visited nodes.
4. `controller.py` sends these results to `gui.py` for **animation**.
5. The GUI animates the **search process** and highlights the **final path**.

---

## ▶ Running the Project

```bash
python main.py
