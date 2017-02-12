#**Behavioral Cloning** 

##Writeup Template

###Abstract
Our goal is to clone human driving into a computer program using a simulator and deep neural networks. This project is part of the Udacity Self Driving Car Nano Degree program. The simulator and project guidelines are supplied by Udacity.

---

**Behavrioal Cloning Project**

The goals / steps of this project are the following:
* Use the simulator to collect data of good driving behavior
* Build, a convolution neural network in Keras that predicts steering angles from images
* Train and validate the model with a training and validation set
* Test that the model successfully drives around track one without leaving the road
* Summarize the results with a written report

## Rubric
###Here I will consider the [rubric points](https://review.udacity.com/#!/rubrics/432/view) individually and describe how I addressed each point in my implementation.  

---
###Files Submitted & Code Quality

####1. Submission includes all required files and can be used to run the simulator in autonomous mode

My project includes the following files:
* model.py containing the script to create and train the model
* drive.py for driving the car in autonomous mode
* model.h5 containing a trained convolution neural network 
* writeup_report.md or writeup_report.pdf summarizing the results

####2. Submssion includes functional code
Using the Udacity provided simulator and my drive.py file, the car can be driven autonomously around the track by executing 
```sh
python drive.py model.h5
```

####3. Submssion code is usable and readable

The model.py file contains the code for training and saving the convolution neural network. The file shows the pipeline I used for training and validating the model, and it contains comments to explain how the code works.

###Model Architecture and Training Strategy

####1. An appropriate model arcthiecture has been employed

My model consists of a convolution neural network with 5x5 and 3x3 filter sizes and depths between 24 and 64. 

The model includes ELU layers to introduce nonlinearity, and the data is normalized in the model using a Keras lambda layer.

Please see Model architecture at line 411 for more.

####2. Attempts to reduce overfitting in the model

The model contains dropout layers in order to reduce overfitting.

The model was trained and validated on different data sets to ensure that the model was not overfitting (code line 10-16). The model was tested by running it through the simulator and ensuring that the vehicle could stay on the track.

####3. Model parameter tuning

The model used an adam optimizer with a setting of .0001.

####4. Appropriate training data

I decided to use the Udacity provided data set for the training data. I connected an xbox controller to the PC and got a small (~800 lines of data with 3 images per line) additional data set to use for validation. I used a combination of center lane driving, and programatically generated "recovery data" from the left and right camera angles.

For details about how I created the training data, see the next section. 

###Model Architecture and Training Strategy

####1. Solution Design Approach and Final Model Architecture

First we must balance_data()
This format allow for a significant amount of flexibility,    for example, it takes an arbitrary length of data   and returns a randomized line, following the settings   specified by the user. Additionally, the user  could fairly easily change "uniform" to another  numpy distribution to test how different distributions  affect results.  Further it creates a very flexible testing environment   In a sense, the data is "shuffled" after every batch    And, an arbitrary sample size can be selected.

We can then discard less important information using chop_images()

Create recovery definition
First, we expect that the car may for various reasons be outside of the ideal centre of the lane. For example, even if the algorithm was perfect, one can imagine that a car in real life has to somehow get to the centre of the lane. Further, valid reasons such as ice, obstructions, emergency maneuvers, gravel, algorithm errors, sensor errors, etc. will routinely push the car outside of an ideal range.
The theory is that by using images mounted from a different perspective, with an offset to the associated steering angle, help train the model to correct itself.

Generate new images
To help build a robustness to the model, we generate images with various artificial warping and artifacts. 

Convolution, fro example: (24, 5, 5) Stride of (2, 2) Exponential Linear Unit activation (ELU)
Abstract. A convolutional layer “scans” an image selecting sub features. The 24 is the number of “scans” to be outputted. The 5x5 is the size (of pixels in this case) of the scan. And the 2x2 is how many pixels should be skipped in between cans.

