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


grid = [[0, 0, 1, 0, 0, 0],
        [0, 0, 1, 0, 0, 0],
        [0, 0, 0, 0, 1, 0],
        [0, 0, 1, 1, 1, 0],
        [0, 0, 0, 0, 1, 0]]
init = [0, 0]
goal = [len(grid)-1, len(grid[0])-1]
cost = 1

delta = [[-1, 0], # go up
         [ 0,-1], # go left
         [ 1, 0], # go down
         [ 0, 1]] # go right

delta_name = ['^', '<', 'v', '>']


class node():

    def __init__(self):
        self.g = 0
        self.x = None
        self.y = None
        self.e = False  # expanded
        self.id = random.randint(1, 100)


def search(grid,init,goal,cost):
    # ----------------------------------------
    # insert code here
    # ----------------------------------------

    # 0. Init
    a = node()
    m = 0  # minimum distance
    a.x, a.y = 4, 5  # first node
    nodes = [a]

    # 1. Find smallest node
    for n in nodes:
        if n.e is False:
            if n.g <= m:
                c = n  # c == current_node
    
    # 2. Check if at goal
    if (c.x, c.y) == (goal[0], goal[1]):
        return (c.g, c.x, c.y)
    
    # 3. Expand node

    
    return 0


print(search(grid, init, goal, cost))