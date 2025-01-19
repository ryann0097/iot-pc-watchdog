import serial
import psutil
import time
from py_dataS import Database

# Inicializando a classe Database com parâmetros de conexão
db = Database(dbname="railway", user="postgres", password="drDERankeuBdCgRoCjZUOlWsQvrgzUjO", host="junction.proxy.rlwy.net", port="48411")


#como usar
'''
# Criar um usuário
db.create_user("João Silva", "123456")

# Buscar um usuário
user = db.search_user("123456")
if user:
    print(user)
'''


#ser = serial.Serial('COM3', 115200) # 115200 porque estamos conversando com um ESP32
time.sleep(2)

def lerUser(user_id):
    return db.search_user(user_id)

def enviar_dados():
    uso_cpu = psutil.cpu_percent()
    temperatura_cpu = 99
    #temperatura_cpu = psutil.sensors_temperatures().get('coretemp', [{}])[0].get('current', 0)
    uso_memoria = psutil.virtual_memory().percent

    dados = f"{uso_cpu},{temperatura_cpu},{uso_memoria}\n"
    #ser.write(dados.encode())
    db.update_data("pc-01", temperatura_cpu, uso_cpu, uso_memoria)


while True:
    enviar_dados()
    time.sleep(120000)
