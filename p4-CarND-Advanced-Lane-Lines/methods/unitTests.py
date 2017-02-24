

def testFindPoints():
	"""
	Purpose: Test if calibration function is working.
	Inputs: Harcoded calibration path.
	Outputs: Prints points.

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
	Purpose:
	Inputs: 
	Outputs: 

	"""

	from calibration import findPoints
	imagesPath = "../camera_cal/calibration*.jpg"
	objpoints, imgpoints = findPoints.findPoints(imagesPath)

	from calibration import findPoints
	import matplotlib.image as mpimg
	image = mpimg.imread('../test_images/test1.jpg')
	ret, mtx, dist, rvecs, tvecs = findPoints.calibrate(image, objpoints, imgpoints)

	return print("Success")

testCalibration()