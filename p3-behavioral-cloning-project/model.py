import tensorflow as tf
import argparse
from keras.models import Sequential
from keras.layers.core import Dense, Flatten, Dropout, Lambda, Activation
from keras.layers.convolutional import Convolution2D
from keras.layers.advanced_activations import ELU
from keras.layers.normalization import BatchNormalization
import numpy as np
import cv2
from keras.models import load_model

from pandas.io.parsers import read_csv

tf.python.control_flow_ops = tf

print('Modules loaded.')


"""
Pre processing pipeline

"""


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
            steering_adjust = +.2
        elif i == 2:  # right
            steering_adjust = -.1
        else:
            steering_adjust = 0

        y_single_sample = [y_single_sample + steering_adjust]
        y_train = np.append(y_train, [y_single_sample], axis=0)

    #print(X_train.shape, y_train.shape)
    return X_train, y_train


"""
Data generator
# https://keras.io/models/model/
"""


def generate_arrays_from_file(path):
    while 1:
        data = read_csv(path).values
        for line in data:
            X_train, y_train = process_line(line)
            yield X_train, y_train

"""
Model architecture
"""

row, col, ch = 160, 320, 3
model = Sequential()
model.add(Lambda(lambda x: x / 127.5 - 1.,
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
model.add(Dropout(.2))
model.add(ELU())

model.add(Dense(512))
model.add(Dropout(.5))
model.add(ELU())

model.add(Dense(1))

model.compile('adam', 'mse')


"""
Train architecture
"""

if __name__ == "__main__":

    print("Training")

    history = model.fit_generator(
        generate_arrays_from_file('driving_log.csv'),
        samples_per_epoch=24000, nb_epoch=2, verbose=2,
        validation_data=generate_arrays_from_file('validation_log2.csv'),
        nb_val_samples=1150)

    """
    Save model
    """
    model.save('my_model.h5')

    model.summary()

    # model.predict()
