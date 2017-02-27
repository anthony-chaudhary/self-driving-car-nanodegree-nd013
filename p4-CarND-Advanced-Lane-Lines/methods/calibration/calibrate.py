import cv2


def calibrate(objpoints, imgpoints):
    """
    Purpose: Provide calibrated camera info
    Inputs: Image array, object points array, and image points array
    Outputs: Vectors and stuff
    """

    #assert img.shape is not None
    #img_size = (img.shape[1], img.shape[0])
    img_size = (1280, 720)

    # Do camera calibration given object points and image points
    ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(
        objpoints, imgpoints, img_size, None, None)

    return ret, mtx, dist, rvecs, tvecs
