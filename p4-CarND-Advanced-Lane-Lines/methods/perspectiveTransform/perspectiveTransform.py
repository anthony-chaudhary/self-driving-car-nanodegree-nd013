import cv2


def perspectiveTransform(img, src, dst):
    """
    Purpose: Fix image warping.
    Inputs: Img array, source points, destintation points
    Outputs: transformMatrix(fixed image, warpedImage
    (Perspective changed image?)
    """

    img_size = (img.shape[1], img.shape[0])

    # Use cv2.getPerspectiveTransform() to get transformMatrix (M)
    transformMatrix = cv2.getPerspectiveTransform(src, dst)

    # Use cv2.warpPerspective() to warp your image to a top-down view
    warpedImage = cv2.warpPerspective(
        img, transformMatrix, img_size, flags=cv2.INTER_LINEAR)

    return warpedImage
