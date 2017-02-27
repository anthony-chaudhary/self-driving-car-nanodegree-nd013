from moviepy.editor import VideoFileClip
from processImage import process_image

clip1 = VideoFileClip("../challenge_video.mp4")
clip = clip1.fl_image(process_image)
clip.write_videofile('attempt1_challenge_video.mp4', audio=False)
