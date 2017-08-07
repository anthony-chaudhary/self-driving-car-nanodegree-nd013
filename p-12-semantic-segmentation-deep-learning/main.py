import os.path
os.environ['TF_CPP_MIN_LOG_LEVEL']='2'
import tensorflow as tf
import helper
import warnings
from distutils.version import LooseVersion
import project_tests as tests


# Check TensorFlow Version
assert LooseVersion(tf.__version__) >= LooseVersion('1.0'), 'Please use TensorFlow version 1.0 or newer.  You are using {}'.format(tf.__version__)
print('TensorFlow Version: {}'.format(tf.__version__))

# Check for a GPU
if not tf.test.gpu_device_name():
    warnings.warn('No GPU found. Please use a GPU to train your neural network.')
else:
    print('Default GPU Device: {}'.format(tf.test.gpu_device_name()))


def load_vgg(sess, vgg_path):
    """
    Load Pretrained VGG Model into TensorFlow.
    :param sess: TensorFlow Session
    :param vgg_path: Path to vgg folder, containing "variables/" and "saved_model.pb"
    :return: Tuple of Tensors from VGG model (image_input, keep_prob, layer3_out, layer4_out, layer7_out)
    """
    
    a = 'vgg16'
    b = 'image_input:0'
    c = 'keep_prob:0'
    d = 'layer3_out:0'
    e = 'layer4_out:0'
    f = 'layer7_out:0'

    tf.saved_model.loader.load(sess, [a], vgg_path)

    list = [b, c, d, e, f]
    placeholders = []

    for i in list:
        placeholders.append(tf.get_default_graph().get_tensor_by_name(i))
    
    return placeholders



def layers(vgg_layer3_out, vgg_layer4_out, vgg_layer7_out, num_classes):
    """
    Create the layers for a fully convolutional network. 
    Build skip-layers using the vgg layers.
    :param vgg_layer7_out: TF Tensor for VGG Layer 3 output
    :param vgg_layer4_out: TF Tensor for VGG Layer 4 output
    :param vgg_layer3_out: TF Tensor for VGG Layer 7 output
    :param num_classes: Number of classes to classify
    :return: The Tensor for the last layer of output
    """
    
        
    l_7 = tf.layers.conv2d(vgg_layer7_out, num_classes, 1, padding='SAME',
                          kernel_initializer=tf.truncated_normal_initializer(stddev = 1e-3))

    l_4 = tf.layers.conv2d(vgg_layer4_out, num_classes, 1, padding='SAME', 
                           kernel_initializer=tf.truncated_normal_initializer(stddev = 1e-3))

    l_3 = tf.layers.conv2d(vgg_layer3_out, num_classes, 1, padding='SAME',
                           kernel_initializer=tf.truncated_normal_initializer(stddev = 1e-3))
                     
    up_7 = tf.layers.conv2d_transpose(l_7, num_classes, 4, strides=(2,2), padding='SAME',
                                      kernel_initializer=tf.truncated_normal_initializer(stddev = 1e-3))

    skip_0 = tf.add(l_4, up_7)      

    up_4_7 = tf.layers.conv2d_transpose(skip_0, num_classes, 4, strides=(2,2), padding='SAME',
                                        kernel_initializer=tf.truncated_normal_initializer(stddev = 1e-3))

    skip_1 = tf.add(l_3, up_4_7)

    up_3_4_7 = tf.layers.conv2d_transpose(skip_1, num_classes, 16, strides=(8, 8), padding='SAME',
                                          kernel_initializer=tf.truncated_normal_initializer(stddev = 1e-3))

       
    return up_3_4_7



