#把要分類的圖片放到food資料夾，按z移動到vegetarian資料夾，按x移動到non-vegetarian資料夾，按v退出
import os
import shutil
import cv2
import pygetwindow as gw

max_size = 1000 #設定顯示圖片大小，可以修改
'''
while True:
    try:
        max_size = int(input("請輸入圖片的最大顯示大小："))
        break  # 如果輸入有效，則退出迴圈
    except ValueError:
        print("請輸入有效的數字。")'''

# 資料夾路徑
source_folder = "food"
target_folders = ["vegetarian", "non-vegetarian"]

# 取得資料夾中的所有圖片檔案
image_files = [f for f in os.listdir(source_folder) if f.lower().endswith(('.png', '.jpg', 'webp', '.jpeg', '.gif', '.bmp'))]

# 建立目標資料夾（如果不存在）
for folder in target_folders:
    folder_path = os.path.join(source_folder, folder)
    os.makedirs(folder_path, exist_ok=True)
    
# 顯示每張圖片，按下z或x移動到相應的資料夾
for image_file in image_files:
    image_path = os.path.join(source_folder, image_file)
    image = cv2.imread(image_path)
    
    # 調整圖片大小，使最長邊為1000像素
    
    height, width, _ = image.shape
    if width > height:
        new_width = max_size
        new_height = int(height * max_size / width)
    else:
        new_height = max_size
        new_width = int(width * max_size / height)

    resized_image = cv2.resize(image, (new_width, new_height))
    cv2.imshow('Resized Image', resized_image)

    #cv2.waitKey(1)  # 必須呼叫waitKey以使imshow顯示窗口
    #window = gw.getWindowsWithTitle('Resized Image')[0]
    #window.activate()

    key = cv2.waitKey(0) & 0xFF
    if key == ord('z'):
        shutil.move(image_path, os.path.join(source_folder, target_folders[0], image_file))
    elif key == ord('x'):
        shutil.move(image_path, os.path.join(source_folder, target_folders[1], image_file))
    elif key == ord('v'):
        break

# 關閉所有圖片視窗
cv2.destroyAllWindows()
