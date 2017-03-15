import numpy as np
import cv2
from calibration import findPoints
from calibration import calibrate
from distortionCorrection import undistort
from perspectiveTransform import perspectiveTransform
from colorGradientThreshold import absoluteSobelThreshold, directionThreshold, magnitudeThreshold, hlsSelect, white, yellow
from laneDetection import regionOfInterest, centriods, customPolyFit
from laneDetection import curveCalculations, offset
from drawing import draw

"""

Purpose: Primary image pipeline for application.


Compute the camera calibration matrix and distortion coefficients given a set of chessboard images.
Apply a distortion correction to raw images.
Use color transforms, gradients, etc., to create a thresholded binary image.
Apply a perspective transform to rectify binary image ("birds-eye view").
Detect lane pixels and fit to find the lane boundary.
Determine the curvature of the lane and vehicle position with respect to center.
Warp the detected lane boundaries back onto the original image.
Output visual display of the lane boundaries and numerical estimation of lane curvature and vehicle position.
"""

imagesPath = "../camera_cal/calibration*.jpg"
objpoints, imgpoints = findPoints.findPoints(imagesPath)

ret, mtx, dist, rvecs, tvecs = calibrate.calibrate(objpoints, imgpoints)

src = np.float32([[544, 470], [736, 470],
                  [128, 700], [1152, 700]])

dst = np.float32([[256, 128], [1024, 128],
                  [256, 720], [1024, 720]])

Minv = cv2.getPerspectiveTransform(dst, src)

window_width = 10
window_height = 92
margin = 25

ploty = np.linspace(0, 720 - 1, 720)

font = cv2.FONT_HERSHEY_SIMPLEX

differenceStore = []

def process_image(self, image, testing_flag=False):

    # Please see doc strings on each function

    # From this point on "image" is the undistorted image
    image = undistort.undistort(image, mtx, dist)

    warpedImage = perspectiveTransform.perspectiveTransform(
        image, src, dst)

    hls_select = hlsSelect.hlsSelect(warpedImage)

    ksize = 17
    gradx = absoluteSobelThreshold.abs_sobel_thresh(
        warpedImage, orient='x', sobel_kernel=ksize, thresh=(35, 115))
    
    mag_binary = magnitudeThreshold.mag_thresh(
        warpedImage, sobel_kernel=ksize, mag_thresh=(35, 115))
    dir_binary = directionThreshold.dir_threshold(
        warpedImage, sobel_kernel=7, thresh=(.8, 1.4))

    white_lines = white.white(warpedImage, thresh=225)
    white_lines = regionOfInterest.region_of_interest(white_lines)

    yellow_lines = yellow.yellow(warpedImage, thresh=235)
    yellow_lines = regionOfInterest.region_of_interest(yellow_lines)

    gradx = regionOfInterest.region_of_interest(gradx)
    mag_binary = regionOfInterest.region_of_interest(mag_binary)
    hls_select = regionOfInterest.region_of_interest(hls_select)

    combined = np.zeros_like(dir_binary)
    combined[(gradx == 1) | (hls_select == 1) | (yellow_lines == 1) | (
        (mag_binary == 1) & (dir_binary == 0)) | white_lines == 1] = 1

    window_centroids, l_center_points, r_center_points = centriods.find_window_centroids(
        combined, window_width, window_height, margin)

    left_fitx, right_fitx, super_fun_y_points = customPolyFit.poly(
        l_center_points, r_center_points, window_height)

    average_curve = curveCalculations.radiusOfCurvature(
        ploty, l_center_points, r_center_points, super_fun_y_points)

    meters_offset = offset.calculateCarOffset(l_center_points, r_center_points)

    # reject bad apples
    distance_between_lines = abs(np.average(left_fitx)-np.average(right_fitx))

    self.distance_between_lines.append(distance_between_lines)
    average_distance_between_lines = np.average(self.distance_between_lines[-60:])

    difference_square = (abs(distance_between_lines - average_distance_between_lines))**2
    print("dif squre:" , difference_square)

    differenceStore.append(difference_square)
    print("runnning average", np.average(difference_square))

    # Only append the current frame if difference square is less
    # than or equal to x pixels.

    if difference_square <= 750:

        self.recent_fit_x_left.append([left_fitx])
        self.recent_fit_x_right.append([right_fitx])

    # Use the average if last n frames as the fit.
    best_x_left = np.average(self.recent_fit_x_left[-5:], axis=0)
    best_x_right = np.average(self.recent_fit_x_right[-5:], axis=0)

    result = draw.drawLane(combined, best_x_left,
                       best_x_right, ploty, image, Minv)

    cv2.putText(result, ("Radius of curvature: " + str(average_curve) +
                         "m"), (800, 50), font, 1, (255, 255, 255), 2, cv2.LINE_AA)
    cv2.putText(result, ("Offset from centre: " + str(meters_offset) + "m"),
                (800, 100), font, 1, (255, 255, 255), 2, cv2.LINE_AA)


    if testing_flag is False:
        return result
    else:
        
        prettyPrintCentriods = centriods.prettyPrintCentriods(combined, window_centroids, window_width, window_height)
        
        return result, gradx, mag_binary, dir_binary, hls_select, combined, prettyPrintCentriods, white_lines, yellow_lines
