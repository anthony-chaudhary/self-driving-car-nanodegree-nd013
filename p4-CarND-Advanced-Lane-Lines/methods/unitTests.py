
"""
Uncomment any function to run test

"Success" means program has run all lines without
python level errors.

Most tests also generate some additional output to
visually verify expected results.

"""


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

#testFindPoints()


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
    ret, mtx, dist, rvecs, tvecs = calibrate.calibrate(objpoints, imgpoints)

    return print("Success")

#testCalibration()


def testDrawChessBoardCorners():
    """
    Purpose: Test if chessboard corners works
    """

    from calibration import findPoints
    imagesPath = "../camera_cal/calibration*.jpg"
    objpoints, imgpoints = findPoints.findPoints(imagesPath)

    from calibration import calibrate
    import matplotlib.image as mpimg
    image = mpimg.imread('../camera_cal/calibration3.jpg')
    ret, mtx, dist, rvecs, tvecs = calibrate.calibrate(objpoints, imgpoints)

    from distortionCorrection import undistort

    nx = 9  # the number of inside corners in x
    ny = 6  # the number of inside corners in y

    undistort, src, dst, ret, corners = undistort.chessBoardTest(
        image, nx, ny, mtx, dist)

    # Draw corners
    import cv2
    chessBoard = cv2.drawChessboardCorners(undistort, (nx, ny), corners, ret)

    # Plot results
    import matplotlib.pyplot as plt
    f, (ax1, ax2) = plt.subplots(1, 2, figsize=(24, 9))
    f.tight_layout()
    ax1.imshow(image)
    ax1.set_title('Original Image', fontsize=30)
    ax2.imshow(chessBoard)
    ax2.set_title('Undistorted image with chessboard corners', fontsize=30)
    plt.subplots_adjust(left=0., right=1, top=0.9, bottom=0.)

    plt.show()

    return print("Success")

#testDrawChessBoardCorners()


def testUndistort():

    from calibration import findPoints
    imagesPath = "../camera_cal/calibration*.jpg"
    objpoints, imgpoints = findPoints.findPoints(imagesPath)

    import matplotlib.image as mpimg
    image = mpimg.imread('../test_images/test1.jpg')

    from calibration import calibrate
    ret, mtx, dist, rvecs, tvecs = calibrate.calibrate(objpoints, imgpoints)

    from distortionCorrection import undistort
    undistort = undistort.undistort(image, mtx, dist)

    import matplotlib.pyplot as plt
    f, (ax1, ax2) = plt.subplots(1, 2, figsize=(24, 9))
    f.tight_layout()
    ax1.imshow(image)
    ax1.set_title('Original image', fontsize=50)
    ax2.imshow(undistort)
    ax2.set_title('Undistorted image', fontsize=50)
    plt.subplots_adjust(left=0., right=1, top=0.9, bottom=0.)

    plt.show()

    return print("Success")

# testUndistort()


def testBirdsEyeView():

    import numpy as np
    src = np.float32([[576, 450], [704, 450],
                      [128, 700], [1152, 700]])

    # Define 4 destination points dst
    dst = np.float32([[256, 128], [1024, 128],
                      [256, 720], [1024, 720]])

    from calibration import findPoints
    imagesPath = "../camera_cal/calibration*.jpg"
    objpoints, imgpoints = findPoints.findPoints(imagesPath)

    import matplotlib.image as mpimg
    image = mpimg.imread('../test_images/straight_lines1.jpg')

    from calibration import calibrate
    ret, mtx, dist, rvecs, tvecs = calibrate.calibrate(objpoints, imgpoints)

    from distortionCorrection import undistort
    undistort = undistort.undistort(image, mtx, dist)

    print(src)

    from perspectiveTransform import perspectiveTransform
    warpedImage = perspectiveTransform.perspectiveTransform(
        image, src, dst)

    import matplotlib.pyplot as plt
    f, (ax1, ax2) = plt.subplots(1, 2, figsize=(24, 9))
    f.tight_layout()
    ax1.imshow(image)
    ax1.set_title('Original image', fontsize=50)
    ax2.imshow(warpedImage)
    ax2.set_title('Birds eye image', fontsize=50)
    plt.subplots_adjust(left=0., right=1, top=0.9, bottom=0.)

    plt.show()

    return print("Success")

