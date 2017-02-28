import cv2


def perspectiveTransform(img, src, dst):
    """
    Purpose: Transform image to bird's eye view.
    Inputs: Img array, source points, destintation points
    Outputs: warpedImage array
    """

    img_size = (img.shape[1], img.shape[0])

    # Use cv2.getPerspectiveTransform() to get transformMatrix (M)
    transformMatrix = cv2.getPerspectiveTransform(src, dst)

    # Use cv2.warpPerspective() to warp your image to a top-down view
    warpedImage = cv2.warpPerspective(
        img, transformMatrix, img_size, flags=cv2.INTER_LINEAR)

    return warpedImage
