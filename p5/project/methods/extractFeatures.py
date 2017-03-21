from supportFunctions import *
import time
import glob
import pickle 

carImages = glob.glob('../../git-ignore/warm-up/vehicles/vehicles/KITTI_extracted/*.png')
notCarImages = glob.glob('../../git-ignore/warm-up/non-vehicles/non-vehicles/GTI/*.png')
cars = []
notcars = []
for image in carImages:
    cars.append(image)
for image in notCarImages:
    notcars.append(image)
print(notcars[10], cars[10])

color_space = 'HSV'  # Can be RGB, HSV, LUV, HLS, YUV, YCrCb
orient = 9   # HOG orientations
pix_per_cell = 8  # HOG pixels per cell
cell_per_block = 2  # HOG cells per block
hog_channel = "ALL"  # Can be 0, 1, 2, or "ALL"
spatial_size = (32, 32)  # Spatial binning dimensions
hist_bins = 32     # Number of histogram bins
spatial_feat = True  # Spatial features on or off
hist_feat = True  # Histogram features on or off
hog_feat = True  # HOG features on or off

t1 = time.time()

car_features = extract_features(cars, color_space=color_space,
                        spatial_size=spatial_size, hist_bins=hist_bins,
                        orient=orient, pix_per_cell=pix_per_cell, 
                        cell_per_block=cell_per_block, 
                        hog_channel=hog_channel, spatial_feat=spatial_feat, 
                        hist_feat=hist_feat, hog_feat=hog_feat)

notcar_features = extract_features(notcars, color_space=color_space, 
                        spatial_size=spatial_size, hist_bins=hist_bins, 
                        orient=orient, pix_per_cell=pix_per_cell, 
                        cell_per_block=cell_per_block, 
                        hog_channel=hog_channel, spatial_feat=spatial_feat, 
                        hist_feat=hist_feat, hog_feat=hog_feat)


# Inspiration https://github.com/jatindhankhar/CarND-Vehicle-Detection/blob/master/Playground.ipynb
t2 = time.time()
print(round(t2-t1, 0), ' seconds to extract features')
# Pickle to save time for testing

features_dictionary = {'car' : car_features, 'notCar' :  notcar_features}

print("Saving features...")

with open('features.pickle', 'wb') as file:
    pickle.dump(features_dictionary, file)

print("Features saved.")