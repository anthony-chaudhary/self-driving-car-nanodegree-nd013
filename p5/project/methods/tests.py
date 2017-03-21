import matplotlib.image as mpimg
import cv2
import os
import detectClass


def testImages():

    test_images = os.listdir("../test_images/")

    try:
        processed_images = os.listdir("../test_images/processed_images/")
    except FileNotFoundError:
        print("File not found")

    x = detectClass.Detect()

    for img in test_images:
        if '.jpg' in img:
            image = mpimg.imread("../test_images/%(filename)s" %
                                 {"filename": img})

            window_img = x.process_image(image)

            result_color_fix = cv2.cvtColor(window_img, cv2.COLOR_BGR2RGB)

            cv2.imwrite("../test_images/processed_images/%(filename)s_processed.jpg" %
                        {"filename": img.replace(".jpg", "")}, result_color_fix)

            # cv2.imwrite("../test_images/processed_images/%(filename)s_gradx.jpg" %
            # {"filename": img.replace(".jpg", "")}, gradx * 255)


testImages()
