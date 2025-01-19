import serial
import psutil
import time
from py_dataS import searchUser, updateData 


ser = serial.Serial('COM3', 115200) # 115200 porque estamos conversando com um ESP32
time.sleep(2)

def lerUser(user_id):
    return searchUser(user_id)

def enviar_dados():
    uso_cpu = psutil.cpu_percent()
    temperatura_cpu = psutil.sensors_temperatures().get('coretemp', [{}])[0].get('current', 0)
    uso_memoria = psutil.virtual_memory().percent

    dados = f"{uso_cpu},{temperatura_cpu},{uso_memoria}\n"
    ser.write(dados.encode())
    updateData("pc-01", temperatura, uso_cpu, uso_memoria)


while True:
    # 13A4DC : valor de entrada do keypad
    user = lerUser("13A4DC")
    print(f"usuario: {user}")
    enviar_dados()
    time.sleep(120000)
