# ----------
# User Instructions:
# 
# Implement the function optimum_policy2D below.
#
# You are given a car in grid with initial state
# init. Your task is to compute and return the car's 
# optimal path to the position specified in goal; 
# the costs for each motion are as defined in cost.
#
# There are four motion directions: up, left, down, and right.
# Increasing the index in this array corresponds to making a
# a left turn, and decreasing the index corresponds to making a 
# right turn.

import random
import itertools

grid = [[1, 1, 1, 0, 0, 0],
        [1, 1, 1, 0, 1, 0],
        [0, 0, 0, 0, 0, 0],
        [1, 1, 1, 0, 1, 1],
        [1, 1, 1, 0, 1, 1]]

goal = [2, 0] # given in the form [row,col]
print("\n Goal:", goal)

delta = [[-1, 0], # go up
         [ 0,-1], # go left
         [ 1, 0], # go down
         [ 0, 1]] # go right

delta_name = ['v', '>', '^', '<']

init = [4, 3, 0]

class action():

    def __init__(self):
        self.g = None   # Cost
        self.o = None   # Orentation adjustment
        self.n = None

class node():

    newid = itertools.count()

    def __init__(self):
        self.o = init[2]  #  Orentation
        self.g = 0   # cost
        self.f = 0    # A* 
        self.x = None
        self.y = None
        self.ep = False  # fully expanded
        self.p = None   # previous node
        self.d = 0   # delta action 
        self.id = node.newid.__next__()


class space():

    def __init__(self):
        self.x = None
        self.y = None
        self.max_moves = False  # Explored


right, forward, left = action(), action(), action()
actions = [right, forward, left]
right.g, forward.g, left.g = 2, 1, 20
right.o, forward.o, left.o = -1, 0 ,1
right.n, forward.n, left.n = "right", "forward", "left"


def search(grid, init, goal):

    # 0. Init
    expanded_grid = [[[node()] for i in range(len(grid[0]))] for i in range(len(grid))]

    a = node()
    a.x, a.y = init[0], init[1]  # first node
    expanded_grid[a.x][a.y] = [a]
    e_nodes = [a] # eligible nodes

    for i in range(30):

        print(len(e_nodes))
        for n in e_nodes:  # get next eligible nodes
            c = n  # c == current_node
            break

        e_nodes.remove(c)
        
        # 2. Check if at goal
        if (c.x, c.y) == (goal[0], goal[1]):
            return expanded_grid

        # 3. Expand node if possible
        valid_actions = []
        for a in actions:

            o = c.o + a.o   # Update car orientation based on action orientation
            d = delta[o]  # get move based on car orientation
            x, y = c.x + d[0], c.y + d[1]   # do move
            if x >= 0 and y >=0 and x <= (len(grid)-1) and y <= len(grid[0])-1:  # check if valid move.
                if grid[x][y] == 0:
                    print(d, o)  
                    valid_actions.append([x, y, a, o])
           
        branches = []        
        for v in valid_actions:

            x, y = v[0],v[1]
            n = node()
            n.g = c.g + v[2].g                  
            n.x, n.y = x, y
            e_nodes.append(n)
            branches.append(n)
            n.o = v[3]
        
        expanded_grid[x][y] = branches

    return expanded_grid


def path(expanded_grid):

    path_grid = [[" " for i in range(len(grid[0]))] for i in range(len(grid))]
    a = node()
    a.x, a.y = goal[0], goal[1]
    a.p = None  # previous
    path_grid[goal[0]][goal[1]] = "*"
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


expanded_grid = search(grid, init, goal)
path_grid = expanded_grid
#path_grid = path(expanded_grid)
print()

print("\nCosts:")
for i, x in enumerate(expanded_grid):
    for y in expanded_grid[i]:
        print("{x:2d}".format(x=len(y) ), " ", end="")
    print()

#print("\nPaths")
#for i, x in enumerate(path_grid):
    #for y in path_grid[i]:
        #print("{x}".format(x=y.d), " ", end="")
    #print()