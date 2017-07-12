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

# x = row
# y = coloumn

grid = [[1, 1, 1, 0, 0, 0],
        [1, 0, 0, 0, 1, 0],
        [0, 0, 1, 0, 0, 0],
        [1, 0, 1, 1, 0, 1],
        [1, 0, 1, 0, 0, 1]]

goal = [2, 0] 
print("\n Goal:", goal)

forward_moves = [[-1,  0], # go up
           [ 0, -1], # go left
           [ 1,  0], # go down
           [ 0,  1]] # go right

forward_symbol = ['^', '<', 'V', '>']
forward_name = ['up', 'left', 'down', 'right']

init = [4, 3, 0]

class action():

    def __init__(self):
        self.g = None   # Cost
        self.o = None   # Orentation adjustment
        self.n = None
        self.s = None  # Symbol

class node():

    newid = itertools.count()

    def __init__(self):
        self.o = None  #  Orentation
        self.a = None  # Action
        self.g = 0   # cost
        self.f = 0    # A* 
        self.x = None
        self.y = None
        self.ep = False  # fully expanded
        self.p = None   # previous node
        self.path = None   # path
        self.d = 0   # delta action 
        self.id = node.newid.__next__()


class path():
    
    newid = itertools.count()

    def __init__(self):
        self.id = path.newid.__next__()
        self.nodes = []
        self.g = 0   # running cost
        self.goal = False   # Path reaches goal


right, forward, left = action(), action(), action()
actions = [right, forward, left]
right.g, forward.g, left.g = 1, 1, 1
right.o, forward.o, left.o = -1, 0 , 1
right.n, forward.n, left.n = "right", "forward", "left"
right.s, forward.s, left.s = "R", "#", "L"

def debug(grid, c, x, y, a_o):
    # x_p x previous
    # x_d x destination

    for i in range(len(grid)):  # rows
        for ii in range(len(grid[i])):  # column
            if i == x and ii == y:
                #print(" {} ".format(c.a.n), end="")
                print(" {} ".format(forward_symbol[a_o]), end="")
            elif i == c.x and ii == c.y:
                print(" {} ".format(forward_symbol[c.o]), end="")
            else:
                print(" - ", end="")
        print()


def print_chart(grid, nodes):
    # x_p x previous
    # x_d x destination
    for i in range(len(grid)):  # rows
        for j in range(len(grid[i])):  # column

            flag = False
            for k in range(len(nodes)):
                # TODO, way to print paths that intersect back on self...
                if flag is False:
                    if i == nodes[k].x and j == nodes[k].y:

                        print(" {} ".format(forward_symbol[nodes[k].o]), end="")
                        #print(" {} ".format(nodes[k].a.s), end="")
                        flag = True

            if flag is False:
                if i == goal[0] and j == goal[1]:
                    print(" {} ".format("*"), end="")
                else:
                    print(" - ", end="")
 
        print()

def search(grid, init, goal):

    # 0. Init
    n = node()
    n.x, n.y = init[0], init[1]  # first node
    n.o = init[2]
    a = action()
    a.o = n.o
    n.a = a
    e_nodes = [n] # eligible nodes
    #debug(grid, n, n.x, n.y)
    print()
    goal_paths = 5
    goal_counter = 0

    p = path()
    p.nodes.append(n)
    path_dict = {n: p}  # map node to path
    paths = [p]

    while True:

        print("Length of e_nodes", len(e_nodes))
        for n in e_nodes:  # get next eligible nodes
            c = n  # c == current_node
            e_nodes.remove(n)
            break

        ### RETURN CONDITION
        p_prior = path_dict[c]
        if (c.x, c.y) == (goal[0], goal[1]):
            goal_reached = True
            p_prior.goal = goal_reached
            goal_counter += 1
            if goal_counter == goal_paths:
                break

        # if at start, or first node
        elif (c.x, c.y) != (init[0], init[1]) or c.id == 0: 
         
            # 3. Expand node if possible
            valid_actions = []
            for a in actions:
                a_o = c.o + a.o   # Update car orientation based on action orientation
                a_o = a_o % 4  # Rest if at end of index

                d = forward_moves[a_o]  # get move based on car orientation
                x, y = c.x + d[0], c.y + d[1]   # do move
                if x >= 0 and y >=0 and x <= (len(grid)-1) and y <= len(grid[0])-1:  # check if valid move.
                    if grid[x][y] == 0:
                        
                        valid_actions.append([x, y, a, a_o])

                        print("Car orientation:", forward_name[c.o], c.o)
                        print("Action:", a.n, d, a_o)  
                        debug(grid, c, x, y, a_o)

            l = len(valid_actions)
            print("Valid actions", l)
            #print(path_dict)

            for v in valid_actions:

                x, y = v[0],v[1]
                n = node()
                n.g = c.g + v[2].g                 
                n.x, n.y = x, y
                n.a = v[2]
                n.o = v[3]
                e_nodes.append(n)

                goal_reached = False

                if l == 1 and l != 0:  # use previous path

                    p_prior.nodes.append(n)
                    p_prior.g = n.g
                    path_dict.update({n: p_prior})
                
                elif l != 0:
                      # spawn new path
                    p = path()
                    p.nodes.append(n)  # add current node
                    p.g = n.g   # update running cost for path
                    paths.append(p)
                    print("Length of paths", len(paths))
                    path_dict.update({n: p}) # update dictionary

                    for n_ in p_prior.nodes:
                        p.nodes.append(n_)
            
            #Check if at goal
            #for p in paths:
                #print(p.g, p.goal)
        print("--------\n")
       
    # TODO condition on say 3 paths reaching goal?? 
    # Return best path
    paths.sort(key=lambda x: x.g)
    for p in paths:
        if p.goal is True:
            print(p.id)
            return p, paths


best_path, paths = search(grid, init, goal)

print()
print("\nNodes:")

for p in paths:
    if p.goal is True:
        print("Path ID", p.id, "cost:", p.g, " Goal reached:", p.goal)


paths.sort(key=lambda x: x.g)

print("\nBest path:")
print_chart(grid, best_path.nodes)

#for i in paths:
#    print(i.id, i.g)
#    print_chart(grid, i.nodes)