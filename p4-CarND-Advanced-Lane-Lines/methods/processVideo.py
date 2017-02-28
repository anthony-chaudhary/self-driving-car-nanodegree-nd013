from moviepy.editor import VideoFileClip
from processImage import process_image

"""
Purpose: Process videos using the methods\processImage.py
pipeline.
Inputs: Video clip
Outputs: Video clip with detected lane recorded and drawn.
"""

clip1 = VideoFileClip("../project_video.mp4")
clip = clip1.fl_image(process_image)
clip.write_videofile(
    '../testing_videos/attempt7_project_video.mp4', audio=False)
