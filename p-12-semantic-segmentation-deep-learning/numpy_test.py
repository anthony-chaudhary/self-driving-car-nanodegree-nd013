

gt_image = [[1, 2, 4], [1, 2, 3]]

import numpy as np
import matplotlib.pyplot as plt

background_color_road = np.array([1, 2, 4])
background_color_car = np.array([1, 2, 0])
background_color_road_ = np.all(gt_image == background_color_road, axis=1)
background_color_car_ = np.all(gt_image == background_color_car, axis=1)

background_all = np.all((background_color_road_, background_color_car_), axis=0)
background_all_invert = np.all((np.invert(background_color_road_), np.invert(background_color_car_)), axis=0)

background_any = np.any((background_color_road_, background_color_car_), axis=0)
background_any_invert = np.any((np.invert(background_color_road_), np.invert(background_color_car_)), axis=0)

print(background_color_road_)
print(background_color_car_)
print(background_all)
print(background_all_invert)
print(background_any)
print(background_any_invert)
#plt.show()