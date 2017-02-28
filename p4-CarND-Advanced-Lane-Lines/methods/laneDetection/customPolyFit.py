
import numpy as np


def poly(l_center_points, r_center_points, window_height):
    """
    Purpose:  Fit a poloynomial line using points found from
    centriod function.
    Inputs: Lists of points
    Outputs: left/right fitx and associated y points.
    """

    ploty = np.linspace(0, 720 - 1, 720)

    windows = int((720 - window_height) / window_height)

    super_fun_y_points = []
    for w in range(1, windows + 1):
        super_fun_y_points.append(window_height * w)

    super_fun_y_points.reverse()

    # Fit a second order polynomial to pixel positions in each lane line
    left_fit = np.polyfit(super_fun_y_points, l_center_points, 2)
    left_fitx = left_fit[0] * ploty**2 + left_fit[1] * ploty + left_fit[2]

    right_fit = np.polyfit(super_fun_y_points, r_center_points, 2)
    right_fitx = right_fit[0] * ploty**2 + right_fit[1] * ploty + right_fit[2]

    return left_fitx, right_fitx, super_fun_y_points
