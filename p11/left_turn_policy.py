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

#grid = [[1, 1, 1, 0, 0, 0],
#        [1, 0, 1, 0, 1, 0],
#        [0, 0, 0, 0, 0, 0],
#        [1, 1, 1, 0, 1, 1],
#        [1, 1, 1, 0, 1, 1]]
# Generate grid
#w = 10
#h = 10
#grid = [[1 for x in range(w)] for y in range(h)]
#for i in range(w):
    #for j in range(w):
        #if j % 1 or j % 7 == 0:
            #grid[i][j] = 0
            #grid[j][i] = 0

grid = [[0, 0, 0, 0, 0, 0],
        [0, 1, 1, 0, 1, 0],
        [0, 0, 0, 0, 0, 0],
        [1, 0, 0, 0, 1, 0],
        [0, 0, 0, 0, 0, 0]]

for i in grid:
    print(i)

#goal = [len(grid)-2, len(grid[0])-3]
goal = [2, 0]
print("\n Goal:", goal)
forward_moves = [[-1,  0], # go up
           [ 0, -1], # go left
           [ 1,  0], # go down
           [ 0,  1]] # go right
forward_name = ['up', 'left', 'down', 'right']
forward_symbol = ['^', '<', 'V', '>']

#init = [0, 0, 3]  # for larger 
init = [2, 5, 3]

class action():

    def __init__(self):
        self.g = None   # Cost
        self.o = None   # Orentation
        self.n = None   # Name
        self.s = None   # Symbol


class node():

    newid = itertools.count()
    def __init__(self):
        self.o = None  #  Orentation
        self.a = None  #  Action
        self.g = 0     #  Cost
        self.x = None  #  Row position
        self.y = None  #  Column position 
        self.id = node.newid.__next__()


class path():
    
    newid = itertools.count()
    def __init__(self):
        self.id = path.newid.__next__()
        self.nodes = []
        self.g = 0       # Total path cost
        self.goal = False   # True if path reaches goal
        self.init_passes = 0  # Times passed through origin


#### INIT ###
right, forward, left = action(), action(), action()
actions = [forward, left, right]
right.g, forward.g, left.g = 1, 1, 200
right.o, forward.o, left.o = -1, 0 , 1
right.n, forward.n, left.n = "right", "forward", "left"
right.s, forward.s, left.s = "R", "#", "L"


### Prints grid cell with current car position and offered action ####
def debug(grid, c, x, y, a_o):

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


### Prints final path (prints entire path while debug() prints single point)
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


## Main entry
## TODO better way to handle low obstacle space?

def search(grid, init, goal, debug_flag=False, info=False):

    # 0. Init
    n = node()
    n.x, n.y = init[0], init[1]  # first node
    n.o = init[2]
    e_nodes = [n] # eligible nodes
 
    print()
    goal_paths = 500   # Number of True goals to find.
    pass_through_init_attempts = 2  # Max times can pass through origin
    goal_counter = 0
    debug(grid, n, n.x, n.y, n.o)

    p = path()
    p.nodes.append(n)
    path_dict = {n: p}  # map node to path
    paths = [p]
    #e_paths = [p]  # eligible paths

    while True:

        if info is True: print("Length of e_nodes", len(e_nodes))

        # 1. Get node.
        for n in e_nodes:
            c = n  # c == current_node
            e_nodes.remove(n)   # Remove node for efficiency 
            break

        # 2. Check if reached goal
        p_prior = path_dict[c]
        if (c.x, c.y) == (goal[0], goal[1]):
            goal_reached = True
            p_prior.goal = goal_reached
            goal_counter += 1
            #e_paths.remove(p_prior)
            #print(e_paths)
            if goal_counter == goal_paths:
                break

        # 3. Check if at start, or first node
        elif p_prior.init_passes <= pass_through_init_attempts or c.id == 0 or (c.x, c.y) != (init[0], init[1]): 
        
            if (c.x, c.y) == (init[0], init[1]):
                p_prior.init_passes += 1
                #if p_prior.init_passes <= pass_through_init_attempts:
                    #e_paths.remove(p_prior)

            # 4. Determine valid actions
            valid_actions = []
            for a in actions:
                a_o = c.o + a.o   # Update car orientation based on action orientation
                a_o = a_o % 4  # Rest if at end of index

                d = forward_moves[a_o]  # get move based on car orientation
                x, y = c.x + d[0], c.y + d[1]   # do move
                if x >= 0 and y >=0 and x <= (len(grid)-1) and y <= len(grid[0])-1:  # check if valid move.
                    if grid[x][y] == 0:
                        
                        valid_actions.append([x, y, a, a_o]) 
                        if debug_flag is True: 
                            print("Car orientation:", forward_name[c.o], c.o)
                            print("Action:", a.n, d, a_o) 
                            debug(grid, c, x, y, a_o)

            l = len(valid_actions)
            if debug_flag is True: print("Valid actions", l)
            
            # 5. Construct new node for each valid action
            #if len(valid_actions) == 0:
                #e_paths.remove(p_prior)

            for v in valid_actions:
                x, y = v[0],v[1]
                n = node()
                n.g = c.g + v[2].g                 
                n.x, n.y = x, y
                n.a = v[2]
                n.o = v[3]
                e_nodes.append(n)

                # 6. Branch paths as required
                # If we have a single action continue on prior path
                # Else build new path.
                if l == 1 and l != 0:  

                    p_prior.nodes.append(n)
                    p_prior.g = n.g
                    path_dict.update({n: p_prior})
                
                elif l != 0:
                      # spawn new path
                    p = path()
                    p.nodes.append(n)  # add current node
                    p.g = n.g   # update running cost for path
                    paths.append(p)
                    if debug is True: print("Length of paths", len(paths))
                    path_dict.update({n: p}) # update dictionary
                    #e_paths.append(p)

                    for n_ in p_prior.nodes:
                        p.nodes.append(n_)
        
        if debug_flag is True:
            print("--------\n")

        # What other reason would you remove a path???
        #if debug_flag is True:
            #print("Eligible paths:", len(e_paths))

            if len(e_nodes) == 0 and len(valid_actions) == 0:
                break


    # TO DO better handling if no path
    
    # 7. Return best path, and other paths
    paths.sort(key=lambda x: x.g)
    best_paths = []
    for p in paths:
        if p.goal is True:
            best_paths.append(p)
    
    try:
        best_path = best_paths[0]
    except:
        exit("Error! No path\n\n")

    return best_path, best_paths


best_path, paths = search(grid, init, goal, debug_flag=False, info=False)
print("\nNodes:")

# General information on paths
#for p in paths:
    #if p.goal is True:
        #print("Path ID {x:2d} ".format(x=p.id), "Cost: {x:2d} ".format(x=p.g), "Steps:", len(p.nodes), " Goal reached:", p.goal )



print("\nBest path:")
print_chart(grid, best_path.nodes)
print("Path ID {x:2d} ".format(x=best_path.id), "Cost: {x:2d} ".format(x=best_path.g), "Steps:", len(best_path.nodes), " Goal reached:", best_path.goal )

try:
    print("\nNext best path")
    print_chart(grid, paths[1].nodes)
    print("Path ID {x:2d} ".format(x=paths[1].id), "Cost: {x:2d} ".format(x=paths[1].g), "Steps:", len(paths[1].nodes), " Goal reached:", paths[1].goal )

except:
    print("Only one best path")
