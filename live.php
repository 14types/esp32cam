<?php

// Указываем хост и порт, на котором будем слушать UDP-пакеты
$host = 'localhost';  // 0.0.0.0 означает, что мы слушаем все интерфейсы
$port = YOUR_PORT;

// Создаем UDP-сокет
$socket = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);

// Привязываем сокет к указанному хосту и порту
socket_bind($socket, $host, $port);

$boundary = "boundary";
header('Access-Control-Allow-Origin: *', false);
header('Content-type: multipart/x-mixed-replace; boundary=' . $boundary);
ob_implicit_flush();

// Бесконечный цикл прослушивания и обработки пакетов
while (true) {
    // Принимаем данные от клиента
    socket_recvfrom($socket, $data, 100000, 0, $client_ip, $client_port);
    
    $jpeg_data = $data;
    
    echo 'Content-type: image/jpeg', PHP_EOL;
    echo 'Content-Length: ' . strlen($jpeg_data), PHP_EOL, PHP_EOL;
    echo $jpeg_data;
    echo '--', $boundary, PHP_EOL;
  
}

// Закрываем сокет (обычно не происходит в бесконечном цикле)
socket_close($socket);

?>
