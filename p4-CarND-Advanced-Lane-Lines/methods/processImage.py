import imageio
from moviepy.editor import VideoFileClip
import matplotlib.image as mpimg
import numpy as np
import matplotlib.pyplot as plt
import cv2

from calibration import findPoints
from calibration import calibrate
from distortionCorrection import undistort
from perspectiveTransform import perspectiveTransform
from colorGradientThreshold import absoluteSobelThreshold, directionThreshold, magnitudeThreshold, hlsSelect
from laneDetection import regionOfInterest
from laneDetection import centriods
from laneDetection import customPolyFit
from drawing import draw

imageio.plugins.ffmpeg.download()

imagesPath = "../camera_cal/calibration*.jpg"
objpoints, imgpoints = findPoints.findPoints(imagesPath)


def process_image(image):

    ret, mtx, dist, rvecs, tvecs = calibrate.calibrate(
        image, objpoints, imgpoints)

    # From this point on "image" is the undistorted image
    image, src, dst = undistort.birdsEyeView(image, mtx, dist)

    transformMatrix, warpedImage = perspectiveTransform.perspectiveTransform(
        image, src, dst)

    ksize = 27
    gradx = absoluteSobelThreshold.abs_sobel_thresh(
        warpedImage, orient='x', sobel_kernel=ksize, thresh=(30, 90))
    grady = absoluteSobelThreshold.abs_sobel_thresh(
        warpedImage, orient='y', sobel_kernel=ksize, thresh=(30, 90))
    mag_binary = magnitudeThreshold.mag_thresh(
        warpedImage, sobel_kernel=ksize, mag_thresh=(30, 90))
    dir_binary = directionThreshold.dir_threshold(
        warpedImage, sobel_kernel=15, thresh=(0.7, 1.3))
    hls_select = hlsSelect.hlsSelect(warpedImage)
    combined = np.zeros_like(dir_binary)
    combined[((gradx == 1) & (grady == 1)) | (
        (mag_binary == 1) & (dir_binary == 1)) | (hls_select == 1)] = 1

    combined, vertices = regionOfInterest.region_of_interest(combined)

    window_width = 50
    window_height = 80
    margin = 100

    ploty = np.linspace(0, 720 - 1, 720)

    window_centroids, l_center_points, r_center_points = centriods.find_window_centroids(
        combined, window_width, window_height, margin)

    # output = centriods.prettyPrintCentriods(
    # combined, window_centroids, window_width, window_height)

    left_fitx, right_fitx, super_fun_y_points = customPolyFit.poly(
        l_center_points, r_center_points, window_height)

    # insert image drawing
    print(combined.shape)
    Minv = cv2.getPerspectiveTransform(dst, src)
    result = draw.drawLane(combined, left_fitx,
                           right_fitx, ploty, image, Minv)

    return result


project_video_output = 'AAOUTproject_video.mp4'
clip1 = VideoFileClip("../project_video.mp4")
# NOTE: this function expects color images!!
clip = clip1.fl_image(process_image)
clip.write_videofile(project_video_output, audio=False)
