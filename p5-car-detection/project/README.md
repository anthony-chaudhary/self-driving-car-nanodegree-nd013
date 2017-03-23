
# Vehicle Detection Project  *Work in progress

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
[image6]: 
[image7]: ./test_images/processed_images/test6_processed.jpg
[video1]: ./processed_project_video.mp4

## [Rubric](https://review.udacity.com/#!/rubrics/513/view) Points
Here I will consider the rubric points individually and describe how I addressed each point in my implementation.  

---
### Writeup / README

#### 1. Provide a Writeup / README that includes all the rubric points and how you addressed each one.
You're reading it. :)


### Histogram of Oriented Gradients (HOG)

#### 1. HOG features extracted from the training images.

The code for this is in:
`methods/extractFeatures.py`

Once the features were extracted I saved it to a pickle file to save computation time later on.

#### 2. Explain how you settled on your final choice of HOG parameters.

I tested HSV, LUV, HLS, YUV, and YCrCB. I got the best results, at the expense of computation speed, when I used YCrCB with all the colour channels. It was generally 99.99% on the test set. While I don't believe 99.99% is an accurate representation of the overall  effectiveness of it, it was the best within the scope of available options. Specifically, in unit tests with HSV and channel 2 (V), an SVC classifier got a test accuracy of 94%, and the S channel got 97%. All the colour channels got 99%. 

I tested a few different `orient` values. 9 seemed to perform well, and based on suggestions and readings from the course it was a logical choice.

#### 3. Trained a classifier using  HOG and color features.

`methods\processImage.py`

I tried both an SVC and MLP classifier. The [Multi-layer Perceptron](http://scikit-learn.org/stable/modules/generated/sklearn.neural_network.MLPClassifier.html) classifier performed noticeable better, detecting ~2x as many windows as the SVC.

I used default parameters for the MLP classifier as it seemed to be quite effective. Perhaps for further improvements I would look at tweaking the activation functions, learning rate etc.


### Sliding Window Search

#### 1. A sliding window search.  How did you decide what scales to search and how much to overlap windows? Show some examples of test images to demonstrate how your pipeline is working.

`methods\supportFunctions.py => find_cars()`
I decided on scales of `[.85, 1, 1.1, 1.2]`.

Using suggestions from the lessons, I used a step over method instead of an overlap percent. With 2 cells per block being stepped over.
The key line here is: `nxsteps = (nxblocks - nblocks_per_window) // cells_per_step`
For example at a scale of 1.0, there are 144 x blocks and 68 x steps.

I used the suite of test images to look at results of various scales.
I found smaller scales were more sensitive and would detect more windows. However they naturally were also more likely to introduce more noise.
I tried scales lower than .85, but generally found they introduced too much undesirable noise. Here is an example of an image with all scales overlayed:

![alt text][image3]
![alt text][image4]

#### 2. What did you do to optimize the performance of your classifier?

In `methods\supportFunctions.py => combineBoundingBoxes()`
I applied a threshold to only accept pixels less than or equal to the threshold. 

I also applied a threshold to only render labels equal to or larger than 70 pixels wide and 50 pixels tall. This is under the assumption that a car would generally be at least that large. In real life of course this would easily break down as you may wish to detect road objects other than cars. Within the scope of car detection however it did help reduce false positives.

---

### Video Implementation

#### 1. Here's a [link to my video result](./processed_project_video.mp4)

#### 2. Filter for false positives combine overlapping bounding boxes.

From the positive detections I created a heatmap and then thresholded that map to identify vehicle positions.  I then used `scipy.ndimage.measurements.label()` to identify individual blobs in the heatmap, assuming each blob corresponded to a vehicle.  I constructed bounding boxes to cover the area of each blob detected.  

Further, I created a class `detectClass.Detect()` that can store heatmaps accross vidoe frames. For ever 5 frames the heatmaps are combined. This yields a smoother result and increases confidence in detection.


Here's an example result showing the binary combined heatmap:
![alt text][image5]

Further examples are available in the `/test-images` folder.

### Here the resulting bounding boxes are drawn onto the last frame in the series:
![alt text][image7]


---

### Discussion

#### 1. Briefly discuss any problems / issues you faced in your implementation of this project.  Where will your pipeline likely fail?  What could you do to make it more robust?

* I focused the search on the right side of the image, something that would be impractical for a real life implementation.
* The current setup uses the CPU primarily for computation and is extremely slow.

I would like to explore using further deep learning methods (beyond the MLP classifier) to tackle this.





