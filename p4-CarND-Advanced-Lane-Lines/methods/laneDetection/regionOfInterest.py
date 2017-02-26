import numpy as np
import cv2


def region_of_interest(edges):
    """
    Applies an image mask.
    Only keeps the region of the image defined by the polygon
    formed from `vertices`. The rest of the image is set to black.
    #defining a blank mask to start with
    """
    # Create a masked edges image
    mask = np.zeros_like(edges)
    ignore_mask_color = 255

    # Define a four sided polygon to mask.
    # numpy.array returns a tuple of number of rows, columns and channels.
    imshape = edges.shape
    vertices = np.array(
        [[(200, imshape[0]), (250, 0), (1130, 0), (1150, imshape[0])]], dtype=np.int32)

    # filling pixels inside the polygon defined by "vertices" with the fill
    # color
    cv2.fillPoly(mask, vertices, ignore_mask_color)

    # returning the image only where mask pixels are nonzero
    masked_edges = cv2.bitwise_and(edges, mask)

    return masked_edges, vertices
