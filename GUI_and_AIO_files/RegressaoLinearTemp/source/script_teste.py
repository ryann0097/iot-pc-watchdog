import psutil
import serial
import time
from sklearn.linear_model import LinearRegression
import numpy as np

# Função para coletar temperatura interna (CPU)
def get_cpu_temperature():
    sensors = psutil.sensors_temperatures()
    cpu_temp = sensors.get('coretemp', [])[0].current  # Depende do seu sistema
    return cpu_temp

# Função para coletar temperatura externa (supondo que você tenha um sensor externo)
def get_external_temperature():
    # Exemplo de uma leitura fictícia. Substitua pela sua fonte real de dados.
    return 25  # Temperatura ambiente em graus Celsius

# Conexão serial com o Arduino
ser = serial.Serial('/dev/ttyUSB0', 9600)  # Ajuste a porta conforme necessário

# Coletando dados para regressão linear
def collect_data():
    x = []  # Temperatura externa
    y = []  # Temperatura interna
    for _ in range(100):  # Coleta 100 amostras
        external_temp = get_external_temperature()
        internal_temp = get_cpu_temperature()
        x.append([external_temp])  # x é a temperatura externa
        y.append(internal_temp)    # y é a temperatura interna
        time.sleep(1)  # Intervalo de 1 segundo entre as leituras
    return np.array(x), np.array(y)

# Realizando a regressão linear
def perform_regression(x, y):
    model = LinearRegression()
    model.fit(x, y)
    return model

# Enviando os resultados para o Arduino
def send_to_arduino(temperature):
    ser.write(f"{temperature}\n".encode())

# Função principal
def main():
    x, y = collect_data()
    model = perform_regression(x, y)
    print("Modelo de regressão linear:", model.coef_, model.intercept_)
    while True:
        external_temp = get_external_temperature()
        predicted_internal_temp = model.predict([[external_temp]])[0]
        print(f"Temperatura externa: {external_temp}°C, Temperatura interna prevista: {predicted_internal_temp}°C")
        send_to_arduino(predicted_internal_temp)
        time.sleep(5)  # Envia os dados a cada 5 segundos

if __name__ == "__main__":
    main()
