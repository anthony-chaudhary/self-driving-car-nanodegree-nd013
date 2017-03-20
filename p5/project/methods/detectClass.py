import numpy as np
from processImage import process_image

# Define a class to receive the characteristics of each line detection
class Detect():
    def __init__(self):
        # was the line detected in the last iteration?
        self.detected = False

        self.distance_between_lines = []

        self.recent_fit_x_left = []
        self.recent_fit_x_right = []


    def process_image(self, image):
        return process_image(self, image)