def optimize(nn_last_layer, labels, learning_rate, num_classes):
    """
    Build the TensorFLow loss and optimizer operations.
    :param nn_last_layer: TF Tensor of the last layer in the neural network
    :param correct_label: TF Placeholder for the correct label image
    :param learning_rate: TF Placeholder for the learning rate
    :param num_classes: Number of classes to classify
    :return: Tuple of (logits, train_op, cross_entropy_loss)
    """
    
    logits = tf.reshape(nn_last_layer, (-1, num_classes))  # 4d to 2d
    labels_re = tf.reshape(labels, (-1, num_classes))
    soft = tf.nn.softmax_cross_entropy_with_logits(logits=logits, labels=labels_re)
    loss = tf.reduce_mean(soft)

    tf.summary.histogram("softmax", soft)
    tf.summary.scalar("loss", loss)

    trainer = tf.train.AdamOptimizer(learning_rate).minimize(loss)

    return logits, trainer, loss



def train_nn(sess, epochs, batch_size, get_batches_fn, train_op, cross_entropy_loss, input_image,
             correct_label, keep_prob, learning_rate):
    """
    Train neural network and print out the loss during training.
    :param sess: TF Session
    :param epochs: Number of epochs
    :param batch_size: Batch size
    :param get_batches_fn: Function to get batches of training data.  Call using get_batches_fn(batch_size)
    :param train_op: TF Operation to train the neural network
    :param cross_entropy_loss: TF Tensor for the amount of loss
    :param input_image: TF Placeholder for input images
    :param correct_label: TF Placeholder for label images
    :param keep_prob: TF Placeholder for dropout keep probability
    :param learning_rate: TF Placeholder for learning rate
    """
    index = 0

    train_writer = tf.summary.FileWriter('./logs', sess.graph)

    for i in range(epochs):
        for j, j_l in get_batches_fn(batch_size):
            

            merge = tf.summary.merge_all()

            summary, _, loss = sess.run([merge, train_op, cross_entropy_loss], 
                feed_dict = {input_image: j, correct_label: j_l,
                                                    keep_prob: .70, learning_rate: 1e-4})
            
            train_writer.add_summary(summary, index)

            if index % 10 == 0:
                print("Epoch", i)
                print("Loss {:.5f}...".format(loss))

            index += 1          


def run():
    num_classes = 2
    image_shape = (160, 576)
    data_dir = './data'
    runs_dir = './runs'

    tests.test_load_vgg(load_vgg, tf)
    tests.test_layers(layers)
    print("layers test passed")
    tests.test_optimize(optimize)
    print("optimize test passed")
    tests.test_train_nn(train_nn)
    print("train test passed")

    #tests.test_for_kitti_dataset(data_dir)
    #helper.maybe_download_pretrained_vgg(data_dir)

    # OPTIONAL: Train and Inference on the cityscapes dataset instead of the Kitti dataset.
    # You'll need a GPU with at least 10 teraFLOPS to train on.
    #  https://www.cityscapes-dataset.com/

    with tf.Session() as sess:


               
        vgg_path = os.path.join(data_dir, 'vgg')
        
        get_batches_fn = helper.gen_batch_function(os.path.join(data_dir, 'data_road/training'), image_shape)

        # OPTIONAL: Augment Images for better results
        #  https://datascience.stackexchange.com/questions/5224/how-to-prepare-augment-images-for-neural-network

        learning_rate = tf.placeholder(tf.float32)
        correct_labels = tf.placeholder(tf.float32, shape=(None, None, None, num_classes))
        
        image_input, keep_prob, vgg_layer3_out, vgg_layer4_out, vgg_layer7_out = load_vgg(sess, vgg_path)

        up_3_4_7 = layers(vgg_layer3_out, vgg_layer4_out, vgg_layer7_out, num_classes)

        logits, trainer, loss = optimize(up_3_4_7, correct_labels, learning_rate, num_classes)

    
        sess.run(tf.global_variables_initializer())

    
        epochs = 15
        batch_size = 8

        train_nn(sess, epochs, batch_size, get_batches_fn, trainer, loss, image_input,
             correct_labels, keep_prob, learning_rate)

        
        helper.save_inference_samples(runs_dir, data_dir, sess, image_shape, logits, keep_prob, image_input)

        # OPTIONAL: Apply the trained model to a video


if __name__ == '__main__':
    run()

