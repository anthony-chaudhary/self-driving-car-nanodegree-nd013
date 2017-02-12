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
from keras.callbacks import ModelCheckpoint
tf.python.control_flow_ops = tf
print('Modules loaded.')

"""
Hyperparemters
"""
# Nerual Network Training
EPOCHS = 12
SAMPLES_EPOCH = 10000
VALIDATION_SAMPLES = 3000
LEARNING_RATE = .0001

BATCH_SIZE = 2  # Size of batch
# 3*BATCH_SIZE*FEATURE_GENERATION_MULTIPLE == total_batch size

# Recovery control
# Float, handled as absolute value, valid range from 0 -> 1
RECOVERY_OFFSET = .25

# Preprocessing
FEATURE_GENERATION_MULTIPLE = 1

# Data balancing
UNIFORM_TEST_MAX = .7  # Max absolute value of data balanced
ZEROS = .50  # Percent chance of getting zeros, higher chance of getting zeros
# NOTE as we add (+/-) for recover angles, the net number of zeros
# will be ~ ZEROS / 3 ie .5 zeros setting will result in ~.16

# The program relies on a variety of random functions
# For detailed testing, use the seed value as needed
# A seed of 8373 should return 91 on the test below

SEED = 8373
custom_random = np.random.RandomState(seed=SEED)
print(custom_random.randint(0, 100))


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

# TODO add further testing functions here


"""
Pre processing pipeline
"""

# Track angles is used to help keep track of angles generated throughout
track_angles = []


def augment_brightness_camera_images(image):
    """
    Purpose: Adjust brightness of image
    Inputs: A 3D matrix, generall in the form [x,y,colours]
    Outputs: Same
    """
    # credit https://medium.com/@vivek.yadav

    image = image.astype(np.uint8)
    image1 = cv2.cvtColor(image, cv2.COLOR_RGB2HSV)
    random_bright = .25 + np.random.uniform()
    # print(random_bright)
    image1[:, :, 2] = image1[:, :, 2] * random_bright
    image1 = cv2.cvtColor(image1, cv2.COLOR_HSV2RGB)
    return image1


