from moviepy.editor import VideoFileClip
import detectClass

"""
Purpose: Process videos using the methods\processImage.py
pipeline.
Inputs: Video clip
Outputs: Video clip with detected lane recorded and drawn.
"""

# Create instance of class
x = detectClass.Detect()

clip1 = VideoFileClip("../project_video.mp4")
clip = clip1.fl_image(x.process_image)
clip.write_videofile(
    '../test_videos/2_project_video_March_21.mp4', audio=False)
