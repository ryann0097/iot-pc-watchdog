import psycopg2
from py_scripts.user import User


class Database:
    def __init__(self, dbname, user, password, host, port):
        self.dbname = dbname
        self.user = user
        self.password = password
        self.host = host
        self.port = port

    def database_initialize(self):
        """Inicializa a conexão com o banco de dados."""
        try:
            conn = psycopg2.connect(
                dbname=self.dbname,
                user=self.user,
                password=self.password,
                host=self.host,
                port=self.port)
            return conn        
        except Exception as e:
            print(f"Erro ao conectar ao banco de dados: {e}")

    def update_data(self, pc, temperatura, cpu, memoria):
        """Atualiza dados na tabela especificada."""
        try:
            conn = self.database_initialize()
            cursor = conn.cursor()
            query = f'INSERT INTO "{pc}" (temperatura, cpu, memoria) VALUES (%s, %s, %s);'
            cursor.execute(query, (temperatura, cpu, memoria))
            conn.commit()
        except Exception as e:
            print(f"Erro ao salvar dados no banco: {e}")
        finally:
            # Fechar a conexão
            if conn:
                cursor.close()
                conn.close()

    def search_user(self, id_cod):
        """Busca um usuário pelo id_cod e retorna um objeto User."""
        try:
            conn = self.database_initialize()
            cursor = conn.cursor()

            # Query para buscar o usuário
            query = "SELECT id_cod, nome, cpu_url, mem_url, cTemp_url FROM usuario WHERE id_cod = %s;"
            cursor.execute(query, (id_cod,))
            resultado = cursor.fetchone()

            if resultado:
                # Criando um objeto User com os dados retornados
                user = User(user_id=resultado[0], nome=resultado[1], cpu_url=resultado[2], mem_url=resultado[3], cTemp_url=resultado[4])
                return user
            else:
                print("Usuário não encontrado.")
                return None
        except (Exception, psycopg2.DatabaseError) as error:
            print(f"Erro ao buscar o usuário: {error}")
            return None
        finally:
            # Fechar a conexão
            if conn:
                cursor.close()
                conn.close()

    def create_user(self, nome, cod_id):
        """Cria um novo usuário no banco de dados."""
        try:
            conn = self.database_initialize()
            cursor = conn.cursor()

            # Inserir o usuário na tabela
            query_insert = "INSERT INTO usuario(nome, id_cod) VALUES (%s, %s);"
            cursor.execute(query_insert, (nome, cod_id))
            conn.commit()

            # Obter o ID do usuário recém-criado
            query_select = "SELECT id FROM usuario WHERE id_cod = %s;"
            cursor.execute(query_select, (cod_id,))  # O parâmetro deve ser uma tupla
            resultado = cursor.fetchone()

            if resultado:
                self.add_urls(resultado[0])  # Passa o ID para a função addUrls
                return True
            else:
                print("Usuário criado, mas o ID não foi encontrado.")
        except (Exception, psycopg2.DatabaseError) as error:
            print(f"Erro ao inserir os dados: {error}")
        finally:
            # Fechar a conexão
            if conn:
                cursor.close()
                conn.close()

    def add_urls(self, id):
        """Atualiza as URLs no banco de dados para o usuário com o ID fornecido."""
        try:
            conn = self.database_initialize()
            cursor = conn.cursor()

            # Gerar os URLs
            cpu_url = "https://io.adafruit.com/api/v2/ryansilv68/feeds/pc-watchdog-project-pilot.cpu-" + str(id)
            mem_url = "https://io.adafruit.com/api/v2/ryansilv68/feeds/pc-watchdog-project-pilot.mem-" + str(id)
            cTemp_url = "https://io.adafruit.com/api/v2/ryansilv68/feeds/pc-watchdog-project-pilot.c-temp-" + str(id)

            # Query para atualizar as URLs
            query = """
                UPDATE usuario 
                SET cpu_url = %s, mem_url = %s, cTemp_url = %s
                WHERE id = %s;
            """
            cursor.execute(query, (cpu_url, mem_url, cTemp_url, id))

            # Confirma a transação
            conn.commit()
            print(f"URLs atualizadas com sucesso para o usuário {id}.")
        except (Exception, psycopg2.DatabaseError) as error:
            print(f"Erro ao atualizar URLs: {error}")
        finally:
            # Fechar a conexão
            if conn:
                cursor.close()
                conn.close()
