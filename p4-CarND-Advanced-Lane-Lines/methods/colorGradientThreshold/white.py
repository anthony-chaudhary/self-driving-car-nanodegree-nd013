import cv2
import numpy as np


def white(img, thresh=250):
    """
    Purpose: 
    Inputs: Image array, threhsold values.
    Outputs: Binary image array with threshold applied.
    """

    gray = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
    binary_output = np.zeros_like(gray)

    # 3) Return a binary image of threshold result
    binary_output[(gray >= thresh)] = 1

    return binary_output