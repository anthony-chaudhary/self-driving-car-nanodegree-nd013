from moviepy.editor import VideoFileClip
import lineClass

"""
Purpose: Process videos using the methods\processImage.py
pipeline.
Inputs: Video clip
Outputs: Video clip with detected lane recorded and drawn.
"""

# Create instance of class
x = lineClass.Line()

clip1 = VideoFileClip("../project_video.mp4")
clip = clip1.fl_image(x.process_image)
clip.write_videofile(
    '../testing_videos/attempt_march13_4_project_video.mp4', audio=False)
