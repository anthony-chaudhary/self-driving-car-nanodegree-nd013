# ----------
# User Instructions:
# 
# Create a function compute_value which returns
# a grid of values. The value of a cell is the minimum
# number of moves required to get from the cell to the goal. 
#
# If a cell is a wall or it is impossible to reach the goal from a cell,
# assign that cell a value of 99.
# ----------

# Grid format:
#   0 = Navigable space
#   1 = Occupied space

import random
import itertools

# (down, right)
grid = [[0, 0, 1, 0, 0, 0],
        [0, 0, 1, 0, 1, 0],
        [0, 0, 0, 0, 1, 0],
        [0, 0, 1, 1, 1, 0],
        [0, 0, 0, 0, 1, 0]]

grid = [[0, 0, 0, 0, 0, 0],
        [0, 1, 0, 0, 1, 0],
        [0, 1, 0, 0, 0, 0],
        [0, 1, 0, 0, 1, 0]]

grid = [[0 for x in range(20)] for y in range(100)]
grid[1][1] = 1
grid[2][2] = 1
grid[4][4] = 1
grid[7][7] = 1
for i in range(3):
    grid[i][0] = 1

for i in grid:
    print(i)

goal = [len(grid)-1, len(grid[0])-1]
print("\n Goal:", goal)
cost = 1

delta = [[-1, 0], # go up
         [ 0,-1], # go left
         [ 1, 0], # go down
         [ 0, 1]] # go right

delta_name = ['v', '>', '^', '<']


class node():

    newid = itertools.count()

    def __init__(self):
        self.g = 0          # cost to get to node
        self.f = 0    # A* 
        self.x = None
        self.y = None
        self.ep = False  # fully expanded
        self.p = None   # previous node?
        self.d = 0   # delta action ?
        self.id = node.newid.__next__()


class space():

    def __init__(self):
        self.x = None
        self.y = None
        self.ex = False  # Explored


def compute_value(grid,goal,cost):

    # 0. Init
    # TODO assigning
    dynamic_grid = [[node() for i in range(len(grid[0]))] for i in range(len(grid))]
    path_grid = [[node() for i in range(len(grid[0]))] for i in range(len(grid))]
    
    a = node()
    a.x, a.y = goal[0], goal[1]  #start at goal
    dynamic_grid[a.x][a.y] = a
    nodes = [a]
    e_nodes = [a] # eligible nodes
    costs = [a.g]
    explored_dict = {(a.x, a.y): "Explored"}
    expanded_dict = {}
 
    while True:

        # 1. Find smallest cost node
        #print("\nNodes:", len(nodes), ", Nodes expanded:", len(expanded_dict), "Eligible nodes", len(e_nodes))

        costs = [i.g for i in e_nodes]
        try:
            m = min(costs)
        except:
            return dynamic_grid, path_grid
        #print("Lowest cost:", m, "Costs", costs)

        for n in e_nodes:
            if n.g <= m:
                if expanded_dict.get(n.id, False) is False:
                    #print("id", n.id, "not in expanded_dict")
                    c = n  # c == current_node
        #print("Node", c.id," at:", c.x, ",", c.y)

        # 3. Expand node if possible
        for d_i, d in enumerate(delta):
            if (d[0], d[1]) == (0, 1):   # If we have reached last move, mark cell as expanded
                expanded_dict.update({c.id: "Expanded"})
                e_nodes.remove(c)
                #print("Added node", c.id, "position", c.x, ",", c.y, "to expanded.")

            x, y = c.x + d[0], c.y + d[1]   # do move
            if x >= 0 and y >=0:   # index sanity check, this should be better then a try block for speed
                if x <= (len(grid)-1) and y <= len(grid[0])-1:
                
                    explored = explored_dict.get((x, y), False)
                    if grid[x][y] == 0 and explored is False:   # if move is valid
                        new = node()
                        new.g = c.g + cost   # update cost
                        #new.f = new.g + heuristic[x][y]
                        #print("New node:", new.id, "at", x, ",", y)
                        new.x, new.y = x, y
                        nodes.append(new)
                        e_nodes.append(new)
                        dynamic_grid[x][y] = new
                        explored_dict.update({(x, y): "Explored"})
                        
                        path_grid[x][y].d = delta_name[d_i]

                        break


dynamic_grid, path_grid = compute_value(grid, goal, cost)
print()

print("\nCosts:")
for i, x in enumerate(dynamic_grid):
    for y in dynamic_grid[i]:
        print("{x:2d}".format(x=y.g), " ", end="")
    print()

print("\nPaths")
for i, x in enumerate(path_grid):
    for y in path_grid[i]:
        print("{x}".format(x=y.d), " ", end="")
    print()
