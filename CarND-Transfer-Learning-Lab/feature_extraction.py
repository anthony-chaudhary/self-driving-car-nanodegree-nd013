import pickle
import tensorflow as tf
import numpy as np
from keras.layers import Input, Flatten, Dense
from keras.models import Model

flags = tf.app.flags
FLAGS = flags.FLAGS

# command line flags
flags.DEFINE_string('training_file', '',
                    "Bottleneck features training file (.p)")
flags.DEFINE_string('validation_file', '',
                    "Bottleneck features validation file (.p)")
flags.DEFINE_integer('epochs', 50, "The number of epochs.")
flags.DEFINE_integer('batch_size', 256, "Da batch size.")


def load_bottleneck_data(training_file, validation_file):
    """
    Utility function to load bottleneck features.

    Arguments:
        training_file - String
        validation_file - String
    """
    print("Training file", training_file)
    print("Validation file", validation_file)

    with open("resnet_traffic_100_bottleneck_features_train.p", 'rb') as f:
        train_data = pickle.load(f)
    with open("resnet_traffic_bottleneck_features_validation.p", 'rb') as f:
        validation_data = pickle.load(f)

    X_train = train_data['features']
    y_train = train_data['labels']
    X_val = validation_data['features']
    y_val = validation_data['labels']

    return X_train, y_train, X_val, y_val


def main(_):
    # load bottleneck data
    X_train, y_train, X_val, y_val = load_bottleneck_data(
        FLAGS.training_file, FLAGS.validation_file)

    print(X_train.shape, y_train.shape)
    print(X_val.shape, y_val.shape)

    numberOfClasses = len(np.unique(y_train))

    inputShape = X_train.shape[1:]
    zzzzzInput = Input(shape=inputShape)

    x = Flatten()(zzzzzInput)
    x = Dense(numberOfClasses, activation='softmax')(x)

    model = Model(zzzzzInput, x)
    model.compile(optimizer='adam',
                  loss='sparse_categorical_crossentropy', metrics=['accuracy'])

    model.fit(X_train, y_train, nb_epoch=FLAGS.epochs,
              batch_size=FLAGS.batch_size, validation_data=(X_val, y_val), shuffle=True)

# parses flags and calls the `main` function above
if __name__ == '__main__':
    tf.app.run()
