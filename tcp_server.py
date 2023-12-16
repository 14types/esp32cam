# Все картинки одним соединением. FPS~13
import socket
import time
import io
from PIL import Image
from io import BytesIO
import binascii
import subprocess
import os

def end_jpeg(data):
    # Проверяем, что длина данных достаточна для считывания сигнатуры JPEG (2 байта)
    if len(data) < 2:
        return False
    delimiter = b'\xFF\xD9'
    index = data.find(delimiter)
    if index == -1:
        return False
    substring1 = data[:index + len(delimiter)]
    substring2 = data[index + len(delimiter):]
    return substring1, substring2
    
def start_jpeg(data):
    if len(data) < 2:
        return False
    delimiter = b'\xFF\xD8\xFF'
    index = data.find(delimiter)
    if index == -1:
        return False
    substring1 = data[index:]
    substring2 = data[:index]
    return substring1, substring2

HOST = '0.0.0.0'
PORT = YOUR_PORT

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_socket.bind((HOST, PORT))
server_socket.listen(1)
server_socket.settimeout(5)

php_host = 'localhost'
php_port = YOUR_PORT
php_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

print(f"Listening on {HOST}:{PORT}")
est_dvij = False;
process = None
image_folder = '/path to images/'
framerate = 7

while True:
    try:
        client_socket, addr = server_socket.accept()
        client_socket.settimeout(3)
    except socket.timeout:
        print("Timed out waiting for the client.")
        if os.listdir(image_folder):
            if not process or process.poll() != None:
                process = subprocess.Popen(['python3', 'mp4.py'])
        continue

    data = b""
    firstimage = b""
    secondimage = b""
    kol_frames = 0
    subprocess.Popen(['python3', 'send_tg.py','Сработал датчик движения! Прямой эфир: YOUR LINK'])
    while True:
        try:
            packet = client_socket.recv(50000)
        except ConnectionResetError as e:
            print(f"Connection reset by peer: {e}")
            break;
        except socket.timeout:
            print(f"Connection socket.timeout")
            break;
        if not packet:
            break
        
        # Start
        delimiter = start_jpeg(packet)
        #end = end_jpeg(packet)
        if delimiter is not False:
            kol_frames += 1
            print('Кадр #' + str(kol_frames))
            if bool(firstimage):
                firstimage += delimiter[1]
                with open("images/" + str(int(time.time() * 1000)) + ".jpg", "wb") as file:
                    file.write(firstimage)
                try:
                    image = Image.open(BytesIO(firstimage))
                    php_socket.sendto(firstimage, (php_host, php_port))
                except Exception as e:
                    print(e)
                firstimage = delimiter[0]
            else:
                print('Пакет начинается с начала кадра')
                firstimage += delimiter[1]
            
        else:
            firstimage += packet
            
        continue
    
    client_socket.close()
    print('Соединение завершено.')
    
    if os.listdir(image_folder):
        if not process or process.poll() != None:
            process = subprocess.Popen(['python3', 'mp4.py'])
