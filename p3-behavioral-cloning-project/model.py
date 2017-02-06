import tensorflow as tf
from keras.models import Sequential
from keras.layers.core import Dense, Flatten, Dropout, Lambda
from keras.layers.convolutional import Convolution2D
from keras.layers.advanced_activations import ELU
from keras.optimizers import Adam
import numpy as np
import cv2
import matplotlib.pyplot as plt
from pandas.io.parsers import read_csv
from keras.callbacks import EarlyStopping
tf.python.control_flow_ops = tf
from keras.callbacks import ModelCheckpoint
import matplotlib.gridspec as gridspec
import matplotlib.image as mpimg
import scipy.misc

tf.python.control_flow_ops = tf

print('Modules loaded.')

"""
Hyperparemters
"""
# Nerual Network Training
BATCH_SIZE = 8
EPOCHS = 5
SAMPLES_EPOCH = 30000
VALIDATION_SAMPLES = 3000

# Steering control
CHANCE_REJECT_ZERO_ANGLE = .50
MINIMUM_REJECT_ANGLE = .001

# Bias control
BIAS_CONTROL = .01

# Recovery control
RECOVERY_OFFSET = .25

# Preprocessing
FEATURE_GENERATION_MULTIPLE = 2


"""
Test suite
"""

# run_once credit 
# http://stackoverflow.com/questions/4103773/efficient-way-of-having-a-function-only-execute-once-in-a-loop

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


"""
Pre processing pipeline
"""

# Credit forum fellow from P2
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


num_classes = 0
track_angles = []

def process_line(data):

    X_train = np.empty([0, 80, 320, 3])
    y_train = np.empty([0, 1])

    # each batch will contain 3 images from data and 3 generated images
    # so a batch of 1 will contain 6 images
    # and a batch of 2 will contain 12 etc

    batch_size = 8  # batch_size * 15 = real batch size TODO refactor to be more direct

    for batch in range(batch_size):
        # get a line number
        line_number = np.random.randint(len(data))
        # print("length of data", len(data))
        # get a line object
        line = data[line_number]

        # Reject if angles less than abs(.1)
        number_of_angles_rejected = 0
        bias_counter = 0
        steering_angle_test = line[3]
        rejection_prob = .5  # %


        """
        bias = 1. / (bias_counter + 1.)
        threshold = np.random.uniform()
        # print(threshold)
        # balance data
        # Credit https://medium.com/@mohankarthik/cloning-a-car-to-mimic-human-driving-5c2f7e8d8aff#.k8w0jbmsm
        while (abs(steering_angle_test + bias)) > threshold:
            line_number = np.random.randint(len(data))  # get new line
            line = data[line_number]
            steering_angle_test = line[3]

            if (abs(steering_angle_test + bias)) < threshold:
                break
            bias_counter = bias_counter + 1
        """
        # while steering angle close to 0 get a new line
        while abs(steering_angle_test) < .0001:   # ~3600*3 samples

            # Chance to not reject the angle
            if np.random.random_sample(1) > rejection_prob:
                break

            line_number = np.random.randint(len(data))  # get new line
            line = data[line_number]
            steering_angle_test = line[3]  # check angle

            # once we have correct angle, break out of loop  try  AND < .1
            if abs(steering_angle_test) > .0001:
                break

            # print("Angles rejected ", number_of_angles_rejected)
            number_of_angles_rejected = number_of_angles_rejected + 1

        for i in range(3):

            imgname = line[i].strip()
            X_single_sample = cv2.imread(imgname)

            # Credit https://github.com/navoshta/behavioral-cloning
            # print("X original  shape", X_single_sample.shape)
            top = int(.4 * X_single_sample.shape[0])
            bottom = int(.1 * X_single_sample.shape[0])
            X_single_sample = X_single_sample[top:-bottom, :]

            #print("X new shape", X_single_sample.shape)

            """
            save images for visualization if required
            """
            # scipy.misc.imsave('figs/orginal' +
                              # str(np.random.randint(999)) + '.jpg', X_single_sample)

            X_train = np.append(X_train, [X_single_sample], axis=0)

            y_single_sample = line[3]
            # Adjust steering angles
            if i == 1:  # left
                steering_adjust = +.25
            elif i == 2:  # right
                steering_adjust = -.25
            else:
                steering_adjust = 0

            y_single_sample = [y_single_sample + steering_adjust]
            track_angles.append(y_single_sample)
            y_train = np.append(y_train, [y_single_sample], axis=0)


            # print(X_train.shape, y_train.shape)

    # Check_text = "Before image generation, shapes: "
    # check_Shape(Check_text, X_train, y_train)

    # Credit https://github.com/navoshta/behavioral-cloning
    #flip_indices = random.sample(range(X_train.shape[0]), int(X_train.shape[0] / 2))
    #X_train[flip_indices] = X_train[flip_indices, :, ::-1, :]
    #y_train[flip_indices] = -y_train[flip_indices]

    new_features = []
    new_labels = []

    for i in range(2):
        for feature in X_train:
            new_features.append(transform_image(feature, 15, 3, 2))
            # save images
            # scipy.misc.imsave('figs/gen' +
                             # str(np.random.randint(999)) + '.jpg', new_features[i])
        for label in y_train:
            new_labels.append(label)
            track_angles.append(label)
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
init_type = "glorot_uniform"
border_mode_type = "valid"

