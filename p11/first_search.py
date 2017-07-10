# ----------
# User Instructions:
# 
# Define a function, search() that returns a list
# in the form of [optimal path length, row, col]. For
# the grid shown below, your function should output
# [11, 4, 5].
#
# If there is no valid path from the start point
# to the goal, your function should return the string
# 'fail'
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
        [0, 1, 1, 1, 1, 0],
        [0, 1, 0, 0, 0, 0],
        [0, 1, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0]]


x = 20
y = 1000

grid = [[0 for x in range(x)] for y in range(y)]
grid[1][1] = 1
grid[2][2] = 1
grid[4][4] = 1
#grid[7][7] = 1
for i in range(3):
    grid[i][0] = 1

#for i in grid:
    #print(i)


heuristic = [[0 for x in range(x)] for y in range(y)]

# populate heuristic
for i in range(y):
    for ii in range(x):
        heuristic[i][ii] = y - i + x - ii - 2
    
#heuristic = [[9, 8, 7, 6, 5, 4],
#             [8, 7, 6, 5, 4, 3],
#             [7, 6, 5, 4, 3, 2],
#             [6, 5, 4, 3, 2, 1],
#             [5, 4, 3, 2, 1, 0]]

#print()
#for i in heuristic:
    #print(i)

init = [0, 0]
goal = [len(grid)-1, len(grid[0])-1]
print(goal)
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
        self.d = None   # delta action ?
        self.id = node.newid.__next__()


class space():

    def __init__(self):
        self.x = None
        self.y = None
        self.ex = False  # Explored


def search(grid,init,goal,cost, heuristic):

    # 0. Init
    expanded_grid = [[-1 for i in range(len(grid[0]))] for i in range(len(grid))]

    a = node()
    a.x, a.y = init[0], init[1]  # first node
    expanded_grid[a.x][a.y] = a.id
    nodes = [a]
    e_nodes = [a] # eligible nodes
    costs = [a.f + a.g]
    explored_dict = {(a.x, a.y): "Explored"}
    expanded_dict = {}
    #print(expanded_grid)

    while True:

        # 1. Find smallest cost node
        #print("\nNodes:", len(nodes), ", Nodes expanded:", len(expanded_dict), "Eligible nodes", len(e_nodes))

        costs = [i.f for i in e_nodes]
        try:
            m = min(costs)
        except:
            exit("Fail")
        #print("Lowest cost:", m, "Costs", costs)

        for n in e_nodes:
            if n.f <= m:
                if expanded_dict.get(n.id, False) is False:
                    #print("id", n.id, "not in expanded_dict")
                    c = n  # c == current_node
        #print("Node", c.id," at:", c.x, ",", c.y)

        # 2. Check if at goal
        if (c.x, c.y) == (goal[0], goal[1]):
            #"At goal"
            return nodes, [c.g, c.x, c.y], expanded_grid

        # 3. Expand node if possible
        for d in delta:
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
                        new.g = c.g + cost    # update cost
                        new.f = new.g + heuristic[x][y]
                        #print("New node:", new.id, "at", x, ",", y)
                        new.x, new.y = x, y
                        nodes.append(new)
                        e_nodes.append(new)
                        expanded_grid[x][y] = new.g
                        explored_dict.update({(x, y): "Explored"})
                        break


nodes, result, expanded_grid = search(grid, init, goal, cost, heuristic)
nodes.sort(key=lambda x: x.g)
#set(nodes)
#previous_cost = 0
print("\n----")
for n in nodes:
    #if n.g != previous_cost:
    print(n.x+1, "down,", n.y+1, "right. cost", n.g)    #plus one for easy of reading



def path(expanded_grid):

    path_grid = [[" " for i in range(len(grid[0]))] for i in range(len(grid))]
    a = node()
    a.x, a.y = goal[0], goal[1]
    a.p = None  # previous
    path_grid[result[1]][result[2]] = "*"
    l = result[0]
    for i in range(l):
        for d_i, d in enumerate(delta):
            x, y = a.x + d[0], a.y + d[1]
            #print(x, y)
            if x >= 0 and y >=0 and x <= (len(grid)-1) and y <= len(grid[0])-1: 
                if expanded_grid[x][y] < (l - i) and expanded_grid[x][y] >=0 and (l-i) != a.p:
                    
                    path_grid[x][y] = delta_name[d_i]
                    a.x, a.y = x, y
                    a.p = (l-i)

    return path_grid


for i in expanded_grid:
    print(i)

print()
path_grid = path(expanded_grid)
for i in path_grid:
    print(i)