def transform_image(img, ang_range, shear_range, trans_range):
    '''
    This function transforms images to generate new images.
    The function takes in following arguments,
    1- Image
    2- ang_range: Range of angles for rotation
    3- shear_range: Range of values to apply affine transform to
    4- trans_range: Range of values to apply translations over.

    A Random uniform distribution is used to generate different parameters 
    for transformation

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

    # added for brightness
    img = augment_brightness_camera_images(img)

    return img


def balance_data(data):
    """
    Purpose: Balance the data given a random distribution
    Inputs: A CSV file
    Outputs: A line of the data file

    This format allow for a sigfnicant amount of flexablity,
    for example, it takes an arbitrary length of data
    and returns a randomized line, following the settings
    specified by the user. Additionally, the user
    could fairly easily change "uniform" to another
    numpy distribution to test how different distributions
    effect results.
    """

    uniform_test_max = UNIFORM_TEST_MAX
    max_tries = 1  # This is a starting value for a timeout function
    zero_flag = False
    data_length = len(data)

    # Test if zero is less than flag set in settings
    if custom_random.random_sample(1) <= ZEROS:
        zero_flag = True

    # Main loop entry
    while True:

        # This block performs balance test and selects steering angle
        uniform_test = (custom_random.uniform(0, uniform_test_max))
        line_number = custom_random.randint(len(data))
        line = data[line_number]
        steering_angle = line[3]

        # Return steering angle if zero flag is true, else resume loop
        if zero_flag is True:
            if steering_angle == 0:
                return line
            else:
                continue
        else:
            # Handle non-zero angles
            if steering_angle != 0 and abs(steering_angle) <= uniform_test:
                return line
            else:  # Handle exceptions
                if max_tries > data_length:
                    print("Exceeded search time, skipping angle:",
                          steering_angle, "Data length:", data_length)
                    return line
                else:
                    continue


def recovery(line):
    """
    Purpose: Create a "recovery" angle based off left/right images
    Inputs: A line, preferably given by balance_data()
    Outputs: Y_train, a [3, 1] matrix
    """

    y_train = np.empty([0, 1])  # Create a new placeholder matrix
    y_single_sample = line[3]  # Select a line from the data
    y_single_sample = round(y_single_sample, 3)  # Round sample to smooth data

    # print(y_single_sample)
    assert type(y_single_sample) == float
    # print(type(y_single_sample))
    new_labels = []

    for i in range(3):

        # left angles
        if i == 1:
            # print("Left", line[i])   #Refactor to an ASSERT
            steering_adjust = +RECOVERY_OFFSET
            # print(type(steering_adjust))
            steering_correction = min(
                1, y_single_sample + steering_adjust)
            assert steering_correction != 1.01, steering_correction != -1
            # print(steering_correction)

        # Right angles
        elif i == 2:
            # print("Right", line[i])
            steering_adjust = -RECOVERY_OFFSET
            steering_correction = max(-1,
                                      y_single_sample + steering_adjust)
            assert steering_correction != -1.01, steering_correction != +1

        # Center angles
        elif i == 0:
            # print("Center", line[i])
            steering_correction = y_single_sample

        track_angles.append(steering_correction)
        new_labels.append([steering_correction])

    # print("new labels", new_labels)
    y_train = np.append(y_train, new_labels, axis=0)
    # print(y_train)

    return y_train


def chop_images(line):
    """
    Purpose: Discard less important information
    Inputs: A line, preferably given by balance_data()
    Outputs: X_train, a [3, 80, 320, 3] tensor
    """

    X_train = np.empty([0, 80, 320, 3])  # Create a new placeholder tensor

    for i in range(3):

        imgname = line[i].strip()  # Use strip to fix whitespace issue

        X_single_sample = cv2.imread(imgname)  # open image with CV2

        # IMPORTANT. CV2 will read teh image in BGR colour space

        # Uncomment to test x shape
        # print("X original  shape", X_single_sample.shape)

        # Perform chop, 40% off the top, 10% off the bottom.
        top = int(.4 * X_single_sample.shape[0])
        bottom = int(.1 * X_single_sample.shape[0])
        X_single_sample = X_single_sample[top:-bottom, :]
        # print("X new shape", X_single_sample.shape)

        # save images for visualization if required
        # scipy.misc.imsave(
        # 'figs/orginal' + str(np.random.randint(999)) + '.jpg', X_single_sample)

        X_train = np.append(X_train, [X_single_sample], axis=0)

    return X_train


def adjust_colours(X_train):
    """
    Purpose: Change colour space to RGB
    Inputs: X_train, a [3, 80, 320, 3] tensor
    Outputs: X_train, a [3, 80, 320, 3] tensor
    """

    for feature in X_train:

        feature = feature.astype(np.uint8)  # fix error 215
        feature = cv2.cvtColor(feature, cv2.COLOR_BGR2RGB)
        # scipy.misc.imsave(
        #'figs2/adjust-colours' + str(np.random.randint(10000)) + '.jpg', feature)

    return X_train


def flip_images(X_train, y_train):
    """
    Purpose: Flip images left to right and invert angles
    This is to help the model generalize better.

    Inputs: X_train, a [3, 80, 320, 3] tensor and y_train, a [3, 1] matrix
    Outputs: X_train, a [3, 80, 320, 3] tensor and y_train, a [3, 1] matrix
    """

    for feature in X_train:
        feature = np.fliplr(feature)

    for angle in y_train:
        angle = -angle

    return X_train, y_train


def generate_features(X_train, y_train):
    """
    Purpose: Generate new features
    Inputs: X_train, a [3, 80, 320, 3] tensor and y_train, a [3, 1] matrix
    Outputs: X_train, a [ZZZ, 80, 320, 3] tensor and y_train, a [ZZZ, 1] matrix
    Where ZZZ = (FEATURE_GENERATION_MULTIPLE * 3) + 3
    """

    new_features = []  # Create placeholder arrays
    new_labels = []

    for i in range(FEATURE_GENERATION_MULTIPLE):

        for feature in X_train:
            # Settings for transofmr images function
            feature = transform_image(feature, 10, 1, 1)

            # credit https://github.com/navoshta/behavioral-cloning
            # switched to used custom random function
            # Adding shadows
            h, w = feature.shape[0], feature.shape[1]
            [x1, x2] = custom_random.choice(w, 2, replace=False)
            k = h / (x2 - x1)
            b = - k * x1
            for j in range(h):
                c = int((j - b) / k)
                feature[j, :c, :] = (feature[j, :c, :] * .5).astype(np.int32)

            # scipy.misc.imsave('figs2/gen-features' + str(np.random.randint(10000)) + '.jpg', feature)
            new_features.append(feature)

        for label in y_train:
            new_labels.append(label)
            track_angles.append(label)

    X_train = np.append(X_train, new_features, axis=0)
    y_train = np.append(y_train, new_labels, axis=0)

    return X_train, y_train


def process_line(data):
    """
    Purpose: Main entry point for processing intial data
    Inputs: A CSV file of data.
    Outputs: X_train, a [3, 80, 320, 3] tensor and y_train, a [3, 1] matrix
    """

    # Please see functions above for further definition
    balanced_line = balance_data(data)
    X_train = chop_images(balanced_line)
    X_train = adjust_colours(X_train)
    y_train = recovery(balanced_line)
    if custom_random.random_sample(1) <= .6:
        flip_images(X_train, y_train)

    # Check_text = "After image generation, shapes: "
    # check_Shape(Check_text, X_train, y_train)

    return X_train, y_train


def generate_arrays_from_file(path):
    """
    Purpose: Yield tensor batches to fit_generator function
    Inputs: A file path
    Outputs: X_train, a [AHH, 80, 320, 3] tensor and y_train, a [AHH, 1] matrix
    Where AHH = ((FEATURE_GENERATION_MULTIPLE * 3) + 3) * BATCH_SIZE
    """
    batch_tracker = 0
    while 1:
        for batch in range(BATCH_SIZE):
            # load the labels and file paths to images
            data = read_csv(path).values

            X_train, y_train = process_line(data)  # Process existing data
            X_train, y_train = generate_features(X_train, y_train)

            X_train = np.append(X_train, X_train, axis=0)
            y_train = np.append(y_train, y_train, axis=0)

        # print(batch_tracker)
        batch_tracker = batch_tracker + 1

        Check_text = "Batch shape: "
        check_Shape(Check_text, X_train, y_train)

        yield X_train, y_train


"""
Model architecture

This model generally follows the Nvidia architecture as described here:
https://arxiv.org/abs/1604.07316

Using glorot_uniform based on this research:
https://github.com/fchollet/keras/issues/52

Decided to use expontential linear units for activation based on
https://github.com/commaai/research/blob/master/train_steering_model.py
and further reading of
https://arxiv.org/abs/1511.07289

Dropout is used to help generalize the network.
"""

# Use this control to easil test different intializations.
init_type = "glorot_uniform"
border_mode_type = "valid"

row, col, ch = 80, 320, 3

model = Sequential()

# Normalization
# Can use a negative range thanks to ELU

model.add(Lambda(lambda X_train: X_train / 127.5 - 1,
                 input_shape=(row, col, ch),
                 output_shape=(row, col, ch)))


model.add(Convolution2D(24, 5, 5,
                        subsample=(2, 2),
                        border_mode=border_mode_type,
                        init=init_type))
model.add(ELU())

model.add(Convolution2D(36, 5, 5,
                        subsample=(2, 2),
                        border_mode=border_mode_type,
                        init=init_type))
model.add(ELU())

model.add(Convolution2D(48, 5, 5,
                        subsample=(2, 2),
                        border_mode=border_mode_type,
                        init=init_type))
model.add(ELU())

model.add(Convolution2D(64, 3, 3,
                        subsample=(2, 2),
                        border_mode=border_mode_type,
                        init=init_type))
model.add(ELU())

model.add(Convolution2D(64, 3, 3,
                        subsample=(2, 2),
                        border_mode=border_mode_type,
                        init=init_type))
model.add(ELU())

model.add(Flatten())

model.add(Dropout(.2))
model.add(Dense(1164, init=init_type))
model.add(ELU())

model.add(Dropout(.3))
model.add(Dense(100, init=init_type))
model.add(ELU())

model.add(Dense(50, init=init_type))
model.add(ELU())

model.add(Dense(10, init=init_type))
model.add(ELU())

model.add(Dense(1))

# Using the ADAM optimizer with a custom starting learning rate
optimizer_settings = Adam(lr=LEARNING_RATE)

# As this is a continous regression problem we are using mean squared error
model.compile(optimizer=optimizer_settings, loss='mse')

# Early stopping to help test longer epochs
early_stopping = EarlyStopping(monitor='val_loss', patience=2)

# saves the model weights after each epoch if the validation loss decreased
# https://keras.io/callbacks/#create-a-callback
checkpointer = ModelCheckpoint(
    filepath="temp-model/weights.hdf5", verbose=1, save_best_only=True)


if __name__ == "__main__":

    print("Training")

    """
    Train architecture

    Using fit_generator allows the CPU to process images "on demand"
    while the GPU is processing the network.

    We are using a validation set to help understand the networks
    ability to generalize.
    """

    history = model.fit_generator(
        generate_arrays_from_file('driving_log.csv'),
        samples_per_epoch=SAMPLES_EPOCH, nb_epoch=EPOCHS, verbose=2,
        callbacks=[early_stopping, checkpointer],
        validation_data=generate_arrays_from_file('validation_log2.csv'),
        nb_val_samples=VALIDATION_SAMPLES)

    """
    Save model
    """
    model.save('my_model.h5')
    print("Model saved.")

    # This presents a summary of stats from Keras
    model.summary()
    print("Complete.")

    plt_number = custom_random.randint(0, 10000)

    """
    Graph model training and validation loss performance. 
    """
    plt.plot(history.history['loss'])
    plt.plot(history.history['val_loss'])
    plt.title('model loss')
    plt.ylabel('loss')
    plt.xlabel('epoch')
    plt.legend(['train', 'test'], loc='upper left')
    fig2 = plt.gcf()
    plt.show()
    fig2.savefig('testing-figures/loss-history' + str(plt_number) + '.png')

    """
    Graph steering angles used by the model 
    """
    track_angles = np.asarray(track_angles)
    print("Length of track angles: ", len(track_angles))

    plt.hist(track_angles, bins='auto')
    plt.title("Gaussian Histogram")
    plt.xlabel("Value")
    plt.ylabel("Frequency")

    fig = plt.gcf()
    plt.show()
    fig.savefig('testing-figures/angle-histogram' + str(plt_number) + '.png')

    print(min(track_angles))
    print(max(track_angles))

    non_zero_count = (np.count_nonzero(track_angles))
    zeros_counter = len(track_angles) - non_zero_count

    np_unique_angles, np_unique_counts = np.unique(
        track_angles, return_counts=True)

    # Provide stats
    print("Non zero angles:", non_zero_count)
    print("Number of zeros: ", zeros_counter)
    print("Percent zero angle", zeros_counter / len(track_angles))
    print("Number of original zeros (Before Recovery +/-): ", zeros_counter * 3)
    print("Unique angles, non recovery:", len(np.unique(track_angles)) / 3)
    print("Non zero, non recovery angles:", len(
        track_angles) - zeros_counter * 3)
    print(np_unique_angles, np_unique_counts)

    plt.hist(np.unique(track_angles), bins='auto')
    plt.title("Gaussian Histogram")
    plt.xlabel("Value")
    plt.ylabel("Frequency")
    fig = plt.gcf()
    plt.show()
    fig.savefig('testing-figures/unique-angles-histogram' +
                str(plt_number) + '.png')
