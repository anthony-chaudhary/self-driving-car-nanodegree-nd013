
"""
    Collects test images, creates folder to put them in, runs pipeline, and saves images.
"""

import os
import shutil

test_images = os.listdir("test_images/")

try:
    processed_images = os.listdir("test_images/processed_images/")
except FileNotFoundError:
    print("File not found")

if processed_images:
    shutil.rmtree("test_images/processed_images/", ignore_errors=True)
    # Create New Folder for Processing
    create_success = os.mkdir("test_images/processed_images/")

for img in test_images:
    if '.jpg' in img:
        image = mpimg.imread("test_images/%(filename)s" % {"filename": img})

        hough = hough_lines(image)
        processed_image = weighted_img(hough, image)
        color_fix = cv2.cvtColor(processed_image, cv2.COLOR_BGR2RGB)

        cv2.imwrite("test_images/processed_images/%(filename)s_processed.jpg" %
                    {"filename": img.replace(".jpg", "")}, color_fix)
