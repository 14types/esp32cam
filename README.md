# esp32cam with PIR sensor + 18650 battery + deepsleep + tcp socket
esp32cam wake up from deepsleep by pir_pin, make shots, send them to tcp server and then go to sleep.
TCP server recieve shots, send them to UDP server, save shots as jpeg-files.
At the end of connection makes video and send to Telegram.
UDP server create mjpeg stream for online watch.
There is code for cam and for tcp and udp servers.
