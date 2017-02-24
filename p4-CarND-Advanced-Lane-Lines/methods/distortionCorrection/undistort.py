import pickle
import cv2


# Read in the saved camera matrix and distortion coefficients
# These are the arrays you calculated using cv2.calibrateCamera()

# figure better way to pass / get these values
# dist_pickle = pickle.load(open("wide_dist_pickle.p", "rb"))
# mtx = dist_pickle["mtx"]
# dist = dist_pickle["dist"]

# Read in an image
nx = 9  # the number of inside corners in x
ny = 6  # the number of inside corners in y


def corners_unwarp(img, nx, ny, mtx, dist):
    # Pass in your image into this function

    # 1) Undistort using mtx and dist
    cv2.undistort(undist, mtx, dist, None, mtx)

    # 2) Convert to grayscale
    gray = cv2.cvtColor(undist, cv2.COLOR_BGR2GRAY)

    # 3) Find the chessboard corners
    ret, corners = cv2.findChessboardCorners(gray, (nx, ny), None)
    # print(corners)

    # 4) If corners found:
    if ret == True:

        # a) draw corners
        img = cv2.drawChessboardCorners(gray, (nx, ny), corners, ret)

        # b) define 4 source points src = np.float32([[,],[,],[,],[,]])

        # Note: you could pick any four of the detected corners
        # as long as those four corners define a rectangle
        # One especially smart way to do this would be to use four well-chosen
        # corners that were automatically detected during the undistortion steps
        # We recommend using the automatic detection of corners in your code
        off = 100
        img_size = (gray.shape[1], gray.shape[0])

        src = np.float32([corners[0], corners[nx - 1],
                          corners[-1], corners[-nx]])

        # c) define 4 destination points dst = np.float32([[,],[,],[,],[,]])
        dst = np.float32([[off, off], [img_size[0] - off, off],
                          [img_size[0] - off, img_size[1] - off],
                          [off, img_size[1] - off]])

        # d) use cv2.getPerspectiveTransform() to get M, the transform matrix
        M = cv2.getPerspectiveTransform(src, dst)

        # e) use cv2.warpPerspective() to warp your image to a top-down view
        warped = cv2.warpPerspective(
            undist, M, img_size, flags=cv2.INTER_LINEAR)

    return warped, M