import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from colorGradientThreshold import absoluteSobelThreshold, directionThreshold, magnitudeThreshold
import numpy as np
import os

"""
[] Compute the camera calibration matrix and distortion coefficients given a set of chessboard images.
[] Apply a distortion correction to raw images.
[] Use color transforms, gradients, etc., to create a thresholded binary image.
[] Apply a perspective transform to rectify binary image ("birds-eye view").
[] Detect lane pixels and fit to find the lane boundary.
[] Determine the curvature of the lane and vehicle position with respect to center.
[] Warp the detected lane boundaries back onto the original image.
[] Output visual display of the lane boundaries and numerical estimation of lane curvature and vehicle position.
"""

# print(os.getcwd())
image = mpimg.imread('../test_images/test1.jpg')

# Choose a Sobel kernel size
ksize = 31  # Choose a larger odd number to smooth gradient measurements

# Apply each of the thresholding functions
gradx = absoluteSobelThreshold.abs_sobel_thresh(
    image, orient='x', sobel_kernel=ksize, thresh=(20, 100))

grady = absoluteSobelThreshold.abs_sobel_thresh(
    image, orient='y', sobel_kernel=ksize, thresh=(20, 100))

mag_binary = magnitudeThreshold.mag_thresh(
    image, sobel_kernel=ksize, mag_thresh=(30, 100))

dir_binary = directionThreshold.dir_threshold(
    image, sobel_kernel=15, thresh=(0.7, 1.3))

combined = np.zeros_like(dir_binary)
combined[((gradx == 1) & (grady == 1)) | (
    (mag_binary == 1) & (dir_binary == 1))] = 1

print(combined.shape)

f, (ax1, ax2) = plt.subplots(1, 2, figsize=(24, 9))
f.tight_layout()
ax1.imshow(image)
ax1.set_title('Original Image', fontsize=50)
ax2.imshow(combined, cmap='gray')
ax2.set_title('Combined Image', fontsize=50)
plt.subplots_adjust(left=0., right=1, top=0.9, bottom=0.)

plt.show()


top_down, perspective_M = corners_unwarp(img, nx, ny, mtx, dist)
f, (ax1, ax2) = plt.subplots(1, 2, figsize=(24, 9))
f.tight_layout()
ax1.imshow(img)
ax1.set_title('Original Image', fontsize=50)
ax2.imshow(top_down)
ax2.set_title('Undistorted and Warped Image', fontsize=50)
plt.subplots_adjust(left=0., right=1, top=0.9, bottom=0.)
