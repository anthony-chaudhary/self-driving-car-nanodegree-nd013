import numpy as np


def radiusOfCurvature(ploty, l_center_points, r_center_points, super_fun_y_points):
    """
    Purpose:  Find the radius of curvature as a sanity check.
    Inputs: Lists of points
    Outputs: Average curvature.
    """

    leftx = np.array(l_center_points)
    rightx = np.array(r_center_points)
    super_fun_y_points = np.array(super_fun_y_points)

    # TODO refactor to get these from customPolyFit.poly()
    left_fit = np.polyfit(super_fun_y_points, l_center_points, 2)
    right_fit = np.polyfit(super_fun_y_points, r_center_points, 2)

    # Define y-value where we want radius of curvature
    # I'll choose the maximum y-value, corresponding to the bottom of the image
    y_eval = np.max(ploty)
    left_curved = (
        (1 + (2 * left_fit[0] * y_eval + left_fit[1])**2)**1.5) / np.absolute(2 * left_fit[0])
    right_curved = (
        (1 + (2 * right_fit[0] * y_eval + right_fit[1])**2)**1.5) / np.absolute(2 * right_fit[0])
    # print(left_curved, right_curved)
    # Example values: 1926.74 1908.48

    # Define conversions in x and y from pixels space to meters
    ym_per_pix = 30 / 720  # meters per pixel in y dimension
    xm_per_pix = 3.7 / 700  # meters per pixel in x dimension

    # Fit new polynomials to x,y in world space
    left_fit_cr = np.polyfit(super_fun_y_points *
                             ym_per_pix, leftx * xm_per_pix, 2)
    right_fit_cr = np.polyfit(
        super_fun_y_points * ym_per_pix, rightx * xm_per_pix, 2)
    # Calculate the new radii of curvature
    left_curved = ((1 + (2 * left_fit_cr[0] * y_eval * ym_per_pix + left_fit_cr[
        1])**2)**1.5) / np.absolute(2 * left_fit_cr[0])
    right_curved = ((1 + (2 * right_fit_cr[0] * y_eval * ym_per_pix + right_fit_cr[
        1])**2)**1.5) / np.absolute(2 * right_fit_cr[0])
    # Now our radius of curvature is in meters
    # print(left_curved, 'm', right_curved, 'm')
    # Example values: 632.1 m    626.2 m

    average_curve = round(((left_curved + right_curved) / 2), 0)

    return average_curve
