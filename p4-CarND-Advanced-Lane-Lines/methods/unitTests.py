

def testFindPoints():
    """
    Purpose: Test find points function.
    Inputs: findPoints.findPoints() and hardcoded calibration path
    Outputs: Prints points and success message if program reaches end.

    """

    from calibration import findPoints

    imagesPath = "../camera_cal/calibration*.jpg"

    objpoints, imgpoints = findPoints.findPoints(imagesPath)

    print("objpoints", objpoints, "\n")
    print("imgpoints", imgpoints, "\n")

    return print("Success")

# testFindPoints()


def testCalibration():
    """
    Purpose: Test calibration function.
    Inputs: findPoints.findPoints(), calibrate.calibrate(),
    hardcoded calibration path, hard coded image path.
    Outputs: Success message if program reaches end.

    """

    from calibration import findPoints
    imagesPath = "../camera_cal/calibration*.jpg"
    objpoints, imgpoints = findPoints.findPoints(imagesPath)

    from calibration import calibrate
    import matplotlib.image as mpimg
    image = mpimg.imread('../test_images/test1.jpg')
    #image = "test"
    ret, mtx, dist, rvecs, tvecs = calibrate.calibrate(
        image, objpoints, imgpoints)

    return print("Success")

# testCalibration()


def testDrawChessBoardCorners():
    """
    Purpose:
    Inputs: 
    Outputs: 
    """

    from calibration import findPoints
    imagesPath = "../camera_cal/calibration*.jpg"
    objpoints, imgpoints = findPoints.findPoints(imagesPath)

    from calibration import calibrate
    import matplotlib.image as mpimg
    image = mpimg.imread('../camera_cal/calibration12.jpg')
    ret, mtx, dist, rvecs, tvecs = calibrate.calibrate(
        image, objpoints, imgpoints)

    from distortionCorrection import undistort

    nx = 9  # the number of inside corners in x
    ny = 6  # the number of inside corners in y

    undistort, src, dst, ret, corners = undistort.undistortPoints(
        image, nx, ny, mtx, dist)

    # Draw corners
    import cv2
    chessBoard = cv2.drawChessboardCorners(undistort, (nx, ny), corners, ret)

    # Plot results
    import matplotlib.pyplot as plt
    f, (ax1, ax2) = plt.subplots(1, 2, figsize=(24, 9))
    f.tight_layout()
    ax1.imshow(image)
    ax1.set_title('Original Image', fontsize=50)
    ax2.imshow(undistort)
    ax2.set_title('Chessboard corners', fontsize=50)
    plt.subplots_adjust(left=0., right=1, top=0.9, bottom=0.)

    plt.show()

    return "success"

# testDrawChessBoardCorners()


def perspectiveTransfromVisual():
    """
    Purpose:
    Inputs: 
    Outputs: 
    """

    from calibration import findPoints
    imagesPath = "../camera_cal/calibration*.jpg"
    objpoints, imgpoints = findPoints.findPoints(imagesPath)

    import matplotlib.image as mpimg
    image = mpimg.imread("../camera_cal/calibration6.jpg")

    from calibration import calibrate
    ret, mtx, dist, rvecs, tvecs = calibrate.calibrate(
        image, objpoints, imgpoints)

    from distortionCorrection import undistort
    nx = 9
    ny = 6
    undistort, src, dst, ret, corners = undistort.undistortPoints(
        image, nx, ny, mtx, dist)

    print(src)

    image = mpimg.imread('../test_images/test1.jpg')
    from perspectiveTransform import perspectiveTransform
    transformMatrix, warpedImage = perspectiveTransform.perspectiveTransform(
        image, src, dst)

    import matplotlib.pyplot as plt
    f, (ax1, ax2) = plt.subplots(1, 2, figsize=(24, 9))
    f.tight_layout()
    ax1.imshow(image)
    ax1.set_title('Original image', fontsize=50)
    ax2.imshow(warpedImage)
    ax2.set_title('Undistorted image', fontsize=50)
    plt.subplots_adjust(left=0., right=1, top=0.9, bottom=0.)

    plt.show()

    return "success"

perspectiveTransfromVisual()
