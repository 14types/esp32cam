# -*- coding: utf-8 -*-
import cv2
import os
import subprocess
import time
import tg
import sys
from PIL import Image

def is_jpeg_valid(file_path):
    try:
        Image.open(file_path).verify()
        return True
    except Exception as e:
        #print(f"Invalid JPEG: {file_path}, Error: {e}")
        return False

image_folder = '/var/www/html/udp_esp32cam/images/'
files_in_folder = os.listdir(image_folder)

for filename in files_in_folder:
    if filename.lower().endswith(('.jpg', '.jpeg')):
        file_path = os.path.join(image_folder, filename)
        is_valid = is_jpeg_valid(file_path)
        if not is_valid:
            print(f"{filename} is an invalid JPEG.")
            os.remove(file_path)

files_in_folder = os.listdir(image_folder)
framerate = 10
video_name = '/ path to videofile like ' + str(int(time.time())) + '.mp4'

if files_in_folder:
    
    print(video_name + " Кадров после проверки: " + str(len(files_in_folder)))
    # -loglevel panic 
    # nohup
    # -fflags +genpts 
    # -err_detect ignore_err 
    command = f"nohup ffmpeg -loglevel panic -r {framerate} -pattern_type glob -i '{image_folder}*.jpg' -y -c:v libx264 -pix_fmt yuv420p {video_name}"
    secodnprocess = subprocess.run(command, shell=True)
    #secodnprocess.wait()
    #print("secodnprocess.returncode " + str(secodnprocess.returncode))
    if not secodnprocess.returncode:
        print("Видео создано.")
    
        for file_name in files_in_folder:
            file_path = os.path.join(image_folder, file_name)
            if os.path.isfile(file_path):
                #file_path.endswith(".jpg") and 
                os.remove(file_path)
                
        if tg.video(video_name,''):
            print('Сообщение с видео успешно отправлено в Телеграм. Видео удалено.')
            os.remove(video_name)
        else:
            print('Ошибка отправки сообщения в Телеграм. Видео не удалено.')
            tg.text('Ошибка отправки видео.')
    
    else:
        print("Ошибка создания видео.")
        tg.text('Ошибка создания видео.')
            
            
else:
    print("Нет файлов для создания видео")
