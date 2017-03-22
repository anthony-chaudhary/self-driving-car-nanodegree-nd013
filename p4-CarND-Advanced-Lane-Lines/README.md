
**Advanced Lane Finding Project**

The goals / steps of this project are the following:

* Compute the camera calibration matrix and distortion coefficients given a set of chessboard images.
* Apply a distortion correction to raw images.
* Use color transforms, gradients, etc., to create a thresholded binary image.
* Apply a perspective transform to rectify binary image ("birds-eye view").
* Detect lane pixels and fit to find the lane boundary.
* Determine the curvature of the lane and vehicle position with respect to center.
* Warp the detected lane boundaries back onto the original image.
* Output visual display of the lane boundaries and numerical estimation of lane curvature and vehicle position.

[//]: # (Image References)

[Undistorted]: ./output_images/figure_1.png "Undistorted"
[Road Undistorted]: ./output_images/undistort_car_output.png "Road Undistorted"

[Centriods]: ./output_images/test2_prettyPrintCentriods.jpg "Centriods"
[Test result]: ./output_images/test2_processed.jpg "Test result"
[Binary magnitude]: ./output_images/test2_mag_binary.jpg "Binary magnitude"
[Binary hls]: ./output_images/test2_hls_select.jpg "Binary hls"
[Binary gradient x]: ./output_images/test2_gradx.jpg "Binary gradient x"
[Binary direction]: ./output_images/test2_dir_binary.jpg "Binary direction"
[Binary combined]: ./output_images/test2_combined.jpg "Binary combined"


[Mask Example]: ./output_images/mask_example.jpg "Mask Example"

[image3]: ./examples/binary_combo_example.jpg "Binary Example"
[birds_eye]: ./output_images/birds_eye.png "Birds_eye Example"
[image5]: ./examples/color_fit_lines.jpg "Fit Visual"
[image6]: ./examples/example_output.jpg "Output"
[video1]: ./project_video.mp4 "Video"

---
### Writeup / README

### Camera Calibration

#### 1. Camera matrix and distortion coefficients:

`methods\calibration\calibrate.py`
`methods\calibration\findPoints.py`

I start by preparing "object points", which will be the (x, y, z) coordinates of the chessboard corners in the world. Here I am assuming the chessboard is fixed on the (x, y) plane at z=0, such that the object points are the same for each calibration image.  Thus, `objp` is just a replicated array of coordinates, and `objpoints` will be appended with a copy of it every time I successfully detect all chessboard corners in a test image.  `imgpoints` will be appended with the (x, y) pixel position of each of the corners in the image plane with each successful chessboard detection.  

I then used the output `objpoints` and `imgpoints` to compute the camera calibration and distortion coefficients using the `cv2.calibrateCamera()` function.  I applied this distortion correction to the test image using the `cv2.undistort()` function and obtained this result: 

![alt text][Undistorted]


### Pipeline (single images)

#### 1. Distortion-corrected image:
![alt text][Road Undistorted]

To spot the differences, look closely at the hood, and the white car.
In the original image the hood appears horizontal, while it's actually curved,
as shown in the right picture.
In the original image the white car appears further away than it actually is,
as shown in the right picture.

####2. Color transforms, gradients or other methods to create a thresholded binary image.

I used a combination of color and gradient thresholds to generate a binary image.
The key functions for this are all within `methods/colorGradientThreshold/`
The methods are use in `methods/processImage.py`.

The critical part is:
```
combined[(gradx == 1) | (hls_select == 1) | (yellow_lines == 1) | (
        (mag_binary == 1) & (dir_binary == 0)) | white_lines == 1] = 1
```
For example if the magnitude and direction functions agree,
or when HLS sees a pixel, add that pixel to the combined image.

I tried a few combinations here and found this to be the best so far.

Binary, meaning that the pixel value is either on or off.

Some example outputs:
##### Magnitude
![Binary magnitude][Binary magnitude]

##### HLS (Hue, light, saturation)
![Binary hls][Binary hls]

##### Gradient x
![Binary gradient x][Binary gradient x]

##### Direction
![Binary direction][Binary direction]

The direction one "looks" terrible, but surprisingly helps with performance.

##### Combined
![Binary combined][Binary combined]


#### 3. Perspective transform and provide an example of a transformed image.

`methods\perspectiveTransform.py\perspectiveTransform()`

    Purpose: Transform image to bird's eye view.
    Inputs: Img array, source points, destintation points
    Outputs: warpedImage array

The source image has the lines going to the horizon,
where as I want them parallel.
I therefore kept the bottom points similar to each other, and mainly focused
on shifting the topleft and topright points.

I used the imagewidth/10 as a basis point possible,
and kept the src/dst points aligned with each other to prevent curving.

```
src = np.float32([[576, 450], [704, 450],
                  [128, 700], [1152, 700]])

dst = np.float32([[256, 128], [1024, 128],
                  [256, 720], [1024, 720]])
```

I verified that my perspective transform was working as expected by drawing the `src` and `dst` points onto a test image and its warped counterpart to verify that the lines appear parallel in the warped image.

![alt text][birds_eye]

####4. Identified lane-line pixels and fit their positions with a polynomial:

`methods\laneDetection\centriods.py\find_window_centriods()`
`methods\laneDetection\customPolyFit.py\poly()`
`methods\laneDetection\regionOfInterest.py\region_of_interest()`

I created a two polygon image mask to create cones around each lane:
I found this really helped, and I believe it's reasonably scalable to forward facing lines. 
I also played with the `window_width, window_height, and margin` to find optimal settings.

![Mask example][Mask example]

Here is an example of the centriod output.

![Centriods][Centriods]

#### 5. Calculated the radius of curvature of the lane and the position of the vehicle with respect to center.

`methods\laneDetection\curveCalculations\radiusOfCurvature()`
`methods\laneDetection\offset\calculateCarOffset()`

For the radius of the curvature,
I used the already calculated left and right points from `find_window_centriods()`.
`super_fun_y_points` are found by using the `window_height` variable,
this allows the points to scale automatically with a chosen window height and speeds testing.

For the offset,
I used the center of the screen as the ground truth, and compared the first value of the 
left and right centriod points.

####6. Provide an example image of your result plotted back down onto the road such that the lane area is identified clearly.

![Test result][Test result]

---

### Pipeline (video)

#### 1. Provide a link to your final video output.  Your pipeline should perform reasonably well on the entire project video (wobbly lines are ok but no catastrophic failures that would cause the car to drive off the road!).

`methods\processVideo.py`
`methods\processImage.py\process_image()`

Here's a [link to my video result](./output_video/complete_project_video.mp4)

---

### Discussion

#### 1. Briefly discuss any problems / issues you faced in your implementation of this project.  Where will your pipeline likely fail?  What could you do to make it more robust?

The pipeline is likely to fail in more challenging curves / conditions.
I would like to consider:
-> Using functions in `methods\tracking\lineClass.py\Line()` to "smooth" detection.
-> More robust parameter selection, for example, the masking, or the thresholding functions
-> More robust processing performance. I was able to get a 3x speedup simply removing some unnecessarily repeated calculations, and would like to improve this further. 

I also performed fairly extensive testing in the `methods/unitTests.py file`,
the following completly self enclosed tests are available:

```
testFindPoints()
testCalibration()
testDrawChessBoardCorners()
testBirdsEyeView()
testThresholds()
peakLaneHistogram()
testLaneDetection()
testAllImages()
```

I felt this gave me a very steady and robust way to test the pipeline. While there are many shortcomings, I feel within the scope of the project the tests were able to given an excellent idea of what is going on with the system.
