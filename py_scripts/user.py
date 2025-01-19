class User:
    """
    Classe que representa um usuário com ID, código e nome.
    """
    def __init__(self, user_id, nome, cpu_url=None, mem_url=None, cTemp_url=None):
        self.user_id = user_id
        self.nome = nome
        self.cpu_url = cpu_url
        self.mem_url = mem_url
        self.cTemp_url = cTemp_url

    def __str__(self):
        return f"ID: {self.user_id}, Nome: {self.nome}, CPU URL: {self.cpu_url}, Mem URL: {self.mem_url}, cTemp URL: {self.cTemp_url}"



    def update_name(self, new_name: str):
        """
        Atualiza o nome do usuário.

        :param new_name: Novo nome do usuário.
        """
        if not new_name:
            raise ValueError("O nome não pode ser vazio.")
        self.name = new_name
