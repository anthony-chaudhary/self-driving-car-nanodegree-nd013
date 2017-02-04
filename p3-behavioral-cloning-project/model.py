import tensorflow as tf
import argparse
from keras.models import Sequential
from keras.layers.core import Dense, Flatten, Dropout, Lambda, Activation
from keras.layers.convolutional import Convolution2D
from keras.layers.advanced_activations import ELU
from keras.layers.normalization import BatchNormalization
from keras.optimizers import Adam
import numpy as np
import cv2
from keras.models import load_model

from pandas.io.parsers import read_csv

tf.python.control_flow_ops = tf

print('Modules loaded.')


"""
Pre processing pipeline

"""


def transform_image(img, ang_range, shear_range, trans_range):
    '''
    This function transforms images to generate new images.
    The function takes in following arguments,
    1- Image
    2- ang_range: Range of angles for rotation
    3- shear_range: Range of values to apply affine transform to
    4- trans_range: Range of values to apply translations over. 

    A Random uniform distribution is used to generate different parameters for transformation

    '''
    # Rotation
    ang_rot = np.random.uniform(ang_range) - ang_range / 2
    # updated to reflect gray pipeline
    rows, cols, ch = img.shape
    Rot_M = cv2.getRotationMatrix2D((cols / 2, rows / 2), ang_rot, 1)

    # Translation
    tr_x = trans_range * np.random.uniform() - trans_range / 2
    tr_y = trans_range * np.random.uniform() - trans_range / 2
    Trans_M = np.float32([[1, 0, tr_x], [0, 1, tr_y]])

    # Shear
    pts1 = np.float32([[5, 5], [20, 5], [5, 20]])

    pt1 = 5 + shear_range * np.random.uniform() - shear_range / 2
    pt2 = 20 + shear_range * np.random.uniform() - shear_range / 2

    pts2 = np.float32([[pt1, 5], [pt2, pt1], [5, pt2]])

    shear_M = cv2.getAffineTransform(pts1, pts2)

    img = cv2.warpAffine(img, Rot_M, (cols, rows))
    img = cv2.warpAffine(img, Trans_M, (cols, rows))
    img = cv2.warpAffine(img, shear_M, (cols, rows))

    return img


def process_line(line):

    X_train = np.empty([0, 160, 320, 3])
    y_train = np.empty([0, 1])

    for i in range(3):

        imgname = line[i].strip()
        X_single_sample = cv2.imread(imgname)
        X_train = np.append(X_train, [X_single_sample], axis=0)

        y_single_sample = line[3]
        # Adjust steering angles
        if i == 1:  # left
            steering_adjust = +.3
        elif i == 2:  # right
            steering_adjust = -.3
        else:
            steering_adjust = 0

        y_single_sample = [y_single_sample + steering_adjust]
        y_train = np.append(y_train, [y_single_sample], axis=0)

    # print(X_train.shape, y_train.shape)

    new_features = []
    new_labels = []

    for feature in X_train:
        new_features.append(transform_image(feature, 10, 2, 1))

    for label in y_train:
        new_labels.append(label)
        # print(label)

    X_train = np.append(X_train, new_features, axis=0)
    y_train = np.append(y_train, new_labels, axis=0)

    #print(X_train.shape, y_train.shape)
    # print(len(X_train))
    return X_train, y_train


"""
Data generator
# https://keras.io/models/model/
"""


def generate_arrays_from_file(path):

    #counter1 = 0
    #counter2 = 0

    while 1:
        #counter1 = counter1 + 1
        #print("While True: ", counter1)

        data = read_csv(path).values
        for line in data:
            #counter2 = counter2 + 1
            #print("For line in data: ", counter2)
            X_train, y_train = process_line(line)
            yield X_train, y_train

    """
    # counter = 0
    while True:
        data = read_csv(path).values

        line_number = np.random.randint(len(data))
        X_train, y_train = process_line(data[line_number])
        #counter = counter + 1
        # print(counter)
        yield X_train, y_train
    """


"""
Model architecture
"""

row, col, ch = 160, 320, 3
model = Sequential()
model.add(Lambda(lambda X_train: X_train / 255.,
                 input_shape=(row, col, ch),
                 output_shape=(row, col, ch)))

# model.add(BatchNormalization())

model.add(Convolution2D(16, 8, 8, subsample=(4, 4), border_mode="same"))
# model.add(Dropout(.3))
# https://keras.io/layers/advanced-activations/#elu
# https://arxiv.org/pdf/1511.07289v1.pdf
model.add(ELU())

model.add(Convolution2D(32, 5, 5, subsample=(2, 2), border_mode="same"))
model.add(ELU())

model.add(Convolution2D(64, 5, 5, subsample=(2, 2), border_mode="same"))

model.add(Flatten())
model.add(Dropout(.3))
model.add(ELU())

model.add(Dense(256))
model.add(Dropout(.5))
model.add(ELU())

model.add(Dense(1))

optimizer_settings = Adam(lr=.0001)
model.compile(optimizer=optimizer_settings, loss='mse')


"""
Train architecture
"""

if __name__ == "__main__":

    print("Training")

    history = model.fit_generator(
        generate_arrays_from_file('driving_log.csv'),
        samples_per_epoch=48000, nb_epoch=10, verbose=2,
        validation_data=generate_arrays_from_file('validation_log2.csv'),
        nb_val_samples=4500)

    """
    Save model
    """
    model.save('my_model.h5')

    model.summary()

    # model.predict()
