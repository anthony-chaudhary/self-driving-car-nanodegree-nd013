import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import glob
import cv2

# Read in a thresholded image
warped = mpimg.imread('../../warped_example.jpg')
# window settings
window_width = 50
window_height = 80  # Break image into 9 vertical layers since image height is 720
margin = 100  # How much to slide left and right for searching


def window_mask(width, height, img_ref, center, level):
    output = np.zeros_like(img_ref)
    output[int(img_ref.shape[0] - (level + 1) * height):int(img_ref.shape[0] - level * height),
           max(0, int(center - width / 2)):min(int(center + width / 2), img_ref.shape[1])] = 1
    return output


def find_window_centroids(image, window_width, window_height, margin):

    # Store the (left,right) window centroid positions per level
    window_centroids = []
    l_center_points = []
    r_center_points = []
    # Create our window template that we will use for convolutions
    window = np.ones(window_width)

    # First find the two starting positions for the left and right lane by using np.sum to get the vertical image slice
    # and then np.convolve the vertical image slice with the window template

    # Sum quarter bottom of image to get slice, could use a different ratio
    l_sum = np.sum(warped[int(3 * warped.shape[0] / 4):,
                          :int(warped.shape[1] / 2)], axis=0)
    l_center = np.argmax(np.convolve(window, l_sum)) - window_width / 2
    r_sum = np.sum(warped[int(3 * warped.shape[0] / 4):,
                          int(warped.shape[1] / 2):], axis=0)
    r_center = np.argmax(np.convolve(window, r_sum)) - \
        window_width / 2 + int(warped.shape[1] / 2)

    # Add what we found for the first layer
    window_centroids.append((l_center, r_center))

    # Go through each layer looking for max pixel locations
    for level in range(1, (int)(warped.shape[0] / window_height)):
        # convolve the window into the vertical slice of the image
        image_layer = np.sum(warped[int(warped.shape[
                             0] - (level + 1) * window_height):int(warped.shape[0] - level * window_height), :], axis=0)
        conv_signal = np.convolve(window, image_layer)
        # Find the best left centroid by using past left center as a reference
        # Use window_width/2 as offset because convolution signal reference is
        # at right side of window, not center of window
        offset = window_width / 2
        l_min_index = int(max(l_center + offset - margin, 0))
        l_max_index = int(min(l_center + offset + margin, warped.shape[1]))
        l_center = np.argmax(
            conv_signal[l_min_index:l_max_index]) + l_min_index - offset
        # Find the best right centroid by using past right center as a
        # reference
        r_min_index = int(max(r_center + offset - margin, 0))
        r_max_index = int(min(r_center + offset + margin, warped.shape[1]))
        r_center = np.argmax(
            conv_signal[r_min_index:r_max_index]) + r_min_index - offset
        # Add what we found for that layer
        window_centroids.append((l_center, r_center))
        l_center_points.append(l_center)
        r_center_points.append(r_center)

    # print(l_center)
    # print(r_center)
    # print("break")
    print(window_centroids)

    return window_centroids, l_center_points, r_center_points

window_centroids, l_center_points, r_center_points = find_window_centroids(
    warped, window_width, window_height, margin)

# If we found any window centers
if len(window_centroids) > 0:

    # Points used to draw all the left and right windows
    l_points = np.zeros_like(warped)
    r_points = np.zeros_like(warped)

    # Go through each level and draw the windows
    for level in range(0, len(window_centroids)):
        # Window_mask is a function to draw window areas
        l_mask = window_mask(window_width, window_height,
                             warped, window_centroids[level][0], level)
        r_mask = window_mask(window_width, window_height,
                             warped, window_centroids[level][1], level)
        # Add graphic points from window mask here to total pixels found
        l_points[(l_points == 255) | ((l_mask == 1))] = 255
        r_points[(r_points == 255) | ((r_mask == 1))] = 255

    # Draw the results
    # add both left and right window pixels together
    template = np.array(r_points + l_points, np.uint8)
    zero_channel = np.zeros_like(template)  # create a zero color channle
    # make window pixels green
    template = np.array(
        cv2.merge((zero_channel, template, zero_channel)), np.uint8)
    # making the original road pixels 3 color channels
    warpage = np.array(cv2.merge((warped, warped, warped)), np.uint8)
    # overlay the orignal road image with window results
    output = cv2.addWeighted(warpage, 1, template, 0.5, 0.0)

# If no window centers found, just display orginal road image
else:
    output = np.array(cv2.merge((warped, warped, warped)), np.uint8)

# Display the final results
plt.imshow(output)
plt.title('window fitting results')
# plt.show()


# New function to split up


import numpy as np
import matplotlib.pyplot as plt

ploty = np.linspace(0, 720 - 1, 720)  # to cover same y-range as image
# print(leftx.shape)

# leftx = leftx[::-1]  # Reverse to match top-to-bottom in y
# rightx = rightx[::-1]  # Reverse to match top-to-bottom in y

x, y = zip(*window_centroids)

print(l_center_points)
print(r_center_points)

# a wild hardcoded list appears! It's super fun!
super_fun_y_points = [80, 160, 240, 320, 400, 480, 560, 640]
super_fun_y_points.reverse()

print(len(window_centroids))
# Fit a second order polynomial to pixel positions in each lane line
left_fit = np.polyfit(super_fun_y_points, l_center_points, 2)
left_fitx = left_fit[0] * ploty**2 + left_fit[1] * ploty + left_fit[2]
right_fit = np.polyfit(super_fun_y_points, r_center_points, 2)
right_fitx = right_fit[0] * ploty**2 + right_fit[1] * ploty + right_fit[2]

# Plot up the data
plt.xlim(0, 1280)
plt.ylim(0, 720)
plt.plot(left_fitx, ploty, color='red', linewidth=3)
plt.plot(right_fitx, ploty, color='red', linewidth=3)
plt.gca().invert_yaxis()  # to visualize as we do the images

plt.show()

# Define y-value where we want radius of curvature
# I'll choose the maximum y-value, corresponding to the bottom of the image
y_eval = np.max(ploty)
left_curverad = (
    (1 + (2 * left_fit[0] * y_eval + left_fit[1])**2)**1.5) / np.absolute(2 * left_fit[0])
right_curverad = (
    (1 + (2 * right_fit[0] * y_eval + right_fit[1])**2)**1.5) / np.absolute(2 * right_fit[0])
print(left_curverad, right_curverad)
# Example values: 1926.74 1908.48


# Define conversions in x and y from pixels space to meters
ym_per_pix = 30 / 720  # meters per pixel in y dimension
xm_per_pix = 3.7 / 700  # meters per pixel in x dimension

# Fit new polynomials to x,y in world space
left_fit_cr = np.polyfit(ploty * ym_per_pix, leftx * xm_per_pix, 2)
right_fit_cr = np.polyfit(ploty * ym_per_pix, rightx * xm_per_pix, 2)
# Calculate the new radii of curvature
left_curverad = ((1 + (2 * left_fit_cr[0] * y_eval * ym_per_pix + left_fit_cr[
                 1])**2)**1.5) / np.absolute(2 * left_fit_cr[0])
right_curverad = ((1 + (2 * right_fit_cr[0] * y_eval * ym_per_pix + right_fit_cr[
                  1])**2)**1.5) / np.absolute(2 * right_fit_cr[0])
# Now our radius of curvature is in meters
print(left_curverad, 'm', right_curverad, 'm')
# Example values: 632.1 m    626.2 m
