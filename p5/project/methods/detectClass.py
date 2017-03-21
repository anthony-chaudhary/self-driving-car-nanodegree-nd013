import numpy as np
from processImage import process_image

# Define a class to receive the characteristics of each line detection


class Detect():
    def __init__(self):

        self.detected = False
        self.labels = []

    def process_image(self, image):
        return process_image(self, image)