row, col, ch = 80, 320, 3
model = Sequential()
model.add(Lambda(lambda X_train: X_train / 127.5 - 1,
                 input_shape=(row, col, ch),
                 output_shape=(row, col, ch)))

model.add(Convolution2D(24, 5, 5,
                        subsample=(2, 2),
                        border_mode="valid",
                        init=init_type))
#convout1 = ELU()
model.add(ELU())

model.add(Convolution2D(36, 5, 5,
                        subsample=(2, 2),
                        border_mode="valid",
                        init=init_type))
model.add(ELU())

model.add(Convolution2D(48, 3, 3,
                        subsample=(2, 2),
                        border_mode="valid",
                        init=init_type))
model.add(ELU())

model.add(Convolution2D(64, 3, 3,
                        subsample=(1, 1),
                        border_mode="valid",
                        init=init_type))
model.add(ELU())


model.add(Flatten())
model.add(Dropout(.5))

model.add(Dense(256,
                bias=True,
                init=init_type
                ))
model.add(ELU())
model.add(Dropout(.5))

model.add(Dense(100, init=init_type))
model.add(ELU())
model.add(Dropout(.5))

model.add(Dense(50, init=init_type))
model.add(ELU())
model.add(Dropout(.5))

model.add(Dense(10, init=init_type))
model.add(ELU())
model.add(Dropout(.5))

model.add(Dense(1))


optimizer_settings = Adam(lr=.0001)
model.compile(optimizer=optimizer_settings, loss='mse')

early_stopping = EarlyStopping(monitor='val_loss', patience=4)

checkpointer = ModelCheckpoint(
    filepath="temp-model/weights.hdf5", verbose=1, save_best_only=True)

"""
Train architecture
"""

if __name__ == "__main__":

    print("Training")

    '''
    saves the model weights after each epoch if the validation loss decreased
    Credit https://keras.io/callbacks/#create-a-callback
    '''


    history = model.fit_generator(
        generate_arrays_from_file('driving_log.csv'),
        samples_per_epoch=3000, nb_epoch=20, verbose=2,
        callbacks=[early_stopping, checkpointer],
        validation_data=generate_arrays_from_file('validation_log2.csv'),
        nb_val_samples=1000)

    """
    Save model
    """
    model.save('my_model.h5')

    print("Model saved.")
   
    model.summary()
    print("Complete.")

    # model.predict()
    # summarize history for loss
    plt.plot(history.history['loss'])
    plt.plot(history.history['val_loss'])
    plt.title('model loss')
    plt.ylabel('loss')
    plt.xlabel('epoch')
    plt.legend(['train', 'test'], loc='upper left')
    plt.show()

    #print(track_angles)
    track_angles = np.around((np.asarray(track_angles)), decimals=3)
    print(track_angles)
    print(num_classes)

    plt.hist(track_angles)  # THIS WORKS OMG
    plt.title("Gaussian Histogram")
    plt.xlabel("Value")
    plt.ylabel("Frequency")

    fig = plt.gcf()
    plt.show()
    fig.savefig('foo.png')
    
    print(min(track_angles))
    print(max(track_angles))