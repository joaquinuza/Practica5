# Practica5
Comunicaciones Bluetooth y Bluetooth con esp32
El objetivo de esta práctica es la comunicación del esp32 con otros dispositivos de su entorno mediante comunicación Bluetooth y Bluetooth BLE, la cual es una red inalámbrica que opera en la banda de frecuencia 2.4Ghz y que posibilita la transmisión de datos entre dispositivos.
## Ejercicio 1
En este primer ejercicio se implementa un chat entre el esp32 y un móvil mediante Bluetooth con perfil SPP, este perfil (Serial Port Profile) es un perfil clásico en el cual se pueden enviar y recibir datos de la misma forma que con el protocolo serie habitual (líneas RX, TX).
## Ejercicio 2
En este segundo ejercicio se crea una característica GATT el esp32 hace de servidor y permite que otros dispositivos sean notificados la comunicación se realiza mediante Bluetooth BLE. Las características GATT (generic attribute profile) pueden tener unos identificadores los cuales están recogidos en la pagina web https://www.bluetooth.com/specifications/assigned-numbers/ identificando de manera única el dispositivo (e.g. dispositivo de sensado mediambiental, manos libres…)   
