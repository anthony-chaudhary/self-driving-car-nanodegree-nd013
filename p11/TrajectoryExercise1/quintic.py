import numpy as np

# TODO - complete this function
def JMT(start, end, T):
    """
    Calculate the Jerk Minimizing Trajectory that connects the initial state
    to the final state in time T.

    INPUTS

    start - the vehicles start location given as a length three array
        corresponding to initial values of [s, s_dot, s_double_dot]

    end   - the desired end state for vehicle. Like "start" this is a
        length three array.

    T     - The duration, in seconds, over which this maneuver should occur.

    OUTPUT 
    an array of length 6, each value corresponding to a coefficent in the polynomial 
    s(t) = a_0 + a_1 * t + a_2 * t**2 + a_3 * t**3 + a_4 * t**4 + a_5 * t**5

    EXAMPLE

    > JMT( [0, 10, 0], [10, 10, 0], 1)
    [0.0, 10.0, 0.0, 0.0, 0.0, 0.0]
    """

	# where s == position  s_dot == velocity and s_dot_dot = acceleration
	# i == initial, f == final

    s_i = start[0]
    s_i_dot = start[1]
    s_i_dot_dot = start[2] / 2

    s_f = end[0]
    s_f_dot = end[1]
    s_f_dot_dot = end[2]

    result_1 = s_f - (s_i + s_i_dot * T + (s_i_dot_dot * T * T )/ 2)
    result_2 = s_f_dot - (s_i_dot + s_i_dot_dot * T)
    result_3 = s_f_dot_dot - s_i_dot_dot

    t_1 = pow(T, 3) + pow(T, 4) + pow(T, 5)
    t_2 = 3 * pow(T, 2) + 4 * pow(T, 3) + 5 * pow(T, 4)
    t_3 = 6 * pow(T, 1) + 12 * pow(T, 2) + 20 * pow(T, 3)
 
    a3 = result_1 / t_1
    a4 = result_2 / t_2
    a5 = result_3 / t_3



    return [s_i, s_i_dot, s_i_dot_dot, a3, a4, a5]
