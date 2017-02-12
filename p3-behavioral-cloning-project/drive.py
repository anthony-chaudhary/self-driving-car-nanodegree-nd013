import argparse
import base64
import json

import numpy as np
import socketio
import eventlet
import eventlet.wsgi
import time
from PIL import Image
from PIL import ImageOps
from flask import Flask, render_template
from io import BytesIO

from keras.models import load_model
from keras.models import model_from_json
from keras.preprocessing.image import ImageDataGenerator, array_to_img, img_to_array

import matplotlib.pyplot as plt
import time

# Fix error with Keras and TensorFlow
import tensorflow as tf
tf.python.control_flow_ops = tf


sio = socketio.Server()
app = Flask(__name__)
model = None
prev_image_array = None

steering_angles = []


@sio.on('telemetry')
def telemetry(sid, data):
    # The current steering angle of the car
    steering_angle = data["steering_angle"]
    # The current throttle of the car
    throttle = data["throttle"]
    # The current speed of the car
    speed = data["speed"]
    # The current image from the center camera of the car
    imgString = data["image"]
    image = Image.open(BytesIO(base64.b64decode(imgString)))
    image_array = np.asarray(image)

    """ Pre Processing """
    # new_size_col, new_size_row  = 1,1

    # Normalize
    # image_array = image_array / 255.

    """ End 
    """
    top = int(.40 * image_array.shape[0])
    bottom = int(.1 * image_array.shape[0])
    image_array = image_array[top:-bottom, :]
    #print("shape", image_array.shape)

    transformed_image_array = image_array[None, :, :, :]
    # This model currently assumes that the features of the model are just the
    # images. Feel free to change this.
    steering_angle = float(model.predict(
        transformed_image_array, batch_size=1))
    # The driving model currently just outputs a constant throttle. Feel free
    # to edit this.
    # Added control statement based on steering angle.
    throttle = 0.2

    if steering_angle > .1:
        throttle = .15

    steering_angles.append(steering_angle)

    print(steering_angle, throttle)
    send_control(steering_angle, throttle)


@sio.on('connect')
def connect(sid, environ):
    print("connect ", sid)
    send_control(0, 0)


def send_control(steering_angle, throttle):
    sio.emit("steer", data={
        'steering_angle': steering_angle.__str__(),
        'throttle': throttle.__str__()
    }, skip_sid=True)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Remote Driving')
    parser.add_argument('model', type=str,
                        help='Path to model definition json. Model weights should be on the same path.')
    args = parser.parse_args()
    with open(args.model, 'r') as jfile:
        # NOTE: if you saved the file by calling json.dump(model.to_json(), ...)
        # then you will have to call:
        #
        #model = model_from_json(json.loads(jfile.read()))\
        #
        # instead.
        #model = model_from_json(jfile.read())
        model = load_model('my_model.h5')

    model.compile("adam", "mse")
    weights_file = args.model.replace('json', 'h5')
    model.load_weights(weights_file)

    # wrap Flask application with engineio's middleware
    app = socketio.Middleware(sio, app)

    # deploy as an eventlet WSGI server
    eventlet.wsgi.server(eventlet.listen(('', 4567)), app)

    wait_time = 30
    time.sleep(wait_time)
    # Plot
    Plt_number = np.random.randint(0, 1000)
    plt.plot(steering_angles)
    plt.title('Angles')
    plt.ylabel('')
    plt.xlabel('')
    plt.show()
    plt.savefig('angles-drive-history' + str(Plt_number) + '.png')
