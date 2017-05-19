
"""
Original interview question reworded for clairty

Given a 1D world of infinite length (x),
and available moves (y) of [1, 2, 3, -1, -2, -3],
and a destination (d) (ie 15),
write a function that returns
the smallest number of moves (result) needed to reach d.

For example if d = 15, result = 5
since the most optimal move is 3, and it can be done 5 times.

Some assumptions:
Starting at 0
moves are in integers
robot will take most optimal move 100% of time with 100% accuracy
Can only use negative moves for negative steps , and only positive moves positive steps
"""


def smallest_number_of_steps(destination, valid_moves):
    """
    Goal: Print smallest number of steps

    >>> Inputs
    destination (float)
    valid_moves (list of floats)

    <<< Outputs:
    Prints information on number of steps used
    """

    total_moves = 0
    counter = 1
    finishedFlag = False

    print("Destination", destination, "\n")
    distance_to_destination = abs(destination)
    validMovesAvailable = 1

    # handle invalid destination
    # TODO would be interesting to change program to accept random starting
    # point
    if destination == 0:
        print("Destination == 0, you have already arrived")
        return

    # Cycle while not at destination.
    while (finishedFlag is False):

        print("Cycle ", counter)

        # 1. Cycle while valid moves in search space don't overshoot target
        validMoveReturned = False
        attemptsCounter = 10

        while (validMoveReturned is False):

            assert(len(valid_moves) > 0)

            if destination < 0:
                # negative direction
                true_move = min(valid_moves)
            else:
                # positive direction
                true_move = max(valid_moves)

            true_move = abs(true_move)

            # Check if valid move
            remainder = abs(distance_to_destination % true_move)
            # Careful with brackets here for division operator
            max_moves_made = (distance_to_destination - remainder) / true_move

            # Check if fraction (ie distance to destination = 1,
            # and true_move = 2)
            if max_moves_made >= 1:

                assert(max_moves_made != .5)

                distance_covered = max_moves_made * true_move

                # 1st exit condition, if valid move is found
                # Valid if less than distance to destination
                if(distance_covered <= distance_to_destination):
                    validMoveReturned = True
                    distance_to_destination -= distance_covered
                    # else break while loop
                    break

            # Update search space if no valid moves available
            validMovesAvailable -= 1
            if (validMovesAvailable == 0):

                # Reduce search space removing last true_move
                valid_moves = [i for i in valid_moves if i != true_move]

                validMovesAvailable = len(valid_moves)

                # print("New search area", valid_moves)
                # print("\nNumber of valid moves available",
                # validMovesAvailable)

                attemptsCounter -= 1

                # Target exit condition, return if exhausted search space
                # Sure there is a better way to do this
                if attemptsCounter == 0 or validMovesAvailable == 0:

                    print("End of search space")

                    if(remainder < true_move):
                        print("Remaining distance to destination:", remainder)
                        finishedFlag = True

                    return

        # update total number of moves
        total_moves += int(max_moves_made)

        print("Covered distance of", (max_moves_made * true_move),
              "at", true_move, "steps per move.")
        print("Distance to destination", distance_to_destination)
        print("Moves this cycle", int(max_moves_made))
        print("Total moves", total_moves)

        # Handle last one if perfect fit
        if distance_to_destination % true_move == 0 or remainder == true_move:
            print("Last cycle, ended with using", true_move)
            print("Final number of moves needed", total_moves)
            finishedFlag = True
            break

        counter += 1
        print("")


# Smalle test
# destination = 56188989653.1565
# valid_moves = [1, 2, 1.1, 2.2, -1, -2, 3.2, -6, 5, 5.5, 2.1, .5]
# smallest_number_of_steps(destination, valid_moves)

import time

# Larger test


def float_range(x, y, step):
    while x < y:
        yield x
        x += step


def test_destination_size(valid_moves_length=10, destinations_length=10000):

    #valid_moves_list = [1, 2, 1.1, 2.2, -1, -2, 3.2, -6, 5, 5.5, 2.1, .5]
    #destinations_list = [1, -1, 0, 1.1, 1.22, -1.1, 200, 300, 56188989653.1565]

    valid_moves_list = [
        i for i in float_range(-valid_moves_length, valid_moves_length, .12)]

    destinations_list = [
        i for i in float_range(destinations_length, destinations_length + 2, 1.0)]

    times = []

    for i in destinations_list:

        start = time.time()

        print("Test", i)
        smallest_number_of_steps(i, valid_moves_list)
        print("\n-------------\n")

        end = time.time()

        times.append(end - start)

    for i, length in enumerate(destinations_list):

        print("For length", length, times[i])


def float_range(x, y, step):
    while x < y:
        yield x
        x += step

test_destination_size()


# Test changes in search space and destination space
# Both for performance and expected output

def test2(valid_moves_length=3, destinations_length=10):

    start = time.time()
    valid_moves_list = [
        i for i in float_range(-valid_moves_length, valid_moves_length, .12)]
    destinations_list = [
        i for i in float_range(-destinations_length, destinations_length, .2)]

    # print(valid_moves_list)
    print("Testing", len(destinations_list), "combinations")

    counter = 0
    for i in destinations_list:

        print("Test", i)
        smallest_number_of_steps(i, valid_moves_list)
        print("\n-------------\n")
        counter += 1

    end = time.time()
    print("Tested", len(destinations_list) *
          len(valid_moves_list), "spaces", end - start, "seconds")
    # print("Length of valid_moves_list", len(valid_moves_list))


