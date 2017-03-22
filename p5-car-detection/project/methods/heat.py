heat = np.zeros_like(image[:,:,0]).astype(np.float)
heat = add_heat(heat,box_list)  # Add heat to each box in box list
heat = apply_threshold(heat,1)  # Apply threshold to help remove false positives 
heatmap = np.clip(heat, 0, 255)  # Visualize the heatmap when displaying   

# Find final boxes from heatmap using label function
labels = label(heatmap)
draw_img = draw_labeled_bboxes(np.copy(image), labels)

fig = plt.figure()
plt.subplot(121)
plt.imshow(draw_img)
plt.title('Car Positions')
plt.subplot(122)
plt.imshow(heatmap, cmap='hot')
plt.title('Heat Map')
fig.tight_layout()
