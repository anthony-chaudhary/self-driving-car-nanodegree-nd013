import cv2
import numpy as np


def hlsSelect(img, thresh=(100, 170)):
    """
    Purpose: Apply a Hue, Light, and Saturation colour threshold
    to an image.
    Inputs: Image array, threhsold values.
    Outputs: Binary image array with threshold applied.
    """

    # 1) Convert to HLS color space
    hls = cv2.cvtColor(img, cv2.COLOR_RGB2HLS)

    # Select S channel  (second channel in colour space)
    s_channel = hls[:, :, 2]

    # 2) Apply a threshold to the S channel
    binary_output = np.zeros_like(s_channel)

    # 3) Return a binary image of threshold result
    binary_output[(s_channel > thresh[0]) & (s_channel <= thresh[1])] = 1

    return binary_output
