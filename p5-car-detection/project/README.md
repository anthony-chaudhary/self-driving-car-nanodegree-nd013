
# Vehicle Detection Project

The goals / steps of this project are the following:

* Perform a Histogram of Oriented Gradients (HOG) feature extraction on a labeled training set of images and train a classifier Linear SVM classifier
* Implement a sliding-window technique and use your trained classifier to search for vehicles in images.
* Run your pipeline on a video stream (start with the test_video.mp4 and later implement on full project_video.mp4) and create a heat map of recurring detections frame by frame to reject outliers and follow detected vehicles.
* Estimate a bounding box for vehicles detected.

[//]: # (Image References)
[image1]: 
[image2]: 
[image3]: ./test_images/processed_images/test1_all_detections.jpg
[image4]: ./test_images/processed_images/test6_all_detections.jpg
[image5]: ./test_images/processed_images/test6_heatmap.jpg
[image6]: ./examples/labels_map.png
[image7]: ./test_images/processed_images/test6_processed.jpg
[video1]: ./processed_project_video.mp4

## [Rubric](https://review.udacity.com/#!/rubrics/513/view) Points
Here I will consider the rubric points individually and describe how I addressed each point in my implementation.  

---
### Writeup / README

#### 1. Provide a Writeup / README that includes all the rubric points and how you addressed each one.
You're reading it. :)


### Histogram of Oriented Gradients (HOG)

##### 1. HOG features extracted from the training images.

The code for this is in:
`methods/extractFeatures.py`

Once the features were extracted I saved it to a pickle file to save computation time later on.

I started by reading in all the `vehicle` and `non-vehicle` images. 

I then explored different color spaces and different `skimage.hog()` parameters (`orientations`, `pixels_per_cell`, and `cells_per_block`).  I grabbed random images from each of the two classes and displayed them to get a feel for what the `skimage.hog()` output looks like.


#### 2. Explain how you settled on your final choice of HOG parameters.





#### 3. Trained a classifier using  HOG and color features.

`methods\processImage.py`



### Sliding Window Search

#### 1. A sliding window search.  How did you decide what scales to search and how much to overlap windows?

`methods\supportFunctions.py => find_cars()`

I used the suite of test images to look at results of various scales.

![alt text][image3]


#### 2. Show some examples of test images to demonstrate how your pipeline is working.  What did you do to optimize the performance of your classifier?

Ultimately I searched on scales `[.8, .9, .95, 1, 1.05, 1.1, 1.2]` using YCrCb 3-channel HOG features plus spatially binned color and histograms of color in the feature vector, which provided a nice result.  Here are some example images:

![alt text][image4]

---

### Video Implementation

#### 1. Here's a [link to my video result](./processed_project_video.mp4)


#### 2. Describe how (and identify where in your code) you implemented some kind of filter for false positives and some method for combining overlapping bounding boxes.

I recorded the positions of positive detections in each frame of the video.  From the positive detections I created a heatmap and then thresholded that map to identify vehicle positions.  I then used `scipy.ndimage.measurements.label()` to identify individual blobs in the heatmap.  I then assumed each blob corresponded to a vehicle.  I constructed bounding boxes to cover the area of each blob detected.  

Here's an example result showing the heatmap from a series of frames of video, the result of `scipy.ndimage.measurements.label()` and the bounding boxes then overlaid on the last frame of video:

### Binary heatmap:

![alt text][image5]

### Here is the output of `scipy.ndimage.measurements.label()` on the integrated heatmap from all six frames:
![alt text][image6]

### Here the resulting bounding boxes are drawn onto the last frame in the series:
![alt text][image7]


---

### Discussion

#### 1. Briefly discuss any problems / issues you faced in your implementation of this project.  Where will your pipeline likely fail?  What could you do to make it more robust?

* I focused the search on the right side of the image, something that would be impractical for a real life implmentation.
* The current setup uses the CPU primarily for computation and is extremely slow.

I would like to explore using a deep learning method to tackle this.





