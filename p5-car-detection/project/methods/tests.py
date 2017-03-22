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

            window_img, heatmap, all_detections_image = x.process_image(
                image, testing_flag=True)

            result_color_fix = cv2.cvtColor(window_img, cv2.COLOR_BGR2RGB)

            all_detections_image = cv2.cvtColor(
                all_detections_image, cv2.COLOR_BGR2RGB)

            cv2.imwrite("../test_images/processed_images/%(filename)s_processed.jpg" %
                        {"filename": img.replace(".jpg", "")}, result_color_fix)

            cv2.imwrite("../test_images/processed_images/%(filename)s_heatmap.jpg" %
                        {"filename": img.replace(".jpg", "")}, heatmap * 255)

            cv2.imwrite("../test_images/processed_images/%(filename)s_all_detections.jpg" %
                        {"filename": img.replace(".jpg", "")}, all_detections_image)


testImages()
