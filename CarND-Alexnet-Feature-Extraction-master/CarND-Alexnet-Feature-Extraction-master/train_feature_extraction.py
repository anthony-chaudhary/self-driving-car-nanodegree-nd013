import pickle
import time
import tensorflow as tf
from sklearn.utils import shuffle
from sklearn.model_selection import train_test_split
from alexnet import AlexNet

# settings
number_of_classes = 43
epochs = 10
batch_size = 128


# Load traffic signs data.
with open('./train.p', 'rb') as open_data:
    data = pickle.load(open_data)

# Split data into training and validation sets.
X_train, X_validate, y_train, y_validate = train_test_split(
    data['features'], data['labels'], test_size=0.20, random_state=0)


# Define placeholders and resize operation.
# using float 32 for images, 32 pixels by 32 with 3 colour channels
features = tf.placeholder(tf.float32, (None, 32, 32, 3))
# None for no shape
labels = tf.placeholder(tf.int64, None)
# Resize images using built in resize feature to expected 227x227
resized = tf.image.resize_images(features, (227, 227))

# Pass features as first argument to `AlexNet`.
fc7 = AlexNet(resized, feature_extract=True)

# NOTE: `tf.stop_gradient` prevents the gradient from flowing backwards
# past this point, keeping the weights before and up to `fc7` frozen.
# This also makes training faster, less work to do!
fc7 = tf.stop_gradient(fc7)

# Add the final layer for traffic sign classification.
shape = (fc7.get_shape().as_list()[-1], number_of_classes)

# Create some random stuff
fc8W = tf.Variable(tf.truncated_normal(shape, stddev=.01))
# Zeros for bias
fc8b = tf.Variable(tf.zeros(number_of_classes))
# Score using X_train * weights + bias
logits = tf.nn.xw_plus_b(fc7, fc8W, fc8b)
# pass scores and labels to softmax super duper version
cross_entropy = tf.nn.sparse_softmax_cross_entropy_with_logits(logits, labels)

# Define loss, training, accuracy operations.
# define goal of reducing average of score?
loss_operation = tf.reduce_mean(cross_entropy)

# assign AdamOptimizer to var for code readability
operation = tf.train.AdamOptimizer()

# train operation trys to reduce loss operation using variables
# from fully connected layer 8
train_operation = operation.minimize(loss_operation, var_list=[fc8W, fc8b])

# Start
start_operation = tf.global_variables_initializer()

# return largest value of index of score
predictions = tf.arg_max(logits, 1)

accuracy_operation = tf.reduce_mean(
    tf.cast(tf.equal(predictions, labels), tf.float32))


def evaulate(X, y, sess):
    total_accuracy = 0
    total_loss = 0

    for offset in range(0, X.shape[0], batch_size):
        end = offset + batch_size
        X_batch = X[offset:end]
        y_batch = y[offset:end]

        loss, accuracy = sess.run([loss_operation, accuracy_operation],
                                  feed_dict={
                                  features: X_batch, labels: y_batch})

        total_loss += (loss * X_batch.shape[0])
        total_accuracy += (accuracy * X_batch.shape[0])

    return total_loss / X.shape[0], total_accuracy / X.shape[0]


with tf.Session() as sess:
    sess.run(start_operation)

    print("Running")
    for epoch in range(epochs):

        X_train, y_train = shuffle(X_train, y_train)
        t0 = time.time()

        for offset in range(0, X_train.shape[0], batch_size):
            end = offset + batch_size
            sess.run(train_operation,
                     feed_dict={features: X_train[offset:end], labels: y_train[offset:end]})

        validated_loss, validated_accuracy = evaulate(
            X_validate, y_validate, sess)

        print("Epoch", epoch + 1)
        print("Time: %.3f seconds" % (time.time() - t0))
        print("Validation loss=", validated_loss)
        print("Validation accuracy =", validated_accuracy)
        print("-----")
