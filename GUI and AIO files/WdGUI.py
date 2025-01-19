import serial
import tkinter as tk
from tkinter import messagebox

class WatchdogClient:
    """
    Classe principal para a interface do painel Watchdog Client.

    Atributos:
        ser (serial.Serial): Objeto para comunicação serial.
        root (tk.Tk): Janela principal da interface gráfica.
        display_var (tk.StringVar): Variável para exibir as teclas pressionadas.
        id_var (tk.StringVar): Variável para armazenar o ID do usuário.
        nome_var (tk.StringVar): Variável para armazenar o nome do usuário.
        url_var (tk.StringVar): Variável para armazenar a URL associada ao usuário.

    Métodos:
        abrir_login(): Abre a janela de login.
        abrir_criar_usuario(): Abre a janela para criar um novo usuário.
        desconectar(): Desconecta e fecha a porta serial.
        janela_login(): Configura a interface de login.
        janela_criar_usuario(): Configura a interface para criar usuários.
        ler_serial(): Lê dados da porta serial e atualiza o display.
    """

    def __init__(self, serial_port='/dev/ttyACM0', baudrate=9600):
        """
        Inicializa o painel Watchdog Client e configura a comunicação serial.

        Args:
            serial_port (str): Porta serial a ser utilizada para comunicação.
            baudrate (int): Taxa de transmissão em bits por segundo.
        """
        try:
            self.ser = serial.Serial(serial_port, baudrate)
        except serial.SerialException as e:
            messagebox.showerror("Erro Serial", f"Não foi possível conectar à porta {serial_port}: {e}")
            self.ser = None

        self.root = tk.Tk()
        self.root.title("Watchdog Client Panel")
        self.root.geometry("400x300")

        self.display_var = tk.StringVar()
        self.id_var = tk.StringVar()
        self.nome_var = tk.StringVar()
        self.url_var = tk.StringVar()

        self.configurar_janela_inicial()
        self.ler_serial()
        self.root.mainloop()

    def configurar_janela_inicial(self):
        """
        Configura a janela inicial com opções para login, criação de usuários e desconexão.
        """
        janela_inicial = tk.Frame(self.root)
        janela_inicial.pack(fill="both", expand=True)

        tk.Label(janela_inicial, text="Watchdog Client Panel", font=("Arial", 16, "bold")).pack(pady=20)
        tk.Button(janela_inicial, text="Fazer Login", font=("Arial", 14), command=self.abrir_login).pack(pady=10)
        tk.Button(janela_inicial, text="Criar Usuário", font=("Arial", 14), command=self.abrir_criar_usuario).pack(pady=10)
        tk.Button(janela_inicial, text="Desconectar", font=("Arial", 14), command=self.desconectar).pack(pady=10)

    def abrir_login(self):
        """Abre a janela de login."""
        self.root.withdraw()
        self.janela_login()

    def abrir_criar_usuario(self):
        """Abre a janela para criação de um novo usuário."""
        self.root.withdraw()
        self.janela_criar_usuario()

    def desconectar(self):
        """Fecha a porta serial e encerra o programa."""
        if self.ser:
            self.ser.close()
        messagebox.showinfo("Desconectado", "Você foi desconectado.")
        self.root.quit()

    def janela_login(self):
        """Configura a interface de login."""
        def enviar_dados_login():
            input_value = self.display_var.get()
            if input_value:
                messagebox.showinfo("Login", f"ID inserido: {input_value}")
                self.display_var.set("")
            else:
                messagebox.showwarning("Aviso", "Por favor, insira seu ID.")

        tela_login = tk.Toplevel(self.root)
        tela_login.title("Login")
        tk.Label(tela_login, text="Digite seu PIN:", font=("Arial", 14)).pack(pady=10)
        tk.Entry(tela_login, textvariable=self.display_var, font=("Arial", 18), width=20).pack(pady=10)
        tk.Button(tela_login, text="Enviar", font=("Arial", 14), command=enviar_dados_login).pack(pady=20)
        tk.Button(tela_login, text="Voltar", font=("Arial", 12),
                  command=lambda: [tela_login.destroy(), self.root.deiconify()]).pack()

    def janela_criar_usuario(self):
        """Configura a interface para criar um novo usuário."""
        def salvar_usuario():
            user_id = self.id_var.get()
            nome = self.nome_var.get()
            url = self.url_var.get()
            if user_id and nome and url:
                messagebox.showinfo("Usuário Criado", f"ID: {user_id}\nNome: {nome}\nURL: {url}")
                self.id_var.set("")
                self.nome_var.set("")
                self.url_var.set("")
            else:
                messagebox.showwarning("Aviso", "Por favor, preencha todos os campos.")

        tela_criar = tk.Toplevel(self.root)
        tela_criar.title("Criar Usuário")
        tk.Label(tela_criar, text="PIN:", font=("Arial", 12)).pack(pady=5)
        tk.Entry(tela_criar, textvariable=self.id_var, font=("Arial", 14)).pack(pady=5)
        tk.Label(tela_criar, text="Nome:", font=("Arial", 12)).pack(pady=5)
        tk.Entry(tela_criar, textvariable=self.nome_var, font=("Arial", 14)).pack(pady=5)
        tk.Button(tela_criar, text="Salvar", font=("Arial", 14), command=salvar_usuario).pack(pady=10)
        tk.Button(tela_criar, text="Voltar", font=("Arial", 12),
                  command=lambda: [tela_criar.destroy(), self.root.deiconify()]).pack()

    def ler_serial(self):
        """Lê dados da porta serial e atualiza o display com as teclas pressionadas."""
        if self.ser and self.ser.in_waiting > 0:
            key = self.ser.read().decode('utf-8')
            print(f"Tecla pressionada: {key}")
            self.display_var.set(self.display_var.get() + key)
        self.root.after(100, self.ler_serial)


if __name__ == "__main__":
    try:
        WatchdogClient()
    except KeyboardInterrupt:
        print("Programa encerrado pelo usuário.")
