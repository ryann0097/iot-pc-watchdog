import psycopg2
from user import User

def databaseInitiallize():
    try:
        conn = psycopg2.connect(
            dbname="railway",
            user="postgres",
            password="drDERankeuBdCgRoCjZUOlWsQvrgzUjO",
            host="junction.proxy.rlwy.net",
            port="48411")
        return conn        
    except Exception as e:
        print(f"Erro ao conncetar ao banco de dados: {e}")


def updateData(pc, temperatura, cpu, memoria):
    try:
        conn = databaseInitiallize()
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


def searchUser(user_id):
    try:
        conn = databaseInitiallize()
        cursor = conn.cursor()

        # Query para buscar o usuário pelo ID
        query = "SELECT id_cod, nome, url FROM users WHERE id_cod = %s;"
        cursor.execute(query, (user_id,))

        # Obter o resultado
        resultado = cursor.fetchone()
       
        if resultado:
            print(user_id)
            # Criar e retornar um objeto da classe User
            return User(user_id=resultado[0], name=resultado[1], url=resultado[2])
        else:
            return None
    except (Exception, psycopg2.DatabaseError) as error:
        print(f"Erro ao buscar o usuário: {error}")
        return None
    finally:
        # Fechar a conexão
        if conn:
            cursor.close()
            conn.close()


def createUser(nome, cod_id):
    try:
        conn = databaseInitiallize()
        cursor = conn.cursor()
        
        # Inserir o usuário na tabela
        query_insert = "INSERT INTO users(nome, id_cod) VALUES (%s, %s);"
        cursor.execute(query_insert, (nome, cod_id))
        conn.commit()

        # Obter o ID do usuário recém-criado
        query_select = "SELECT id FROM users WHERE id_cod = %s;"
        cursor.execute(query_select, (cod_id,))  # O parâmetro deve ser uma tupla
        resultado = cursor.fetchone()

        if resultado:
            addUrls(resultado[0])  # Passa o ID para a função addUrls
        else:
            print("Usuário criado, mas o ID não foi encontrado.")
    except (Exception, psycopg2.DatabaseError) as error:
        print(f"Erro ao inserir os dados: {error}")
    finally:
        # Fechar a conexão
        if conn:
            cursor.close()
            conn.close()


def addUrls(id):
    try:
        conn = databaseInitiallize()
        cursor = conn.cursor()

        # Gerar os URLs
        cpu_url = "https://io.adafruit.com/api/v2/ryansilv68/feeds/pc-watchdog-project-pilot.cpu-" + str(id)
        mem_url = "https://io.adafruit.com/api/v2/ryansilv68/feeds/pc-watchdog-project-pilot.mem-" + str(id)
        cTemp_url = "https://io.adafruit.com/api/v2/ryansilv68/feeds/pc-watchdog-project-pilot.c-temp-" + str(id)

        # Query para atualizar as URLs
        query = """
            UPDATE users 
            SET cpu_url = %s, mem_url = %s, cTemp_url = %s
            WHERE id = %s;
        """
        cursor.execute(query, (cpu_url, mem_url, cTemp_url, id))

        # Confirma a transação
        conn.commit()
        print(f"URLs atualizadas com sucesso para o usuário {id}.")

    except (Exception, psycopg2.DatabaseError) as error:
        print(f"Erro ao atualizar URLs: {error}")
        return None
    finally:
        # Fechar a conexão
        if conn:
            cursor.close()
            conn.close()