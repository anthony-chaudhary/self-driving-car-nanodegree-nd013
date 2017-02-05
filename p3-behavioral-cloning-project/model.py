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
from keras.regularizers import l2, activity_l2

from pandas.io.parsers import read_csv

tf.python.control_flow_ops = tf

print('Modules loaded.')


"""
Pre processing pipeline

"""
# credit
# http://stackoverflow.com/questions/4103773/efficient-way-of-having-a-function-only-execute-once-in-a-loop

"""
Test suite
"""


def run_once(f):
    def wrapper(*args, **kwargs):
        if not wrapper.has_run:
            wrapper.has_run = True
            return f(*args, **kwargs)
    wrapper.has_run = False
    return wrapper


@run_once
def check_Shape(check_text, X_train, y_train):
    print(check_text, X_train.shape, y_train.shape)


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


def process_line(data):

    # TODO Add some kind of rejection to more than 50% angles of less than
    # abs(.1) in any batch to reduce bias of driving straight.
    X_train = np.empty([0, 160, 320, 3])
    y_train = np.empty([0, 1])

    # each batch will contain 3 images from data
    # and 3 generated images
    # so a batch of 1 will contain 6 images
    # and a batch of 2 will contain 12 etc

    batch_size = 8  # batch_size * 6 = real batch size TODO refactor to be more direct

    for batch in range(batch_size):
        # get a line number
        line_number = np.random.randint(len(data))
        # get a line object
        line = data[line_number]

        # Reject if angles less than abs(.1)
        number_of_angles_rejected = 0
        steering_angle_test = line[3]
        rejection_prob = .4

        # while steering angle close to 0 get a new line
        while abs(steering_angle_test) < .1:

            # Chance to not reject the angle
            if np.random.random_sample(1) > rejection_prob:
                break

            line_number = np.random.randint(len(data))  # get new line
            line = data[line_number]
            steering_angle_test = line[3]  # check angle

            # once we have correct angle, break out of loop
            if abs(steering_angle_test) > .1:
                break

            # print("Angles rejected ", number_of_angles_rejected)
            number_of_angles_rejected = number_of_angles_rejected + 1

        for i in range(3):

            imgname = line[i].strip()
            X_single_sample = cv2.imread(imgname)
            X_train = np.append(X_train, [X_single_sample], axis=0)

            y_single_sample = line[3]
            # Adjust steering angles
            if i == 1:  # left
                steering_adjust = +.28
            elif i == 2:  # right
                steering_adjust = -.28
            else:
                steering_adjust = 0

            y_single_sample = [y_single_sample + steering_adjust]
            y_train = np.append(y_train, [y_single_sample], axis=0)

            # print(X_train.shape, y_train.shape)

    # Double features with generated

    # Check_text = "Before image generation, shapes: "
    # check_Shape(Check_text, X_train, y_train)

    new_features = []
    new_labels = []

    for i in range(2):
        for feature in X_train:
            new_features.append(transform_image(feature, 15, 2, 2))

        for label in y_train:
            new_labels.append(label)
            # print(label)

    X_train = np.append(X_train, new_features, axis=0)
    y_train = np.append(y_train, new_labels, axis=0)

    Check_text = "After image generation, shapes: "
    check_Shape(Check_text, X_train, y_train)

    return X_train, y_train


"""
Data generator
# https://keras.io/models/model/
"""


def generate_arrays_from_file(path):
    """
    # counter1 = 0
    # counter2 = 0

    while 1:
        # counter1 = counter1 + 1
        # print("While True: ", counter1)

        data = read_csv(path).values
        for line in data:
            # counter2 = counter2 + 1
            # print("For line in data: ", counter2)
            X_train, y_train = process_line(line)
            yield X_train, y_train

    """
    # counter = 0
    # While samples to generate does not equal 0 run this generator

    while True:
        # load the labels and file paths to images
        data = read_csv(path).values
        # All pre processing including batch size and number of generated
        # images.
        X_train, y_train = process_line(data)
        # Yied results back to fit_generator
        yield X_train, y_train


"""
Model architecture
"""

row, col, ch = 160, 320, 3
model = Sequential()
model.add(Lambda(lambda X_train: X_train / 127.5 - 1,
                 input_shape=(row, col, ch),
                 output_shape=(row, col, ch)))

# model.add(BatchNormalization())


model.add(Convolution2D(24, 5, 5, subsample=(2, 2), border_mode="valid"))
model.add(ELU())

model.add(Convolution2D(36, 5, 5, subsample=(2, 2), border_mode="valid"))
model.add(ELU())

model.add(Convolution2D(48, 3, 3, subsample=(2, 2), border_mode="valid"))
model.add(ELU())

model.add(Convolution2D(64, 3, 3, subsample=(1, 1), border_mode="valid"))
model.add(ELU())

model.add(Convolution2D(64, 3, 3, subsample=(1, 1), border_mode="valid"))
model.add(ELU())

model.add(Flatten())
model.add(Dropout(.5))

model.add(Dense(1164, bias=True))
model.add(ELU())
model.add(Dropout(.5))

model.add(Dense(100))
model.add(ELU())
model.add(Dropout(.5))

model.add(Dense(50))
model.add(ELU())
model.add(Dropout(.5))

model.add(Dense(10))
model.add(ELU())
model.add(Dropout(.5))

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
        samples_per_epoch=72000, nb_epoch=10, verbose=1,
        validation_data=generate_arrays_from_file('validation_log2.csv'),
        nb_val_samples=7200)

    """
    Save model
    """
    model.save('my_model.h5')

    model.summary()

    # model.predict()