#testBirdsEyeView()


def testThresholds():
    import numpy as np
    # Define 4 source points as src
    src = np.float32([[576, 450], [704, 450],
                      [128, 700], [1152, 700]])

    # Define 4 destination points dst
    dst = np.float32([[256, 128], [1024, 128],
                      [256, 720], [1024, 720]])

    from calibration import findPoints
    imagesPath = "../camera_cal/calibration*.jpg"
    objpoints, imgpoints = findPoints.findPoints(imagesPath)

    import matplotlib.image as mpimg
    image = mpimg.imread('../test_images/test5.jpg')

    from calibration import calibrate
    ret, mtx, dist, rvecs, tvecs = calibrate.calibrate(objpoints, imgpoints)

    from distortionCorrection import undistort
    image = undistort.undistort(image, mtx, dist)

    from perspectiveTransform import perspectiveTransform
    warpedImage = perspectiveTransform.perspectiveTransform(image, src, dst)

    from colorGradientThreshold import absoluteSobelThreshold, directionThreshold, magnitudeThreshold, hlsSelect
    import numpy as np

    ksize = 31

    # Apply each of the thresholding functions
    gradx = absoluteSobelThreshold.abs_sobel_thresh(
        image, orient='x', sobel_kernel=ksize, thresh=(30, 90))

    grady = absoluteSobelThreshold.abs_sobel_thresh(
        image, orient='y', sobel_kernel=ksize, thresh=(30, 90))

    mag_binary = magnitudeThreshold.mag_thresh(
        image, sobel_kernel=ksize, mag_thresh=(30, 90))

    dir_binary = directionThreshold.dir_threshold(
        image, sobel_kernel=15, thresh=(0.7, 1.3))

    hls_select = hlsSelect.hlsSelect(image)

    combined = np.zeros_like(dir_binary)
    combined[((gradx == 1) & (grady == 1)) | (
        (mag_binary == 1) & (dir_binary == 1)) | (hls_select == 1)] = 1

    import matplotlib.pyplot as plt
    f, (ax1, ax2, ax3, ax4, ax5, ax6, ax7) = plt.subplots(
        1, 7, figsize=(24, 9))
    f.tight_layout()

    ax1.imshow(image)
    ax1.set_title('Original', fontsize=30)

    ax2.imshow(gradx, cmap="gray")
    ax2.set_title('gradx', fontsize=30)

    ax3.imshow(grady, cmap="gray")
    ax3.set_title('grady', fontsize=30)

    ax4.imshow(mag_binary, cmap="gray")
    ax4.set_title('mag_binary', fontsize=30)

    ax5.imshow(dir_binary, cmap="gray")
    ax5.set_title('dir_binary', fontsize=30)

    ax6.imshow(combined, cmap="gray")
    ax6.set_title('Combined', fontsize=30)

    ax7.imshow(hls_select, cmap="gray")
    ax7.set_title('hls_select', fontsize=30)

    plt.subplots_adjust(left=0., right=1, top=0.9, bottom=0.)
    plt.show()

    plt.imshow(combined, cmap="gray")
    plt.show()

    return print("Success")


#testThresholds()


