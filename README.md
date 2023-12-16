# esp32cam with PIR sensor + 18650 battery + deepsleep + tcp socket
esp32cam wake up from deepsleep by pir_pin, make shots, send them to tcp server and then go to sleep.

TCP server recieve shots, send them to UDP server, save shots as jpeg-files.

At the end of connection makes video and send to Telegram.

UDP server create mjpeg stream for online watch.

There is code for cam and for tcp and udp servers and 3d files for box.
![Screenshot 2023-12-16 233615](https://github.com/14types/esp32cam-pir-18650-deepsleep-tcp-socket/assets/34601503/40d68ae6-db9c-4456-a323-eca75d13091d)
