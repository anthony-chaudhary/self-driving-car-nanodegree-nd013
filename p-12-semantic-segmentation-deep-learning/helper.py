import re
import random
import numpy as np
import os.path
import scipy.misc
import shutil
import zipfile
import time
import tensorflow as tf
from glob import glob
from urllib.request import urlretrieve
from tqdm import tqdm
import matplotlib.pyplot as plt

class DLProgress(tqdm):
    last_block = 0

    def hook(self, block_num=1, block_size=1, total_size=None):
        self.total = total_size
        self.update((block_num - self.last_block) * block_size)
        self.last_block = block_num


def maybe_download_pretrained_vgg(data_dir):
    """
    Download and extract pretrained vgg model if it doesn't exist
    :param data_dir: Directory to download the model to
    """
    vgg_filename = 'vgg.zip'
    vgg_path = os.path.join(data_dir, 'vgg')
    vgg_files = [
        os.path.join(vgg_path, 'variables/variables.data-00000-of-00001'),
        os.path.join(vgg_path, 'variables/variables.index'),
        os.path.join(vgg_path, 'saved_model.pb')]

    missing_vgg_files = [vgg_file for vgg_file in vgg_files if not os.path.exists(vgg_file)]
    if missing_vgg_files:
        # Clean vgg dir
        if os.path.exists(vgg_path):
            shutil.rmtree(vgg_path)
        os.makedirs(vgg_path)

        # Download vgg
        print('Downloading pre-trained vgg model...')
        with DLProgress(unit='B', unit_scale=True, miniters=1) as pbar:
            urlretrieve(
                'https://s3-us-west-1.amazonaws.com/udacity-selfdrivingcar/vgg.zip',
                os.path.join(vgg_path, vgg_filename),
                pbar.hook)

        # Extract vgg
        print('Extracting model...')
        zip_ref = zipfile.ZipFile(os.path.join(vgg_path, vgg_filename), 'r')
        zip_ref.extractall(data_dir)
        zip_ref.close()

        # Remove zip file to save space
        os.remove(os.path.join(vgg_path, vgg_filename))


def gen_batch_function(data_folder, image_shape):
    """
    Generate function to create batches of training data
    :param data_folder: Path to folder that contains all the datasets
    :param image_shape: Tuple - Shape of image
    :return:
    """
    def get_batches_fn(batch_size):
        """
        Create batches of training data
        :param batch_size: Batch Size
        :return: Batches of training data
        """

        # for citi scape
        image_paths = glob(os.path.join(data_folder, 'leftImg8bit/train/aachen', '*.png'))
        
        label_paths = glob(os.path.join(data_folder, 'gtFine/train/aachen', '*_color.png'))
        
        print(label_paths[0])
       

        background_color_road = np.array([128, 64, 128])
        background_color_car = np.array([0, 0, 142])

        random.shuffle(image_paths)
        for batch_i in range(0, len(image_paths), batch_size):
            
            images = []
            gt_images = []
            
            for i in range(len(image_paths[batch_i:batch_i+batch_size])):

                image = scipy.misc.imresize(scipy.misc.imread(image_paths[i]), image_shape)
                #print("image size 1", image.shape)
                gt_image = scipy.misc.imresize(scipy.misc.imread(label_paths[i], mode="RGB"), image_shape)
                #print("gt_image size 1", gt_image.shape)

                gt_bg_road = np.all(gt_image == background_color_road, axis=2)
                #plt.imshow(gt_bg_road)
                #plt.show()
                gt_bg_road = gt_bg_road.reshape(*gt_bg_road.shape, 1)
                
                gt_bg_car = np.all(gt_image == background_color_car, axis=2)
                # plt.imshow(gt_bg_car)
                # plt.show()
                gt_bg_car = gt_bg_car.reshape(*gt_bg_car.shape, 1)

                gt_bg = np.all((np.invert(gt_bg_car), np.invert(gt_bg_road)), axis=0)
                #print(gt_bg.shape)
                #plt.imshow(gt_bg)
                #plt.show()

                gt_image = np.concatenate((gt_bg, gt_bg_road, gt_bg_car), axis=2)
                # gt_image_2 = np.concatenate((gt_bg_car, np.invert(gt_bg_car)), axis=2)
                                 
                #plt.imshow(gt_image)
                #plt.show()

                #gt_image = np.concatenate((gt_image_1, gt_image_2), axis=2)
                #print("gt_image size", gt_image.shape)

                images.append(image)
                gt_images.append(gt_image)

            yield np.array(images), np.array(gt_images)
    return get_batches_fn


def gen_test_output(sess, logits, keep_prob, image_pl, data_folder, image_shape):
    """
    Generate test output using the test images
    :param sess: TF session
    :param logits: TF Tensor for the logits
    :param keep_prob: TF Placeholder for the dropout keep robability
    :param image_pl: TF Placeholder for the image placeholder
    :param data_folder: Path to the folder that contains the datasets
    :param image_shape: Tuple - Shape of image
    :return: Output for for each test image
    """
    for image_file in glob(os.path.join(data_folder, '*.png')):
        image = scipy.misc.imresize(scipy.misc.imread(image_file), image_shape)

        im_softmax = sess.run(
            [tf.nn.softmax(logits)],
            {keep_prob: 1.0, image_pl: [image]})
        
        #print(im_softmax[0], "\n")
        #print(im_softmax[0][:, 0], "\n")

        #print(im_softmax[0][:, 1], "\n")
            
        #print(len(im_softmax[0][:, 0]))
        ###print(im_softmax[1]) returns error
        # Step size seems to determine which probabliity to pick
        # ie 1 == second index result...
        # so 0 is first result
        # and 2 is third result?

        im_road_np = np.array(im_softmax[0][:, 1])
        im_car_np = np.array(im_softmax[0][:, 2])
        #print(im_car_np)

        im_softmax = im_road_np.reshape(image_shape[0], image_shape[1])
        im_softmax_car = im_car_np.reshape(image_shape[0], image_shape[1])
        
        segmentation = (im_softmax > 0.5).reshape(image_shape[0], image_shape[1], 1)
        segmentation_car = (im_softmax_car > 0.2).reshape(image_shape[0], image_shape[1], 1)

        mask = np.dot(segmentation, np.array([[0, 255, 0, 127]]))
        mask = scipy.misc.toimage(mask, mode="RGBA")

        mask_car = np.dot(segmentation_car, np.array([[0, 0, 255, 127]]))
        mask_car = scipy.misc.toimage(mask_car, mode="RGBA")

        street_im = scipy.misc.toimage(image)

        street_im.paste(mask, box=None, mask=mask)
        street_im.paste(mask_car, box=None, mask=mask_car)

        yield os.path.basename(image_file), np.array(street_im)


def save_inference_samples(runs_dir, data_dir, sess, image_shape, logits, keep_prob, input_image):
    # Make folder for current run
    output_dir = os.path.join(runs_dir, str(time.time()))
    if os.path.exists(output_dir):
        shutil.rmtree(output_dir)
    os.makedirs(output_dir)

    # Run NN on test images and save them to HD
    print('Training Finished. Saving test images to: {}'.format(output_dir))
    image_outputs = gen_test_output(
        sess, logits, keep_prob, input_image, os.path.join(data_dir, 'leftImg8bit/test/berlin'), image_shape)
    for name, image in image_outputs:
        scipy.misc.imsave(os.path.join(output_dir, name), image)
