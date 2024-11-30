import serial
import psutil
import time

ser = serial.Serial('COM3', 115200) # 115200 porque estamos conversando com um ESP32
time.sleep(2)

def enviar_dados():
    uso_cpu = psutil.cpu_percent()
    temperatura_cpu = psutil.sensors_temperatures().get('coretemp', [{}])[0].get('current', 0)
    uso_memoria = psutil.virtual_memory().percent

    dados = f"{uso_cpu},{temperatura_cpu},{uso_memoria}\n"
    ser.write(dados.encode())

while True:
    enviar_dados()
    time.sleep(120000)