The Exponential Linear Unit is an “advanced” activation that handles negatives better than a Rectified Linear Unit (ReLU). 

Neural network outline:

Layer (type)                     Output Shape          Param #     Connected to                     
====================================================================================================
lambda_1 (Lambda)                (None, 80, 320, 3)    0           lambda_input_1[0][0]             
____________________________________________________________________________________________________
convolution2d_1 (Convolution2D)  (None, 38, 158, 24)   1824        lambda_1[0][0]                   
____________________________________________________________________________________________________
elu_1 (ELU)                      (None, 38, 158, 24)   0           convolution2d_1[0][0]            
____________________________________________________________________________________________________
convolution2d_2 (Convolution2D)  (None, 17, 77, 36)    21636       elu_1[0][0]                      
____________________________________________________________________________________________________
elu_2 (ELU)                      (None, 17, 77, 36)    0           convolution2d_2[0][0]            
____________________________________________________________________________________________________
convolution2d_3 (Convolution2D)  (None, 7, 37, 48)     43248       elu_2[0][0]                      
____________________________________________________________________________________________________
elu_3 (ELU)                      (None, 7, 37, 48)     0           convolution2d_3[0][0]            
____________________________________________________________________________________________________
convolution2d_4 (Convolution2D)  (None, 3, 18, 64)     27712       elu_3[0][0]                      
____________________________________________________________________________________________________
elu_4 (ELU)                      (None, 3, 18, 64)     0           convolution2d_4[0][0]            
____________________________________________________________________________________________________
convolution2d_5 (Convolution2D)  (None, 1, 8, 64)      36928       elu_4[0][0]                      
____________________________________________________________________________________________________
elu_5 (ELU)                      (None, 1, 8, 64)      0           convolution2d_5[0][0]            
____________________________________________________________________________________________________
flatten_1 (Flatten)              (None, 512)           0           elu_5[0][0]                      
____________________________________________________________________________________________________
dropout_1 (Dropout)              (None, 512)           0           flatten_1[0][0]                  
____________________________________________________________________________________________________
dense_1 (Dense)                  (None, 1164)          597132      dropout_1[0][0]                  
____________________________________________________________________________________________________
elu_6 (ELU)                      (None, 1164)          0           dense_1[0][0]                    
____________________________________________________________________________________________________
dropout_2 (Dropout)              (None, 1164)          0           elu_6[0][0]                      
____________________________________________________________________________________________________
dense_2 (Dense)                  (None, 100)           116500      dropout_2[0][0]                  
____________________________________________________________________________________________________
elu_7 (ELU)                      (None, 100)           0           dense_2[0][0]                    
____________________________________________________________________________________________________
dense_3 (Dense)                  (None, 50)            5050        elu_7[0][0]                      
____________________________________________________________________________________________________
elu_8 (ELU)                      (None, 50)            0           dense_3[0][0]                    
____________________________________________________________________________________________________
dense_4 (Dense)                  (None, 10)            510         elu_8[0][0]                      
____________________________________________________________________________________________________
elu_9 (ELU)                      (None, 10)            0           dense_4[0][0]                    
____________________________________________________________________________________________________
dense_5 (Dense)                  (None, 1)             11          elu_9[0][0]                      
====================================================================================================
Total params: 850,551
Trainable params: 850,551
Non-trainable params: 0
_______________________


####3. Creation of the Training Set & Training Process

Here is an example image of center lane driving:

![alt text][./examples/center_2016_12_01_13_30_48_287.jpg]

I then used the left and right lanes to generate recovery data:

![alt text][./examples/left_2016_12_01_13_34_03_716.jpg]
![alt text][./examples/right_2016_12_01_13_36_10_800.jpg]

Some examples of augmented images:

![alt text][./examples/gen-features4123.jpg]
![alt text][./examples/gen-features4072.jpg]

Loss history example:
![alt text][./examples/loss-history8289.png]


Angle balancing example:
![alt text][./examples/unique-angles-histogram2815.png]
