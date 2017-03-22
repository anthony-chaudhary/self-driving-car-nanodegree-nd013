import numpy as np
import cv2


def region_of_interest(edges):
    """
    Purpose:  Limit search space to reasonable areas.
    Inputs: Image array (edges)
    Outputs: Image array with mask applied.
    Only keeps the region of the image defined by the polygon
    formed from `vertices`. The rest of the image is set to black.
    """

    mask = np.zeros_like(edges)
    ignore_mask_color = 255

    # Left mask
    match = 128
    bottom_left_a = (match * 2.25, 720)
    bottom_right_a = (match * 10, 720)
    top_left_a = (600, 400)
    top_right_a = (950, 400)

    vertices = np.array([[bottom_left_a, top_left_a,
                          top_right_a, bottom_right_a]],
                        dtype=np.int32)

    # filling pixels inside the polygon defined by "vertices" with the fill
    cv2.fillPoly(mask, vertices, ignore_mask_color)

    # returning the image only where mask pixels are nonzero
    masked_edges = cv2.bitwise_and(edges, mask)

    return masked_edges
