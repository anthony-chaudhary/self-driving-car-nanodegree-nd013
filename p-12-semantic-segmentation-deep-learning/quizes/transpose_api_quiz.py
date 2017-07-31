import oldtensorflow as tf
import numpy as np


def upsample(x):
    """
    Apply a two times upsample on x and return the result.
    :x: 4-Rank Tensor
    :return: TF Operation
    """
    # TODO: Use `tf.layers.conv2d_transpose`
    return tf.layers.conv2d_transpose(x, 3, [2, 2], [2, 2] )


x = tf.constant(np.random.randn(1, 4, 4, 3), dtype=tf.float32)
conv = upsample(x)

with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    result = sess.run(conv)

    print('Input Shape: {}'.format(x.get_shape()))
    print('Output Shape: {}'.format(result.shape))
