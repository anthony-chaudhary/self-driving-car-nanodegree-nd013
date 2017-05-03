# Don't modify the code below. Please enter
# your code at the bottom.

from math import *
import random

landmarks = [[10.0, 20.0], [80.0, 90.0], [20.0, 200.0], [80.0, 20.0]]
world_size = 1000


class robot:
    def __init__(self):
        self.x = random.random() * world_size
        self.y = random.random() * world_size
        self.orientation = random.random() * 2.0 * pi
        self.forward_noise = 0.0
        self.turn_noise = 0.0
        self.sense_noise = 0.0

    def set(self, new_x, new_y, new_orientation):
        if new_x < 0 or new_x >= world_size:
            raise ValueError('X coordinate out of bound')
        if new_y < 0 or new_y >= world_size:
            raise ValueError('Y coordinate out of bound')
        if new_orientation < 0 or new_orientation >= 2 * pi:
            raise ValueError('Orientation must be in [0..2pi]')
        self.x = float(new_x)
        self.y = float(new_y)
        self.orientation = float(new_orientation)

    def set_noise(self, new_f_noise, new_t_noise, new_s_noise):
        # makes it possible to change the noise parameters
        # this is often useful in particle filters
        self.forward_noise = float(new_f_noise)
        self.turn_noise = float(new_t_noise)
        self.sense_noise = float(new_s_noise)

    def sense(self):
        Z = []
        for i in range(len(landmarks)):
            dist = sqrt((self.x - landmarks[i][0])
                        ** 2 + (self.y - landmarks[i][1]) ** 2)
            dist += random.gauss(0.0, self.sense_noise)
            Z.append(dist)
        return Z

    def move(self, turn, forward):
        if forward < 0:
            raise ValueError('Robot cant move backwards')

        # turn, and add randomness to the turning command
        orientation = self.orientation + \
            float(turn) + random.gauss(0.0, self.turn_noise)
        orientation %= 2 * pi

        # move, and add randomness to the motion command
        dist = float(forward) + random.gauss(0.0, self.forward_noise)
        x = self.x + (cos(orientation) * dist)
        y = self.y + (sin(orientation) * dist)
        x %= world_size    # cyclic truncate
        y %= world_size

        # set particle
        res = robot()
        res.set(x, y, orientation)
        res.set_noise(self.forward_noise, self.turn_noise, self.sense_noise)
        return res

    def Gaussian(self, mu, sigma, x):

        # calculates the probability of x for 1-dim Gaussian with mean mu and
        # var. sigma
        return exp(- ((mu - x) ** 2) / (sigma ** 2) / 2.0) / sqrt(2.0 * pi * (sigma ** 2))

    def measurement_prob(self, measurement):

        # calculates how likely a measurement should be

        prob = 1.0
        for i in range(len(landmarks)):
            dist = sqrt((self.x - landmarks[i][0])
                        ** 2 + (self.y - landmarks[i][1]) ** 2)
            prob *= self.Gaussian(dist, self.sense_noise, measurement[i])
        return prob

    def __repr__(self):
        return '[x=%.6s y=%.6s orient=%.6s]' % (str(self.x), str(self.y), str(self.orientation))


def eval(r, p):
    sum = 0.0
    for i in range(len(p)):  # calculate mean error
        dx = (p[i].x - r.x + (world_size / 2.0)
              ) % world_size - (world_size / 2.0)
        dy = (p[i].y - r.y + (world_size / 2.0)
              ) % world_size - (world_size / 2.0)
        err = sqrt(dx * dx + dy * dy)
        sum += err
    return sum / float(len(p))


# Make a robot called myrobot that starts at
# coordinates 30, 50 heading north (pi/2).
# Have your robot turn clockwise by pi/2, move
# 15 m, and sense. Then have it turn clockwise
# by pi/2 again, move 10 m, and sense again.

#myrobot = robot()
#myrobot.set_noise(5.0, 0.1, 5.0)
#myrobot.set(30.0, 50.0, pi/2)
#myrobot = myrobot.move(-pi/2, 15.0)
# print myrobot.sense()
#myrobot = myrobot.move(-pi/2, 10.0)
# print myrobot.sense()


def particle_filter_move_and_weights(N, p, Z):
    """
    Experimental function
    Takes: N, number of particles (integer)
    p, Generated particles (array)
    myrobot, Object

    Returns
    p, particles with sense and move completed
    w, weights of particles
    """

    p2 = []
    for i in range(N):
        p2.append(p[i].move(0.1, 5.0))
    p = p2

    w = []
    for i in range(N):
        w.append(p[i].measurement_prob(Z))

    return p, w


def weighted_choice(weights):
    distribution = random.uniform(0, 1)
    limit = 0

    for index, weight in enumerate(weights):

        if limit + weight >= distribution:
            return index  # returns index

        limit += weight   # add to weight
        # print(limit)


# n log n
# a list with particles resampled according to their weights.
def naive_normalize_and_choice(N, weights, p):

    p_new = []
    normalized_list = []
    sum_weights = sum(weights)

    for i in range(N):
        normalized_weight = weights[i] / sum_weights
        normalized_list.append(normalized_weight)
        # print(normalized_weight)
    # print(sum(normalized_list))

    for i in range(N):
        x = weighted_choice(normalized_list)
        p_new.append(p[x])

    return p_new


def resample_wheel(N, weights, p):

    p_new = []
    index = int(random.random() * N)
    beta = 0
    max_w = max(weights)

    for i in range(N):

        beta += random.random() * 2.0 * max_w

        while beta > weights[index]:
            # Favours big ones, so it eats away at the beta, the bigger
            # index ones will subtract more and it will skip further ahead?
            beta -= weights[index]
            index = (index + 1) % N
            # print(index, beta, w[index])

        p_new.append(p[index])
        # print("\n", p[index])

    return p_new


def generate_particles(N):
    # Returns generated particles
    p = []
    # Create particles
    for i in range(N):
        particle = robot()
        particle.set_noise(0.05, 0.05, 5.0)
        p.append(particle)

    return p


def particle_super_duper_filter(time_steps, N, myrobot, p):

    print("Initial error:   ", round(eval(myrobot, p), 2))

    for i in range(time_steps):
        # print(len(p))

        # 1. Robot moves
        myrobot = myrobot.move(0.2, 5.0)

        # 2. Robot senses
        Z = myrobot.sense()

        # 3. Pariticles move (and weights are updated)
        p, w = particle_filter_move_and_weights(N, p, Z)

        # 4. Particles are resampled
        p = resample_wheel(N, w, p)

        # uncomment to use alternate method
        # p = naive_normalize_and_choice(N, w, p)

        error = eval(myrobot, p)
        # print(p[-3:])
        print("Error at time:", i, " ", round(error, 2))


# Initialization
time_steps = 10  # loop through cycle
N = 1000  # number of particles

myrobot = robot()
p = generate_particles(N)
particle_super_duper_filter(time_steps, N, myrobot, p)