def peakLaneHistogram():

    import numpy as np
    # Define 4 source points as src
    src = np.float32([[576, 450], [704, 450],
                  [128, 700], [1152, 700]])

    # Define 4 destination points dst
    dst = np.float32([[256, 128], [1024, 128],
                  [256, 720], [1024, 720]])

    from calibration import findPoints
    imagesPath = "../camera_cal/calibration*.jpg"
    objpoints, imgpoints = findPoints.findPoints(imagesPath)

    import matplotlib.image as mpimg
    image = mpimg.imread('../test_images/test1.jpg')

    from calibration import calibrate
    ret, mtx, dist, rvecs, tvecs = calibrate.calibrate(objpoints, imgpoints)

    from distortionCorrection import undistort
    undistort = undistort.undistort(image, mtx, dist)

    from perspectiveTransform import perspectiveTransform
    warpedImage = perspectiveTransform.perspectiveTransform(
        image, src, dst)

    from colorGradientThreshold import absoluteSobelThreshold, directionThreshold, magnitudeThreshold, hlsSelect
    import numpy as np
    image = undistort
    ksize = 27
    gradx = absoluteSobelThreshold.abs_sobel_thresh(
        warpedImage, orient='x', sobel_kernel=ksize, thresh=(30, 90))
    grady = absoluteSobelThreshold.abs_sobel_thresh(
        warpedImage, orient='y', sobel_kernel=ksize, thresh=(30, 90))
    mag_binary = magnitudeThreshold.mag_thresh(
        warpedImage, sobel_kernel=ksize, mag_thresh=(30, 90))
    dir_binary = directionThreshold.dir_threshold(
        warpedImage, sobel_kernel=15, thresh=(0.7, 1.3))
    hls_select = hlsSelect.hlsSelect(warpedImage)
    combined = np.zeros_like(dir_binary)
    combined[((gradx == 1) & (grady == 1)) | (
        (mag_binary == 1) & (dir_binary == 1)) | (hls_select == 1)] = 1

    import matplotlib.pyplot as plt

    histogram = np.sum(combined[combined.shape[0] / 2:, :], axis=0)
    plt.plot(histogram)
    plt.show()

    f, (ax1, ax2) = plt.subplots(1, 2, figsize=(24, 9))
    f.tight_layout()
    ax1.imshow(undistort)
    ax1.set_title('Image', fontsize=50)
    ax2.imshow(combined, cmap="gray")
    ax2.set_title('Birds eye image', fontsize=50)
    plt.subplots_adjust(left=0., right=1, top=0.9, bottom=0.)
    plt.show()

    return "success"


#peakLaneHistogram()


def testLaneDetection():

    import numpy as np
    # Define 4 source points as src
    src = np.float32([[576, 450], [704, 450],
                      [128, 700], [1152, 700]])

    # Define 4 destination points dst
    dst = np.float32([[256, 128], [1024, 128],
                      [256, 720], [1024, 720]])

    from calibration import findPoints
    imagesPath = "../camera_cal/calibration*.jpg"
    objpoints, imgpoints = findPoints.findPoints(imagesPath)

    import matplotlib.image as mpimg
    image = mpimg.imread('../test_images/test1.jpg')

    from calibration import calibrate
    ret, mtx, dist, rvecs, tvecs = calibrate.calibrate(objpoints, imgpoints)

    from distortionCorrection import undistort
    undistort = undistort.undistort(image, mtx, dist)

    from perspectiveTransform import perspectiveTransform
    warpedImage = perspectiveTransform.perspectiveTransform(image, src, dst)

    from colorGradientThreshold import absoluteSobelThreshold, directionThreshold, magnitudeThreshold, hlsSelect
    import numpy as np
    image = undistort
    ksize = 27
    gradx = absoluteSobelThreshold.abs_sobel_thresh(
        warpedImage, orient='x', sobel_kernel=ksize, thresh=(30, 90))
    grady = absoluteSobelThreshold.abs_sobel_thresh(
        warpedImage, orient='y', sobel_kernel=ksize, thresh=(30, 90))
    mag_binary = magnitudeThreshold.mag_thresh(
        warpedImage, sobel_kernel=ksize, mag_thresh=(30, 90))
    dir_binary = directionThreshold.dir_threshold(
        warpedImage, sobel_kernel=15, thresh=(0.7, 1.3))
    hls_select = hlsSelect.hlsSelect(warpedImage)
    combined = np.zeros_like(dir_binary)
    combined[((gradx == 1) & (grady == 1)) | (
        (mag_binary == 1) & (dir_binary == 1)) | (hls_select == 1)] = 1

    from laneDetection import regionOfInterest
    combined = regionOfInterest.region_of_interest(combined)

    window_width = 50
    window_height = 80
    margin = 100

    ploty = np.linspace(0, 720 - 1, 720)

    from laneDetection import centriods
    window_centroids, l_center_points, r_center_points = centriods.find_window_centroids(
        combined, window_width, window_height, margin)

    output = centriods.prettyPrintCentriods(
        combined, window_centroids, window_width, window_height)

    from laneDetection import customPolyFit
    left_fitx, right_fitx, super_fun_y_points = customPolyFit.poly(
        l_center_points, r_center_points, window_height)

    import matplotlib.pyplot as plt

    plt.imshow(combined, cmap="gray")
    plt.show()

    plt.imshow(output)
    plt.title('window fitting results')
    plt.xlim(0, 1280)
    plt.ylim(0, 720)
    plt.plot(left_fitx, ploty, color='red', linewidth=3)
    plt.plot(right_fitx, ploty, color='blue', linewidth=3)
    plt.gca().invert_yaxis()  # to visualize as we do the images
    plt.show()

    from drawing import draw
    import cv2
    Minv = cv2.getPerspectiveTransform(dst, src)
    result = draw.drawLane(combined, left_fitx,
                           right_fitx, ploty, image, Minv)

    from laneDetection import curveCalculations, offset
    average_curve = curveCalculations.radiusOfCurvature(
        ploty, l_center_points, r_center_points, super_fun_y_points)
    meters_offset = offset.calculateCarOffset(l_center_points, r_center_points)

    font = cv2.FONT_HERSHEY_SIMPLEX
    cv2.putText(result, ("Radius of curvature: " + str(average_curve) + "m"),(800,50), font, 1,(255,255,255),2,cv2.LINE_AA)
    cv2.putText(result, ("Offset from centre: " + str(meters_offset) + "m"),(800,100), font, 1,(255,255,255),2,cv2.LINE_AA)

    plt.imshow(result)
    plt.show()

    return print("Success")

