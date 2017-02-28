
def calculateCarOffset(l_center_points, r_center_points):
    """
    Purpose:  Find where the car is in the lane,
    perhaps for some kind of future lane-keeping system.
    Inputs: Lists of points
    Outputs: Offset from lane center in meters.
    """

    xm_per_pix = 3.7 / 700

    # Taking first one to represent point nearest car
    leftx = l_center_points[0]
    rightx = r_center_points[0]

    ground_truth_middle = 640
    car_middle = (leftx + rightx) / 2

    offset = ground_truth_middle - car_middle
    meters_offset = xm_per_pix * offset

    meters_offset = round(meters_offset, 2)

    return meters_offset
