import tensorflow as tf
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

    imgname = line[0]
    # print(imgname)
    x = cv2.imread('data/' + imgname)
    x = np.array(x)
    y = line[3]
    y = np.array(y)
    # print(y)
    # https://carnd-forums.udacity.com/questions/36897633/fitgenerator----valueerror-error-when-checking-model-input-expected-lambdainput1-to-have-4-dimensions-but-got-array-with-shape-160-320-3
    x = np.expand_dims(x, axis=0)
    y = np.expand_dims(y, axis=0)
    # print(x.shape, y.shape)
    return x, y

# Inspired by
# https://github.com/commaai/research/blob/master/train_steering_model.py

"""
Data generator
# https://keras.io/models/model/
"""


def generate_arrays_from_file(path):
    while 1:
        data = read_csv(path).values
        for line in data:
            x, y = process_line(line)
            # print(x, y)
            yield x, y

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
model.add(ELU())

model.add(Dense(256))

model.add(Dropout(.5))
model.add(ELU())
model.add(Dense(1))

model.compile('adam', 'mse', ['accuracy'])


"""
Train architecture
"""

if __name__ == "__main__":

    print("Training")

    history = model.fit_generator(generate_arrays_from_file(
        'data/driving_log.csv'), samples_per_epoch=8000, nb_epoch=3, verbose=2,)

    """
	Save model
	"""
    model.save('my_model.h5')
