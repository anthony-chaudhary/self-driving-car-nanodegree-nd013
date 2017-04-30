p = [0.2, 0.2, 0.2, 0.2, 0.2]
world = ['green', 'red', 'red', 'green', 'green']
measurements = ['red', 'red', 'green', 'green', 'green']
motions = [1, 1, 2, 1, 1]
pHit = 0.6
pMiss = 0.2
pExact = 0.8
pOvershoot = 0.1
pUndershoot = 0.1


def sense(p, Z):
    q = []
    for i in range(len(p)):
        hit = (Z == world[i])
        q.append(p[i] * (hit * pHit + (1 - hit) * pMiss))
    s = sum(q)
    for i in range(len(q)):
        q[i] = q[i] / s
    return q


# Where U == movement
def move(p, U):
    q = []
    for i in range(len(p)):
        s = pExact * p[(i - U) % len(p)]
        s = s + pOvershoot * p[(i - U - 1) % len(p)]
        s = s + pUndershoot * p[(i - U + 1) % len(p)]
        q.append(s)
    return q

# Given the list motions=[1,1] which means the robot
# moves right and then right again, compute the posterior
# distribution if the robot first senses red, then moves
# right one, then senses green, then moves right again,
# starting with a uniform prior distribution.


def pFormat(zlist):
    return [round(i, 3) for i in zlist]


def posterior(p, world, motions, measurements):
    """
    world == 1 demensional envrionemnt example
    where p == probablity of position in world
    motions == steps taken
    measurements == demension in the world to compare,
        related to motions

    Function loops through motion list,
    and calls move and sense functions to update envrionment
    """

    print("Start ->", pFormat(p))

    for i, motion in enumerate(motions):
        p = sense(p, measurements[i])
        p_index = p.index(max(p))
        print("Sense ->", pFormat(p), "Index ->",
              p_index, "  Position ->", world[p_index], "  Measured ->", measurements[i])

        p = move(p, motion)
        p_index = p.index(max(p))
        print("Move  ->", pFormat(p), "Moved ->",
              motion, "  New Index ->", p_index)

    return p

p = posterior(p, world, motions, measurements)
print("Final result ->", pFormat(p))