#testLaneDetection()


########### BULK TEST IMAGES ############
# Collects test images, creates folder to put them in, runs pipeline, and saves images.
# Uses settings in methods/processImage

def testAllImages():
    import matplotlib.image as mpimg
    import cv2
    import os
    from processImage import process_image
    import lineClass

    test_images = os.listdir("../test_images/")

    try:
        processed_images = os.listdir("../test_images/processed_images/")
    except FileNotFoundError:
        print("File not found")

    x = lineClass.Line()

    for img in test_images:
        if '.jpg' in img:
            image = mpimg.imread("../test_images/%(filename)s" %
                                 {"filename": img})

            result, gradx, mag_binary, dir_binary, hls_select, combined, prettyPrintCentriods, white_lines, yellow_lines = x.process_image(image, testing_flag=True)
            result_color_fix = cv2.cvtColor(result, cv2.COLOR_BGR2RGB)
            #print(combined.shape,s prettyPrintCentriods.shape)

            cv2.imwrite("../test_images/processed_images/%(filename)s_processed.jpg" %
                        {"filename": img.replace(".jpg", "")}, result_color_fix)

            cv2.imwrite("../test_images/processed_images/%(filename)s_gradx.jpg" %
                        {"filename": img.replace(".jpg", "")}, gradx * 255)

            cv2.imwrite("../test_images/processed_images/%(filename)s_mag_binary.jpg" %
                        {"filename": img.replace(".jpg", "")}, mag_binary * 255)

            cv2.imwrite("../test_images/processed_images/%(filename)s_dir_binary.jpg" %
                        {"filename": img.replace(".jpg", "")}, dir_binary * 255)

            cv2.imwrite("../test_images/processed_images/%(filename)s_hls_select.jpg" %
                        {"filename": img.replace(".jpg", "")}, hls_select * 255)

            cv2.imwrite("../test_images/processed_images/%(filename)s_white_lines.jpg" %
                        {"filename": img.replace(".jpg", "")}, white_lines * 255)

            cv2.imwrite("../test_images/processed_images/%(filename)s_yellow_lines.jpg" %
                        {"filename": img.replace(".jpg", "")}, yellow_lines * 255)

            cv2.imwrite("../test_images/processed_images/%(filename)s_combined.jpg" %
                        {"filename": img.replace(".jpg", "")}, combined * 255)

            cv2.imwrite("../test_images/processed_images/%(filename)s_prettyPrintCentriods.jpg" %
                        {"filename": img.replace(".jpg", "")}, prettyPrintCentriods)

testAllImages()
