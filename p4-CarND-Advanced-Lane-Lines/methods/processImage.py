import numpy as np
import cv2
from calibration import findPoints
from calibration import calibrate
from distortionCorrection import undistort
from perspectiveTransform import perspectiveTransform
from colorGradientThreshold import absoluteSobelThreshold, directionThreshold, magnitudeThreshold, hlsSelect
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

# Define 4 source points as src
src = np.float32([[576, 450], [704, 450],
                  [128, 700], [1152, 700]])

# Define 4 destination points dst
dst = np.float32([[256, 128], [1024, 128],
                  [256, 720], [1024, 720]])

Minv = cv2.getPerspectiveTransform(dst, src)

window_width = 40
window_height = 120
margin = 50

ploty = np.linspace(0, 720 - 1, 720)

font = cv2.FONT_HERSHEY_SIMPLEX


def process_image(image):

    # From this point on "image" is the undistorted image
    image = undistort.undistort(image, mtx, dist)

    warpedImage = perspectiveTransform.perspectiveTransform(
        image, src, dst)

    hls_select = hlsSelect.hlsSelect(warpedImage)

    ksize = 27
    gradx = absoluteSobelThreshold.abs_sobel_thresh(
        warpedImage, orient='x', sobel_kernel=ksize, thresh=(30, 90))
    grady = absoluteSobelThreshold.abs_sobel_thresh(
        warpedImage, orient='y', sobel_kernel=ksize, thresh=(30, 90))
    mag_binary = magnitudeThreshold.mag_thresh(
        warpedImage, sobel_kernel=ksize, mag_thresh=(30, 90))
    dir_binary = directionThreshold.dir_threshold(
        warpedImage, sobel_kernel=15, thresh=(0.7, 1.3))

    gradx = regionOfInterest.region_of_interest(gradx)
    grady = regionOfInterest.region_of_interest(grady)
    mag_binary = regionOfInterest.region_of_interest(mag_binary)
    hls_select = regionOfInterest.region_of_interest(hls_select)

    combined = np.zeros_like(dir_binary)
    combined[((gradx == 1) & (grady == 1)) | (
        (mag_binary == 1) & (dir_binary == 0)) | (hls_select == 1)] = 1

    window_centroids, l_center_points, r_center_points = centriods.find_window_centroids(
        combined, window_width, window_height, margin)

    prettyPrintCentriods = centriods.prettyPrintCentriods(
        combined, window_centroids, window_width, window_height)

    left_fitx, right_fitx, super_fun_y_points = customPolyFit.poly(
        l_center_points, r_center_points, window_height)

    result = draw.drawLane(combined, left_fitx,
                           right_fitx, ploty, image, Minv)

    average_curve = curveCalculations.radiusOfCurvature(
        ploty, l_center_points, r_center_points, super_fun_y_points)

    meters_offset = offset.calculateCarOffset(l_center_points, r_center_points)

    cv2.putText(result, ("Radius of curvature: " + str(average_curve) +
                         "m"), (800, 50), font, 1, (255, 255, 255), 2, cv2.LINE_AA)
    cv2.putText(result, ("Offset from centre: " + str(meters_offset) + "m"),
                (800, 100), font, 1, (255, 255, 255), 2, cv2.LINE_AA)

    # return result, gradx, mag_binary, dir_binary, hls_select, combined
    return result