def test_average_speed(function, runs):

    start = time.time()

    for i in range(runs):
        function()

    end = time.time()

    total = (end - start) / runs
    print("Average time for", total, "seconds")


# test_average_speed(test2, 1)
#test_average_speed(test1, 1)


def chart_time(function):

    times = []
    lengths = [1, 10, 50, 100]

    for i in lengths:

        start = time.time()

        function(destinations_length=i)

        end = time.time()

        times.append(end - start)

    for i, length in enumerate(lengths):

        print("For length", length, times[i])

# chart_time(test2)


"""
With destinations_length = 10

Results for search attempts 10
O(n ^ 2)
For length 1 0.05199790000915527
For length 10 1.1939997673034668
For length 50 26.219700813293457
For length 100 103.36707592010498  (Tested 168367 spaces)

search attempts 3
O(n ^ 2)
For length 1 0.025998830795288086
For length 10 0.27699971199035645
For length 50 5.900574684143066
For length 100 22.92504048347473

search attempts 2
O(n ^ 2)
For length 1 0.02149820327758789
For length 10 0.14450979232788086
For length 50 2.9780118465423584
For length 100 11.556997537612915

search attempts 1
O(n) ish
For length 1 0.01799750328063965
For length 10 0.01150202751159668
For length 50 0.020499706268310547
For length 100 0.0319979190826416

--------------

valid_moves_length = 10

Destinations_length = length
search attempts 2
O(n)
For length 1 0.015497922897338867
For length 10 0.144500732421875
For length 50 0.7734990119934082
For length 100 1.5210022926330566

search attempts 3
O(n)
For length 1 0.030499935150146484
For length 10 0.2889993190765381
For length 50 1.4169998168945312
For length 100 2.8250014781951904

For length 1 0.03249764442443848
For length 10 0.27750086784362793
For length 50 1.4244987964630127
For length 100 2.828575372695923
For length 1000 28.20200252532959


Example output at search atempts 1

Test -45.59999999999923
Destination -45.59999999999923 

Cycle  1
Covered distance of 45.0 at 3 steps per move.
Distance to destination 0.5999999999992269
Moves this cycle 15
Total moves 15

Cycle  2
End of search space
Remaining distance to destination: 0.5999999999992269

O(n)
For length 1 0.0014994144439697266
For length 10 0.016501665115356445
For length 50 0.09399843215942383
For length 100 0.18999958038330078



Example output at search atempts 10

Test 99.80000000000176
Destination 99.80000000000176 

Cycle  1
Covered distance of 97.9200000000001 at 2.880000000000003 steps per move.
Distance to destination 1.8800000000016581
Moves this cycle 34
Total moves 34

Cycle  2
Covered distance of 1.800000000000002 at 1.800000000000002 steps per move.
Distance to destination 0.08000000000165608
Moves this cycle 1
Total moves 35

Cycle  3
End of search space
Remaining distance to destination: 0.08000000000165608


O(n)
For length 1 0.014481306076049805
For length 10 0.1569993495941162
For length 50 0.8114991188049316
For length 100 1.585500717163086



For change in size of destination
O (log n)

For length -20.0 0.0005021095275878906
For length -19.0 0.01599740982055664
For length -18.0 0.016498327255249023
For length -17.0 0.017001628875732422
For length -16.0 0.01650404930114746
For length -15.0 0.016997575759887695
For length -14.0 0.01650214195251465
For length -13.0 0.016497135162353516
For length -12.0 0.017000436782836914
For length -11.0 0.01650238037109375

For length 986.0 0.0065000057220458984
For length 987.0 0.00700068473815918
For length 988.0 0.007000446319580078
For length 989.0 0.006499528884887695
For length 990.0 0.006499528884887695
For length 991.0 0.012499570846557617
For length 992.0 0.006499767303466797
For length 993.0 0.0065004825592041016
For length 994.0 0.0065000057220458984
For length 995.0 0.006500244140625
For length 996.0 0.006499767303466797
For length 997.0 0.0065000057220458984
For length 998.0 0.0065000057220458984
For length 999.0 0.006501197814941406

For length 10000 0.0064983367919921875
For length 10001.0 0.006999015808105469

Example output:

Test 10000
Destination 10000 

Cycle  1
Covered distance of 9999.359999999988 at 9.919999999999987 steps per move.
Distance to destination 0.6400000000121508
Moves this cycle 1008
Total moves 1008

Cycle  2
End of search space
Remaining distance to destination: 0.6400000000121508


Overall implementation is

--> is O(log n) for change in size of an indvidual destination
	(Mainly due to modulo operation?)

--> is O(n ^ 2 ) for change in search space size
--> O (n) naturally if search attempts restricted to once per loop

--> is O(n ) for change in number of destinations serached
(I think this is more to do with my test case setup)


"""
