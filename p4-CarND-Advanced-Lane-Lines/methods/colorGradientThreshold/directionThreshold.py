import numpy as np
import cv2


def dir_threshold(image, sobel_kernel=15, thresh=(0.7, 1.3)):
    # Calculate gradient direction
        # Apply the following steps to img
    # 1) Convert to grayscale
    gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)

    # 2) Take the gradient in x and y separately
    sobelx = cv2.Sobel(gray, cv2.CV_64F, 1, 0, ksize=sobel_kernel)
    sobely = cv2.Sobel(gray, cv2.CV_64F, 0, 1, ksize=sobel_kernel)

    # 3) Take the absolute value of the x and y gradients
    abs_s_x = np.absolute(sobelx)
    abs_s_y = np.absolute(sobely)

    # 4) Use np.arctan2(abs_sobely, abs_sobelx) to calculate the direction 
    # of the gradient
    # Important, y should come before x here if we want to detect lines
    dir_grad = np.arctan2(abs_s_y, abs_s_x)

    # 5) Create a binary mask where direction thresholds are met
    binary_output = np.zeros_like(dir_grad)

    # 6) Return this mask as your binary_output image
    binary_output[(dir_grad >= thresh[0]) & (dir_grad <= thresh[1])] = 1

    return binary_output
