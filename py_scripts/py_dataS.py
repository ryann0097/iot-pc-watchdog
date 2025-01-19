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


def createUser(nome, cod_id, url):
    try:
        conn = databaseInitiallize()
        cursor = conn.cursor()
        # Query para buscar o usuário
        query = "INSERT INTO users(nome, cod_id, url) VALUES (%s, %s, %s);"
        cursor.execute(query, (nome, cod_id, url))
    except (Exception, psycopg2.DatabaseError) as error:
        print(f"Erro ao inserir os dados: {error}")
    finally:
        # Fechar a conexão
        if conn:
            cursor.close()
            conn.close()
