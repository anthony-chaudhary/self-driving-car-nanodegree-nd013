import cv2
import numpy as np


def undistortPoints(img, nx, ny, mtx, dist):
    """
    Purpose: Define source and destination points for perspectiveTransform()
    Inputs: Img array, number of chessboard corners (nx, ny),
    mtx and dist correction values from calibrate()
    Outputs: imgWithChessBoard, source points, destintation points
    """

    # 1) Undistort using mtx and dist
    cv2.undistort(img, mtx, dist, None, mtx)

    # 2) Convert to grayscale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # 3) Find the chessboard corners
    ret, corners = cv2.findChessboardCorners(gray, (nx, ny), None)

    # 4) If corners found:
    if ret is True:

        # a) draw corners
        imgWithChessBoard = cv2.drawChessboardCorners(
            gray, (nx, ny), corners, ret)

        # b) define 4 source points as src
        off = 100
        img_size = (gray.shape[1], gray.shape[0])
        src = np.float32([corners[0], corners[nx - 1],
                          corners[-1], corners[-nx]])

        # c) define 4 destination points dst = np.float32([[,],[,],[,],[,]])
        dst = np.float32([[off, off], [img_size[0] - off, off],
                          [img_size[0] - off, img_size[1] - off],
                          [off, img_size[1] - off]])

    return imgWithChessBoard, src, dst
