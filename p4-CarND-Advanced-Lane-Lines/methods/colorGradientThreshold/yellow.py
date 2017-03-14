import cv2
import numpy as np


def yellow(img, thresh=250):
    """
    Purpose: 
    Inputs: Image array, threhsold values.
    Outputs: Binary image array with threshold applied.
    """

    red = img[:, :, 0]

    binary_output = np.zeros_like(red)

    #Return a binary image of threshold result
    binary_output[(red >= thresh)] = 1

    return binary_output