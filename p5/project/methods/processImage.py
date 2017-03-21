from supportFunctions import *
import numpy as np
import time
from sklearn.svm import LinearSVC
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
import pickle
from scipy.ndimage.measurements import label
import matplotlib.image as mpimg
import matplotlib.pyplot as plt


color_space = 'YCrCb'  # Can be RGB, HSV, LUV, HLS, YUV, YCrCb
orient = 9   # HOG orientations
pix_per_cell = 8  # HOG pixels per cell
cell_per_block = 2  # HOG cells per block
hog_channel = "ALL"  # Can be 0, 1, 2, or "ALL"
spatial_size = (16, 16)  # Spatial binning dimensions
hist_bins = 32     # Number of histogram bins
spatial_feat = True  # Spatial features on or off
hist_feat = True  # Histogram features on or off
hog_feat = True  # HOG features on or off

"""
1. Get extracted iamge features
"""
with open('features.pickle', 'rb') as featuresPickle:
    data = pickle.load(featuresPickle)
car_features = data['car']
notcar_features = data['notCar']
print("Features loaded.")

"""
2. Train classifier
"""

X = np.vstack((car_features, notcar_features)).astype(np.float64)
X_scaler = StandardScaler().fit(X)  # Fit a per-column scaler
scaled_X = X_scaler.transform(X)  # Apply the scaler to X

# Define the labels vector
y = np.hstack((np.ones(len(car_features)), np.zeros(len(notcar_features))))

# Split up data into randomized training and test sets
rand_state = np.random.randint(0, 100)
X_train, X_test, y_train, y_test = train_test_split(
    scaled_X, y, test_size=0.2, random_state=rand_state)

print('Using:', orient, 'orientations', pix_per_cell,
      'pixels per cell and', cell_per_block, 'cells per block')
print('Feature vector length:', len(X_train[0]))

svc = LinearSVC()  # Use a linear SVC
t = time.time()  # Check the training time for the SVC
svc.fit(X_train, y_train)
t2 = time.time()
print(round(t2 - t, 2), 'Seconds to train SVC...')
# Check the score of the SVC
print('Test Accuracy of SVC = ', round(svc.score(X_test, y_test), 4))
# Check the prediction time for a single sample
t = time.time()

"""
3. Process image
"""

def process_image(self, image):

    ystart = 400
    ystop = 656
    scale = .8

    final_result, draw_img = find_cars(image, ystart, ystop, scale, svc, X_scaler,
                                       orient, pix_per_cell, cell_per_block, spatial_size, hist_bins)

    # plt.imshow(final_result)
    # plt.show()

    # plt.imshow(draw_img)
    # plt.show()

    return final_result


